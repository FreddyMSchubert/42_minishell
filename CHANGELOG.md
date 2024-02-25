# CHANGELOG minishell by nburchha & fschuber

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