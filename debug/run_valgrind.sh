#!/bin/bash

make re && make clean && \
			valgrind --leak-check=full \
			--show-leak-kinds=all \
			--track-origins=yes \
			--trace-children=yes \
			--track-fds=yes \
			--gen-suppressions=all \
			--suppressions=./debug/readline.supp \
			--log-file=./debug/log.txt \
			--error-limit=no \
			-s ./minishell

# --time-stamp=yes \
# --verbose \
