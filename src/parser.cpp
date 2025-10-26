#include "shell.h"
#include <sstream>
#include <vector>
#include <algorithm>
#include <cstring>

std::vector<std::string> IntelligentShell::parse_command(const std::string& input) {
    std::vector<std::string> tokens;
    std::stringstream ss(input);
    std::string token;
    
    while (ss >> token) {
        // Handle quotes
        if (token.front() == '"' || token.front() == '\'') {
            char quote = token.front();
            std::string quoted_token = token.substr(1);
            
            while (ss >> token) {
                if (token.back() == quote) {
                    quoted_token += " " + token.substr(0, token.length() - 1);
                    tokens.push_back(quoted_token);
                    break;
                } else {
                    quoted_token += " " + token;
                }
            }
        } else {
            tokens.push_back(token);
        }
    }
    
    return tokens;
}

bool IntelligentShell::has_pipes(const std::string& command) {
    return command.find('|') != std::string::npos;
}
