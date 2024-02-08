# CHANGELOG minishell by nburchha & fschuber

## v0.3.0 - 08.02.24
- fixed parser.c array assingment & infinite loop problem
    - fixed get_dominant_operator not iterating
    - fixed get_sub_token_arr incorrect malloc size calculation
- moved print_token as a seperate function out of print_tokens in printing.c
- added print_binary_tree() to printing.c
- fixed broken import in testing.c & added print binary tree call
- apparently Makefile spelling was not properly corrected, should be good now

## v0.2.0 - 08.02.24
- added `parser.c` & `parser_util.c` which create basic bin tree from token array
	- untested because theres not a printing function yet
- changed t_bin_tree_node to save a token array as value instead of a single token

## v0.1.0 - 07.02.24
- added `printing.c` for printing debug functions (with print_tokens())
- added `testing.c` for testing debug functions (with test_lexer())
- added non-finished `lexer.c` that splits up input string into token array
- added deprecated folder with alternative token space seperator approach for unused but good code of potential future use

### v0.0.2 - 07.02.24
- Makefile spelling fix & executable placement error
	- added normcomp target to allow for comp style execution without norm checks

### v0.0.1 - 30.01.24
- readme line adjustments

## v0.0.0 - 30.01.24
- Basic Project Setup
