intelligent-shell/
├── src/
│   ├── main.cpp
│   ├── exec.cpp
│   ├── parser.cpp
│   ├── bridge.cpp
│   └── shell.h
├── ai_module/
│   ├── translator.py
│   ├── corrector.py
│   ├── suggestor.py
│   └── ai_utils.py
├── include/
│   └── shell.h
├── tests/
│   ├── test_shell.cpp
│   ├── test_ai.py
│   └── integration_test.py
├── requirements.txt
├── CMakeLists.txt
├── Makefile
└── README.md
# Intelligent Shell (AI Command-Line Assistant)

A smart shell that interprets natural language and converts it to bash commands using local Ollama models.

## Features

- Natural language to bash command translation
- Command correction for typos
- Predictive command suggestions
- Pipe and redirection support
- Background job execution
- History-based learning

## Installation

1. Install dependencies:
```bash
sudo apt-get install libreadline-dev cmake python3-pip
pip3 install -r requirements.txt
