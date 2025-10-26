#include "shell.h"
#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <signal.h>
#include <fstream>
#include <cstdlib>

IntelligentShell::IntelligentShell() {
    current_dir = getcwd(nullptr, 0);
    load_history();
    setup_signal_handlers();
}

IntelligentShell::~IntelligentShell() {
}

void IntelligentShell::run() {
    std::cout << "Intelligent Shell v1.0 - AI Command-Line Assistant" << std::endl;
    std::cout << "Type natural language commands or use 'help' for assistance" << std::endl;
    
    while (true) {
        char* input = readline("intellishell> ");
        
        if (!input) break; // EOF
        
        std::string command(input);
        free(input);
        
        if (command.empty()) continue;
        
        // Add to history
        add_history(command.c_str());
        save_history(command);
        
        // Check if it's natural language (starts with non-command words)
        bool is_natural_lang = true;
        std::vector<std::string> common_cmds = {"ls", "cd", "pwd", "git", "echo", "cat", "grep"};
        
        for (const auto& cmd : common_cmds) {
            if (command.find(cmd) == 0) {
                is_natural_lang = false;
                break;
            }
        }
        
        if (is_natural_lang && command.length() > 10) {
            // Use AI translation
            std::cout << "Translating to bash command..." << std::endl;
            std::string translated = translate_natural_language(command);
            
            if (!translated.empty() && translated != "echo 'Error: Cannot connect to Ollama server'") {
                std::cout << "Suggested command: " << translated << std::endl;
                std::cout << "Execute? (y/n): ";
                
                std::string response;
                std::getline(std::cin, response);
                
                if (response == "y" || response == "yes") {
                    execute_command(translated);
                }
            } else {
                execute_command(command); // Fallback to direct execution
            }
        } else {
            execute_command(command);
        }
    }
}

void IntelligentShell::execute_command(const std::string& command) {
    if (command.empty()) return;
    
    // Check for built-in commands
    auto args = parse_command(command);
    if (args.empty()) return;
    
    const std::string& cmd = args[0];
    
    if (cmd == "cd" || cmd == "exit") {
        handle_builtin(args);
        return;
    }
    
    // Check for pipes
    if (has_pipes(command)) {
        execute_piped_commands(command);
        return;
    }
    
    // Check for redirection
    bool has_redirect = false;
    for (const auto& arg : args) {
        if (arg == ">" || arg == "<" || arg == ">>") {
            has_redirect = true;
            break;
        }
    }
    
    if (has_redirect) {
        handle_redirection(args);
        return;
    }
    
    // Check for background job
    if (args.back() == "&") {
        handle_background_job(args);
        return;
    }
    
    // Regular command execution
    execute_external(args);
}

void IntelligentShell::handle_background_job(const std::vector<std::string>& args) {
    std::vector<std::string> cmd_args(args.begin(), args.end() - 1); // Remove '&'
    
    pid_t pid = fork();
    if (pid == 0) {
        // Child process
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
        std::cout << "Background job started with PID: " << pid << std::endl;
        // Don't wait for background process
    }
}

void IntelligentShell::load_history() {
    std::ifstream file(".intellishell_history");
    std::string line;
    
    while (std::getline(file, line)) {
        history.push_back(line);
    }
}

void IntelligentShell::save_history(const std::string& command) {
    history.push_back(command);
    
    std::ofstream file(".intellishell_history", std::ios::app);
    file << command << std::endl;
}

void IntelligentShell::setup_signal_handlers() {
    // Ignore SIGINT for background processes
    signal(SIGINT, SIG_IGN);
}
