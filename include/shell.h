#ifndef SHELL_H
#define SHELL_H

#include <vector>
#include <string>
#include <map>

class IntelligentShell {
public:
    IntelligentShell();
    ~IntelligentShell();
    
    void run();
    void execute_command(const std::string& command);
    std::vector<std::string> parse_command(const std::string& input);
    void handle_builtin(const std::vector<std::string>& args);
    void execute_external(const std::vector<std::string>& args);
    bool has_pipes(const std::string& command);
    void execute_piped_commands(const std::string& command);
    void handle_redirection(const std::vector<std::string>& args);
    void handle_background_job(const std::vector<std::string>& args);
    
    // AI Integration
    std::string translate_natural_language(const std::string& natural_lang);
    std::string correct_command(const std::string& command);
    std::vector<std::string> get_suggestions(const std::string& partial);
    
private:
    std::string current_dir;
    std::map<std::string, std::string> aliases;
    std::vector<std::string> history;
    void load_history();
    void save_history(const std::string& command);
    void setup_signal_handlers();
};

#endif
