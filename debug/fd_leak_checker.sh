#!/bin/bash

valgrind --leak-check=full --trace-children=yes ./minishell &
VALGRIND_PID=$!

wait $VALGRIND_PID

# check for open fds at end of execution
lsof -p $VALGRIND_PID
