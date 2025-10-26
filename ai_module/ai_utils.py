import requests
import json
import subprocess
import os
from difflib import SequenceMatcher

class AIUtils:
    def __init__(self):
        self.ollama_url = "http://localhost:11434/api/generate"
        self.history_file = os.path.expanduser("~/.intellishell_history")
        self.command_patterns = self.load_command_patterns()
    
    def load_command_patterns(self):
        # Common command patterns for ~40% accuracy prediction
        return {
            "show": ["ls", "cat", "less", "more"],
            "list": ["ls", "dir", "find"],
            "find": ["find", "grep", "locate"],
            "check": ["git status", "ps", "df", "free"],
            "create": ["touch", "mkdir", "nano", "vim"],
            "delete": ["rm", "rmdir"],
            "move": ["mv", "cp"],
            "search": ["grep", "ack", "ag"],
            "process": ["ps", "top", "htop"],
            "network": ["ping", "curl", "wget", "ifconfig"]
        }
    
    def query_ollama(self, prompt, model="mistral"):
        """Query local Ollama model for command translation"""
        payload = {
            "model": model,
            "prompt": f"Translate this natural language to bash command: {prompt}. Return only the bash command without explanation.",
            "stream": False
        }
        
        try:
            response = requests.post(self.ollama_url, json=payload, timeout=10)
            if response.status_code == 200:
                return response.json().get("response", "").strip()
            else:
                return f"echo 'Error: Ollama server not responding'"
        except requests.exceptions.RequestException:
            return f"echo 'Error: Cannot connect to Ollama server'"
    
    def similarity(self, a, b):
        """Calculate similarity between two strings"""
        return SequenceMatcher(None, a.lower(), b.lower()).ratio()
    
    def load_history(self):
        """Load command history for suggestions"""
        if os.path.exists(self.history_file):
            with open(self.history_file, 'r') as f:
                return [line.strip() for line in f.readlines()[-100:]]  # Last 100 commands
        return []
    
    def save_to_history(self, command):
        """Save command to history file"""
        with open(self.history_file, 'a') as f:
            f.write(command + '\n')
