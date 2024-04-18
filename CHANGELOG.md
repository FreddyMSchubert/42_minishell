# CHANGELOG minishell by nburchha & fschuber

### v0.28.3 - 18.4.24 - Freddy
- added some better free protection in expander & wildcard

### v0.28.2 - 18.4.24 - Freddy
- replaces malloc call in ms_split with calloc to avoid a potential jump on uninitialized value

### v0.28.1 - 04.04.24 - Freddy 
- better error logging function, used in executer
- small wrong check fix in execute_command_utils F_OK -> X_OK
- fixed error message not printing (No such file or directory) for random inputs, e.g. adfosdfnsdfl.
	- this was surprisingly hard and my brain is in pain

## v0.28.0 - 03.04.24 - Freddy - 1670 / 2823
- fixed export not working like bash bug for var-value pairs where var is only 1 char long
- some fixes, and norminettification of envcp utils

## v0.27.0 - 02.04.24 - Freddy - 1670 / 2823
- moved builtins out of child processes
	- made all of htem print to the outfd using ft_putstr_fd
- fully removed wholly unnecessary cmd_start_index variable

## v0.26.0 - 02.04.24 - Freddy
- changes to make testing easy
	- added DEBUG macro
	- renamed executable to minishell
- made cd - work (theoretically), but cant test that yet lol

### v0.25.4 - 02.04.24 - Freddy
- made Makefile put object files into ./obj folder for cleanliness

### v0.25.3 - 02.04.24 - Freddy
- error handling
- multiple fixes

### v0.25.2 - 26.03.24 - Freddy
- cleanup of removal of command_start_index leftover code weirdness

### v0.25.1 - 26.03.24 - Freddy
- some general refactorings
- parser bracket logic fix
- other minor fixes

## v0.25.0 - 25.03.24 - Freddy
- started substring update to remove ignore flag
- currently dysfunctional

### v0.24.3 - 25.03.24 - Freddy
- redirect/get_filename NULL segfault fix
- false lexer builtin token type fix

### v0.24.2 - 20.03.24 - Freddy
- memory leak fixes
	- new input, as allocated by put spaces between tokens, freed in ms split
	- gänsefüsschen removed string double free
- added pid printout, very helpful
- removed some unnecessary print statements

### v0.24.1 - 20.03.24 - Freddy
- input loop reordering. moved all visual / unnecessary stuff into own file; some refactorings in those functions
- stopped makefile from printing unwanted tabs & some other changes

## v0.24.0 - 19.03.24 - Freddy
- made heredoc lines get expanded
- currently, stuff inside "" is not yet not expanded. that logic has to be handled in the expander itself, this is only the heredoc side of the heredoc expansion for now.

### v0.23.9 - 19.03.24 - Freddy
- Added garbage collector element appending duplicate protection
- added some weird ls leaks to the suppressions file
- some more cases testing
- malloc fixes
	- child assets free after execve failed
	- child process exit duplicate gc free problem solved

### v0.23.8 - 18.03.24 - Freddy
- Removed some unnecessary print statements
- added last pid printout, helpful for understanding valgrind logs
- many tweaks and fixes and whatever ngl i lost track
- child processes dont properly free everything on exit. envp works, but the garbage collector doesnt get properly cleaned. added child_process_exit to handle that stuff.

### v0.23.7 - 18.03.24 - Freddy
- Added ft_lstlen to libft submodule for testing
- garbage collector tweaks
- more detailed run command which tracks children + file descriptors
- Malloc fixes
	- further lexer detect_toen_type fixes
	- parser new node allocation not being appended fix
	- parser_util/sub_token_t_list not appending after calling ft_lstnew fixed
- Fd fixes
	- heading fd wasn't being closed in input_loop/print_heading

### v0.23.6 - 18.03.24 - Freddy
- history not working bugfix
- added log.txt to gitingore, deleted it from git
- exit was directly exiting main program, not just setting exit flag bug fixed
- some optimizations
- some garbage collector logic improvements
- malloc fixes
	- mallocs not being freed on exit fixed
	- expander/get_rid_of_quotes didnt append new_str to gc
	- garbage collector itself was not being freed

### v0.23.5 - 18.03.24 - Freddy
- created log_to_supp.sh which takes in a valgrind log file generated with --gen-suppressions=all and extracts the suppressions for readline (or anything else really)
- ran that, generated readline.supp which should adequately suppress anything useless to us while we work with valgrind

