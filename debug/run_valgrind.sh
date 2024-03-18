make comp && valgrind --leak-check=full --show-leak-kinds=all --track-origins=yes --suppressions=./debug/readline.supp --gen-suppressions=all --log-file=./debug/log.txt -s ./crash
