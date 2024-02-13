/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:30:11 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/13 11:29:23 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ------ INCLUDES

#include <stdbool.h>

// ----- SUBMODULES

#include "../submodules/42_ft_printf/ft_printf.h"
#include "../submodules/42_get_next_line/get_next_line.h"
#include "../submodules/42_libft/libft.h"

// ----- TOKEN TYPES

#define TOK_CMD_ARG 0    	// Commands or Arguments
#define TOK_S_QUOTE 1    	// Single Quotes (')
#define TOK_D_QUOTE 2    	// Double Quotes (")
#define TOK_VAR_EXP 3    	// Dollar Sign ($) for variable expansion
#define TOK_EXIT_STAT 4  	// Exit Status ($?)
#define TOK_REDIR 5      	// Redirections (<, >, >>, <<)
#define TOK_PIPE 6       	// Pipe (|)
#define TOK_LOG_OP 7     	// Logical Operators (&&, ||)
#define TOK_WILDCARD 8   	// Wildcard (*)
#define TOK_BUILTIN 9    	// Builtins (echo, cd, pwd, export, unset, env, exit)
#define TOK_CTRL_SEQ 10  	// Control Sequences (Ctrl-C, Ctrl-D, Ctrl-Z)
#define TOK_OPEN_BRACE 11	// Open Brace -> (
#define TOK_CLOSE_BRACE 12	// Close Brace -> )

// ----- STRUCTS

typedef struct s_token
{
	char type;
	char *value;
	char ignored;
}				t_token;
// char used for type to save space, as there are only 12 types
// ignored is 0 if not, 1 if ignored

typedef struct s_bin_tree_node t_bin_tree_node;
typedef struct s_bin_tree_node
{
	t_token **val;
	t_bin_tree_node *l;
	t_bin_tree_node *r;
}				t_bin_tree_node;
// maybe make val an array e.g. for flags

// ----- SETTINGS

// will output detailed logging if set to 1, and normal logging if 0
#define VERBOSE 0

// ----- FUNCTIONS

// --- utils / debug
// printing
void				print_tokens(t_token **tokens);
void                print_token(t_token *token);
void                print_binary_tree(t_bin_tree_node *tree, int tabs);
// testing
void				test_lexer();

// --- 0-lexing
t_token				**lexer(char *input);

// --- 3-parsing
// util
t_token				**sub_tok_arr(t_token **token_arr, int start, int end);
int					toklen(t_token **token_arr);
// parser
t_bin_tree_node		*tok_to_bin_tree(t_token **token_arr);

// --- 4-executing
// general
int					executer(t_bin_tree_node *tree);
// commands
int					execute_log_op(t_bin_tree_node *tree);
int					execute_redir(t_bin_tree_node *tree);
int					execute_pipe(t_bin_tree_node *tree);
// builtins
int					execute_echo(t_bin_tree_node *tree);