### v0.23.4 - 15.03.24 - Freddy
- Malloc fixes
	- missing getwd return free in 4/builtins/cd
	- parser allocated nodes not appended to gc
	- new arrays as created by t_list_to_token_arr not appended to gc
	- new tokens as allocated by detect_token_type never got appended to gc. which is to say, every single token we ever used not allocated to gc. (same for their values)
- added debug folder with debug files and run_valgrind. also added a suppression file that stops makes the annoying readline leaks not show up
- added rl_clear_history at the end of the garbage collector cleanup

### v0.23.3 - 15.03.24 - Freddy
- fully removed get_next_line (makefile wasnt in commit)
- various small fixes

### v0.23.2 - 15.03.24 - Freddy
- removed get_next_line because it's unnecessary (it was never here...)

### v0.23.1 - 15.03.24 - Freddy
- logical operators dont wait until left child is finished to evaluate right child, making them pretty useless. fixed
- removed unnecessary print statements accidentally pushed in arraytolist v4

## v0.23.0 - 15.03.24 - Freddy
- Made Makefile colorful, more minimal compilation messages
- updated logo & colored input prompt

### [ArrayToList] - v4 - 15.03.24 - Freddy
- switched parser to lists. some other fixes
- validator referencing wrong token numbering fixed. now uses macros instead of numbers.
- some other validator logic fixes
### [ArrayToList] - v3 - 14.03.24 - Freddy
- switched expander to lists. some other fixes
### [ArrayToList] - v2 - 14.03.24 - Freddy
- switched validator to lists. redirects and log ops seem to work fine now.
### [ArrayToList] - v1 - 14.03.24 - Freddy
- switched lexer to lists. unable to test redirects and logical operators

## v0.21.0 - 13.03.24 - Freddy
- Added signals. ctrl-c & ctrl-d & ctrl-\

## v0.20.0 - 07.03.24 - Freddy
- fixed reorder args and redirect function segfaulting when left is null
- fixed urgent bug in child processes of normal executions, which return instead of exiting on error, therefore causing mayhem in the rest of the program

## v0.19.0 - 07.03.24 - Freddy
- created argument reordering function for redirects

## v0.18.0 - 06.03.24 - Freddy
- implemented here_doc
- parents now return childs exit status
- fixed some bugs in the executer
- fixed new line not resetting program return value

## v0.17.0 - 04.03.24 - Freddy
- Added ft_isspace to libft submodule, removed from ms_split
- fixed whitespace crashing program execution, which was surprisingly hard

## v0.16.0 - 04.03.24 - Freddy
- made executable execution while specifying a path work
- fun fact you can now run `./crash` and it will execute itself

### v0.14.1 - 01.03.24 - Niklas
- Concurrent processes in pipelines implemented, exit code handling added (not tested)
- `> >>` redirections working, `<` redirection buggy but basically working

## v0.14.0 - 28.02.24 - Niklas
- Changed structure of forking and execution of commands & builtins, working up to some extent, not finished
- Fixed exit to only exit when not in pipes, also to exit with the correct status code

### v0.15.1 - 28.02.24 - Freddy
- fixed some small weird errors in lexing and validating

## v0.15.0 - 27.02.24 - Freddy
- implemented readline (-> history, file completion, arrow key through inputs)
- move program loop stuff into own folder

## v0.14.0 - 27.02.24 - Freddy
- oldpwd & pwd specific precise bash replication if either or both are missing
- removed unnecessary print statements form parser

### v0.13.4 - 27.02.24 - Niklas
- Corrected pipe behavior, so it works in main loop

### v0.13.3 - 26.02.24 - Niklas
- Pipes working properly with pipelines, fd's are parsed through struct nodes


### v0.13.2 - 26.02.24 - Freddy
- duplicate brackets for non-builtins fix (previously ignored brackets would be counted as args)

### v0.13.1 - 26.02.24 - Freddy
- fixed parser being weird with multiple nested brackets
- dumb double execution executer fix
- Norminettification of printing.c
- renamed expander and executer to fit present progressive naming scheme

## v0.13.0 - 26.02.24 - Freddy
- made executer ignore stuff flagged with ignore
- removed unnecessary functions from cd.c
- logical AND & OR fixes
- added more logging based on verbose flag to input_loop.c
- parser ignore flag setting when there arent even brackets fix

