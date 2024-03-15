make comp && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=./debug/ignore_readline.supp -s ./crash
