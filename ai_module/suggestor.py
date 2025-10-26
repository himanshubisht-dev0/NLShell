#!/usr/bin/env python3
import sys
import json
from ai_utils import AIUtils

def get_suggestions(partial_command, history):
    ai = AIUtils()
    suggestions = []
    
    # Load recent history for context
    recent_commands = history[-10:]  # Last 10 commands
    
    # Pattern-based suggestions (~40% accuracy target)
    if partial_command.startswith("git"):
        suggestions.extend([
            "git status",
            "git branch",
            "git log --oneline",
            "git pull",
            "git push"
        ])
    elif partial_command.startswith("ls"):
        suggestions.extend([
            "ls -la",
            "ls -lh",
            "ls -t",
            "ls -R"
        ])
    elif partial_command.startswith("find"):
        suggestions.extend([
            "find . -name \"*.py\"",
            "find . -type f -size +1M",
            "find . -mtime -7"
        ])
    
    # History-based suggestions
    for cmd in recent_commands:
        if cmd.startswith(partial_command) and cmd not in suggestions:
            suggestions.append(cmd)
    
    # AI-based completion for complex cases
    if len(suggestions) < 3:
        prompt = f"Complete this bash command: {partial_command}"
        ai_suggestion = ai.query_ollama(prompt)
        ai_suggestion = ai_suggestion.replace('```bash', '').replace('```', '').strip()
        
        if ai_suggestion and ai_suggestion not in suggestions:
            suggestions.append(ai_suggestion)
    
    return suggestions[:5]  # Return top 5 suggestions

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: suggestor.py <input_file> <output_file>")
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    
    # Read input
    with open(input_file, 'r') as f:
        data = json.load(f)
        partial = data.get("partial", "")
        history = data.get("history", [])
    
    # Get suggestions
    suggestions = get_suggestions(partial, history)
    
    # Write output
    with open(output_file, 'w') as f:
        json.dump({"suggestions": suggestions}, f)
