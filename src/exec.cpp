#include "shell.h"
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#include <signal.h>
#include <iostream>
#include <cstring>
#include <sstream>
void IntelligentShell::handle_builtin(const std::vector<std::string>& args) {
    if (args.empty()) return;
    
    const std::string& cmd = args[0];
    
    if (cmd == "cd") {
        if (args.size() > 1) {
            if (chdir(args[1].c_str()) != 0) {
                std::cerr << "cd: " << args[1] << ": No such directory" << std::endl;
            }
        } else {
            chdir(getenv("HOME"));
        }
    } else if (cmd == "exit") {
        exit(0);
    }
}

void IntelligentShell::execute_external(const std::vector<std::string>& args) {
    if (args.empty()) return;
    
    // Convert vector to char* array
    char** argv = new char*[args.size() + 1];
    for (size_t i = 0; i < args.size(); i++) {
        argv[i] = new char[args[i].size() + 1];
        strcpy(argv[i], args[i].c_str());
    }
    argv[args.size()] = nullptr;
    
    pid_t pid = fork();
    
    if (pid == 0) {
        // Child process
        execvp(argv[0], argv);
        std::cerr << "Command not found: " << args[0] << std::endl;
        exit(1);
    } else if (pid > 0) {
        // Parent process
        int status;
        waitpid(pid, &status, 0);
    } else {
        std::cerr << "Fork failed!" << std::endl;
    }
    
    // Cleanup
    for (size_t i = 0; i < args.size(); i++) {
        delete[] argv[i];
    }
    delete[] argv;
}

void IntelligentShell::execute_piped_commands(const std::string& command) {
    std::vector<std::string> commands;
    std::stringstream ss(command);
    std::string cmd;
    
    while (std::getline(ss, cmd, '|')) {
        commands.push_back(cmd);
    }
    
    int num_commands = commands.size();
    int pipefds[2*(num_commands-1)];
    
    // Create pipes
    for (int i = 0; i < num_commands - 1; i++) {
        if (pipe(pipefds + i*2) < 0) {
            std::cerr << "Pipe creation failed" << std::endl;
            return;
        }
    }
    
    // Execute commands
    for (int i = 0; i < num_commands; i++) {
        pid_t pid = fork();
        
        if (pid == 0) {
            // Child process
            if (i > 0) {
                // Redirect stdin from previous pipe
                dup2(pipefds[(i-1)*2], STDIN_FILENO);
            }
            
            if (i < num_commands - 1) {
                // Redirect stdout to next pipe
                dup2(pipefds[i*2 + 1], STDOUT_FILENO);
            }
            
            // Close all pipe file descriptors
            for (int j = 0; j < 2*(num_commands-1); j++) {
                close(pipefds[j]);
            }
            
            // Execute command
            auto args = parse_command(commands[i]);
            if (args.empty()) exit(0);
            
            char** argv = new char*[args.size() + 1];
            for (size_t j = 0; j < args.size(); j++) {
                argv[j] = new char[args[j].size() + 1];
                strcpy(argv[j], args[j].c_str());
            }
            argv[args.size()] = nullptr;
            
            execvp(argv[0], argv);
            std::cerr << "Command not found: " << args[0] << std::endl;
            exit(1);
        }
    }
    
    // Close all pipe file descriptors in parent
    for (int i = 0; i < 2*(num_commands-1); i++) {
        close(pipefds[i]);
    }
    
    // Wait for all child processes
    for (int i = 0; i < num_commands; i++) {
        wait(NULL);
    }
}

void IntelligentShell::handle_redirection(const std::vector<std::string>& args) {
    // Implementation for >, <, >> redirection
    // This is a simplified version
    std::vector<std::string> cmd_args;
    std::string input_file, output_file;
    bool append = false;
    
    for (size_t i = 0; i < args.size(); i++) {
        if (args[i] == ">") {
            if (i + 1 < args.size()) {
                output_file = args[i + 1];
                i++;
            }
        } else if (args[i] == ">>") {
            if (i + 1 < args.size()) {
                output_file = args[i + 1];
                append = true;
                i++;
            }
        } else if (args[i] == "<") {
            if (i + 1 < args.size()) {
                input_file = args[i + 1];
                i++;
            }
        } else {
            cmd_args.push_back(args[i]);
        }
    }
    
    if (cmd_args.empty()) return;
    
    pid_t pid = fork();
    if (pid == 0) {
        // Handle input redirection
        if (!input_file.empty()) {
            int fd_in = open(input_file.c_str(), O_RDONLY);
            if (fd_in >= 0) {
                dup2(fd_in, STDIN_FILENO);
                close(fd_in);
            }
        }
        
        // Handle output redirection
        if (!output_file.empty()) {
            int flags = O_WRONLY | O_CREAT;
            if (append) flags |= O_APPEND;
            else flags |= O_TRUNC;
            
            int fd_out = open(output_file.c_str(), flags, 0644);
            if (fd_out >= 0) {
                dup2(fd_out, STDOUT_FILENO);
                close(fd_out);
            }
        }
        
        // Execute command
        char** argv = new char*[cmd_args.size() + 1];
        for (size_t i = 0; i < cmd_args.size(); i++) {
            argv[i] = new char[cmd_args[i].size() + 1];
            strcpy(argv[i], cmd_args[i].c_str());
        }
        argv[cmd_args.size()] = nullptr;
        
        execvp(argv[0], argv);
        std::cerr << "Command not found: " << cmd_args[0] << std::endl;
        exit(1);
    } else if (pid > 0) {
        wait(NULL);
    }
}
