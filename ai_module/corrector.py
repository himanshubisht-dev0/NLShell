#!/usr/bin/env python3
import sys
import json
from ai_utils import AIUtils

def correct_command(command):
    ai = AIUtils()
    
    # Common command corrections
    corrections = {
        "psuh": "push",
        "branhc": "branch",
        "stauts": "status",
        "commti": "commit",
        "ad": "add",
        "puhs": "push",
        "pul": "pull",
        "chekcout": "checkout",
        "maste": "master",
        "main": "main"
    }
    
    # Split command and correct each part
    parts = command.split()
    corrected_parts = []
    
    for part in parts:
        if part in corrections:
            corrected_parts.append(corrections[part])
        else:
            corrected_parts.append(part)
    
    corrected_command = " ".join(corrected_parts)
    
    # If correction didn't help, use AI
    if corrected_command == command:
        prompt = f"Correct this potential typo in bash command: {command}"
        corrected_command = ai.query_ollama(prompt)
        corrected_command = corrected_command.replace('```bash', '').replace('```', '').strip()
    
    return corrected_command

if __name__ == "__main__":
    if len(sys.argv) != 3:
        print("Usage: corrector.py <input_file> <output_file>")
        sys.exit(1)
    
    input_file = sys.argv[1]
    output_file = sys.argv[2]
    
    # Read input
    with open(input_file, 'r') as f:
        data = json.load(f)
        command = data.get("command", "")
    
    # Correct
    corrected = correct_command(command)
    
    # Write output
    with open(output_file, 'w') as f:
        json.dump({"corrected": corrected}, f)
