#include "shell.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

// Simple JSON-based bridge to Python AI module
std::string IntelligentShell::translate_natural_language(const std::string& natural_lang) {
    // Write input to temporary file
    std::ofstream input_file("/tmp/intellishell_input.json");
    input_file << "{\"input\": \"" << natural_lang << "\"}";
    input_file.close();
    
    // Call Python translator
    std::string command = "python3 ai_module/translator.py /tmp/intellishell_input.json /tmp/intellishell_output.json";
    system(command.c_str());
    
    // Read output
    std::ifstream output_file("/tmp/intellishell_output.json");
    std::string json_result;
    if (output_file.is_open()) {
        std::getline(output_file, json_result);
        output_file.close();
    }
    
    // Simple JSON parsing to extract the first command
    std::string command_to_execute;
    size_t cmd_pos = json_result.find("\"command\": \"");
    if (cmd_pos != std::string::npos) {
        cmd_pos += 12; // Length of "command": "
        size_t end_pos = json_result.find("\"", cmd_pos);
        if (end_pos != std::string::npos) {
            command_to_execute = json_result.substr(cmd_pos, end_pos - cmd_pos);
        }
    }
    
    return command_to_execute;
}

std::string IntelligentShell::correct_command(const std::string& command) {
    // Similar implementation for correction
    return command; // Placeholder
}

std::vector<std::string> IntelligentShell::get_suggestions(const std::string& partial) {
    return std::vector<std::string>(); // Placeholder
}