## v.0.12.1 - 26.02.24 - Niklas
- Fixed Lexer issue interpreting arguments as builtins

## v0.12.0 - 26.02.24 - Freddy
- Added logical AND & OR
- changelog dates fix

### v0.11.2 - 26.02.24 - Freddy
- Fixed export trying to set an already set variable without a value

### v0.11.1 - 26.02.24 - Freddy
- Fixed cd not returning to home directory when run with no args

## v0.11.0 - 22.02.24 - Freddy
- Added basic command execution based on pathfinding of right executables, with arguments and everything
	- Added execute_command_utils and a new command struct that it populates.
- Norminettification (fixed all norminette error except WRONG_SCOPE_COMMENT)
	- general files
		- moved logo printing into own function; created logo.txt
	- 4-executer in full
	- 3-parsing in full
	- headers
- removed deprecated folder

## v0.10.0 - 21.02.24 - Niklas
- Added expansion of * in the cwd and $ after the lexer
- Deleted expansion and wildcard token types
- Minor bug fixes

## v0.9.0 - 21.02.24 - Freddy
- Added more builtins (all currently with only flimsy error handling)
	- pwd
	- cd
	- export
	- unset
- created envcp_utils for use in cd, export, & unset (get, create, set, delete an env parameter)
- added new builtin_errors.c file for builtin error handling
- added ft_strjoinfree which not only joins strings but frees originals
- heap use after free error in input_loop fixed, missed one creategarbagecollector call in l23.

## v0.8.0 - 20.02.24 - Freddy
- Added builtins
	- env
	- exit
- added exit status to program data

## v0.7.0 - 20.02.24 - Freddy
- created a basic program execution loop with lexer, validator, parser, and executer, currently missing expander & executer is very basic
- created basic executer, does not consider operators and can only execute one builtin, echo.
- Removed unnecessary compile flags

## v0.6.0 - 19.02.24 - Freddy
- Added garbage collector, currently unimplemented
- removed dysfunctional execution functions for merging with master
- added input loop which takes input and currently just throws it into the lexer & tree building testing function
- Added .DS_Store to gitignore

### v0.5.1 - 15.02.24 - Freddy
- fixed parser segfaulting when two operators are next to each other or an operator is at start of line

## v0.5.0 - 14.02.24 - Niklas
- Added `ms_split` function that splits input into tokens, taking care of quotes

## v0.4.0 - 09.02.24 - Freddy
- Added correct bracket binary tree creation functionality in `parser`
- Added opening & closing brace operators for use in parser, implemented in `lexer`
	- Changed testing input to test for braces
	- added to `printing.c`
- made `parser` and `parser_util` norminette conforming
- renamed executable to crash, the name of our shell & added to `.gitignore`
- Added ignored char to token struct
- simplified names of some functions for norminette conformity

### v0.3.1 - 08.02.24 - Freddy
- moved `lexer.c`
- removed ; semicolon recognition as unique token from `lexer` and `minishell.h` and `printing.c`

## v0.3.0 - 08.02.24 - Freddy
- fixed `parser.c` array assingment & infinite loop problem
    - fixed get_dominant_operator not iterating
    - fixed get_sub_token_arr incorrect malloc size calculation
- moved print_token as a seperate function out of print_tokens in `printing.c`
- added print_binary_tree() to `printing.c`
- fixed broken import in `testing.c` & added print binary tree call
- apparently Makefile spelling was not properly corrected, should be good now

## v0.2.0 - 08.02.24 - Freddy
- added `parser.c` & `parser_util.c` which create basic bin tree from token array
	- untested because theres not a printing function yet
- changed t_bin_tree_node to save a token array as value instead of a single token

## v0.1.0 - 07.02.24 - Freddy
- added `printing.c` for printing debug functions (with print_tokens())
- added `testing.c` for testing debug functions (with test_lexer())
- added non-finished `lexer.c` that splits up input string into token array
- added deprecated folder with alternative token space seperator approach for unused but good code of potential future use

### v0.0.2 - 07.02.24 - Freddy
- Makefile spelling fix & executable placement error
	- added normcomp target to allow for comp style execution without norm checks

### v0.0.1 - 30.01.24 - Freddy
- readme line adjustments

## v0.0.0 - 30.01.24 - Freddy
- Basic Project Setup
