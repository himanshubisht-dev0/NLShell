
# Intelligent Shell (AI Command-Line Assistant)

A smart shell that interprets natural language and converts it to bash commands using local Ollama models.

## Features

- Natural language to bash command translation
- Command correction for typos
- Predictive command suggestions
- Pipe and redirection support
- Background job execution
- History-based learning
- 
# NLShell - Natural Language Command-Line Assistant

NLShell is an intelligent shell that interprets natural language and converts it to bash commands using local Ollama models. It provides an intuitive interface for users who prefer natural language over memorizing complex command syntax.

![NLShell Demo](https://via.placeholder.com/800x400?text=NLShell+Demo)

## Features

- 🗣️ **Natural Language Processing**: Type commands in plain English
- 🔄 **Command Translation**: Converts natural language to bash commands
- ✅ **Command Correction**: Automatically fixes typos in commands
- 💡 **Predictive Suggestions**: Offers command suggestions based on input
- 📊 **Pipe and Redirection Support**: Full support for command chaining
- 🔙 **Background Job Execution**: Run processes in the background
- 📚 **History-Based Learning**: Improves suggestions based on usage patterns

## Prerequisites

- Linux/WSL environment
- C++ compiler (GCC/Clang)
- Python 3.6+
- CMake 3.10+
- Ollama (for local AI model)

## Installation

### 1. Clone the repository

```bash
git clone https://github.com/yourusername/NLShell.git
cd NLShell
```

### 2. Install dependencies

```bash
# Install system dependencies
sudo apt-get install libreadline-dev cmake python3-pip

# Install Python dependencies
pip3 install -r requirements.txt

# Install and start Ollama (if not already installed)
# Follow instructions at https://ollama.ai/
```

### 3. Build the project

```bash
mkdir -p build
cd build
cmake ..
make
```

### 4. Run Ollama server

Make sure the Ollama server is running in a separate terminal:

```bash
ollama serve
```

### 5. Launch NLShell

```bash
# From the project root directory
./build/intellishell
```

## Usage Examples

intellishell> show me large files in this directory
🤖 Translating to bash command...
💡 Suggested command: ls -lh
Execute? (y/n): y

total 48K
-rw-r--r-- 1 user user  12K Dec 10 10:30 large_file.txt
-rwxr-xr-x 1 user user  25K Dec 10 09:15 executable

intellishell> check my git branch status
🤖 Translating to bash command...
💡 Suggested command: git branch
Execute? (y/n): y

* main
  feature-branch

intellishell> find all python processes running
🤖 Translating to bash command...
💡 Suggested command: ps aux | grep python
Execute? (y/n): y

user     1234  0.0  0.5  12345  6789 ?        S    10:30   0:00 python3 app.py

intellishell> git psuh origin main
🤖 Detected potential typo: 'psuh' → 'push'
💡 Corrected command: git push origin main
Execute? (y/n): y


### Basic Commands
Natural Language Examples:Natural Language	Translated Command
"show hidden files":	ls -la
"search for text in files":	grep -r "text" .
"check disk space":	df -h
"monitor system resources"	: top
"compress a folder":	tar -czf archive.tar.gz folder/
"count lines in file":	wc -l file.txt
"find large files":	find . -size +10M
"kill process by name"	:pkill process_name

