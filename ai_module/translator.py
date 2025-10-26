#!/usr/bin/env python3
import sys
import json
from ai_utils import AIUtils

def translate_natural_language(input_text):
    ai = AIUtils()
    
    # Get translation from Ollama
    bash_command = ai.query_ollama(input_text)
    
    # Clean up the response
    bash_command = bash_command.replace('```bash', '').replace('```', '').strip()
    
    # Validate and return top 3 suggestions with confidence
    suggestions = [
        {"command": bash_command, "confidence": 0.85},
        {"command": get_alternative_suggestion(input_text), "confidence": 0.70},
        {"command": get_fallback_suggestion(input_text), "confidence": 0.60}
    ]
    
    return suggestions

def get_alternative_suggestion(input_text):
    """Generate alternative suggestion based on patterns"""
    input_lower = input_text.lower()
    
    if "show" in input_lower or "list" in input_lower:
        if "file" in input_lower or "large" in input_lower:
            return "ls -lh"
        elif "process" in input_lower:
            return "ps aux"
        else:
            return "ls -la"
    
    elif "find" in input_lower or "search" in input_lower:
        if "python" in input_lower:
            return "ps aux | grep python"
        elif "file" in input_lower:
            return "find . -name \"*\" -type f"
        else:
            return "grep -r \"pattern\" ."
    
    elif "git" in input_lower:
        if "branch" in input_lower:
            return "git branch"
        elif "status" in input_lower:
            return "git status"
        else:
            return "git log --oneline"
    
    else:
        return "echo 'Command not recognized'"

def get_fallback_suggestion(input_text):
    """Fallback suggestion for unknown commands"""
    return "echo 'Try: ls, cd, pwd, git status, ps aux'"

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: translator.py <input_file> <output_file>")
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    
    # Read input
    with open(input_file, 'r') as f:
        data = json.load(f)
        input_text = data.get("input", "")
    
    # Translate
    suggestions = translate_natural_language(input_text)
    
    # Write output
    with open(output_file, 'w') as f:
        json.dump({"suggestions": suggestions}, f)
