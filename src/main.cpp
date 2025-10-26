#include "shell.h"
#include <iostream>
#include <readline/readline.h>
#include <readline/history.h>

int main() {
    IntelligentShell shell;
    shell.run();
    return 0;
}
