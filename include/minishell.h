/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:30:11 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/07 09:19:40 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ----- SUBMODULES

#include "../submodules/42_ft_printf/ft_printf.h"
#include "../submodules/42_libft/libft.h"
#include "../submodules/42_get_next_line/get_next_line.h"

// ----- TOKEN TYPES

#define TOK_CMD_ARG 0    // Commands or Arguments
#define TOK_S_QUOTE 1    // Single Quotes (')
#define TOK_D_QUOTE 2    // Double Quotes (")
#define TOK_VAR_EXP 3    // Dollar Sign ($) for variable expansion
#define TOK_EXIT_STAT 4  // Exit Status ($?)
#define TOK_REDIR 5      // Redirections (<, >, >>, <<)
#define TOK_PIPE 6       // Pipe (|)
#define TOK_LOG_OP 7     // Logical Operators (&&, ||)
#define TOK_WILDCARD 8   // Wildcard (*)
#define TOK_BUILTIN 9    // Builtins (echo, cd, pwd, export, unset, env, exit)
#define TOK_CTRL_SEQ 10  // Control Sequences (Ctrl-C, Ctrl-D, Ctrl-Z)
#define TOK_SEMICOLON 11 // Semicolon (;)

// ----- STRUCTS

typedef struct s_token
{
	char	type;
	char	*value;
}				t_token;
// char used for type to save space, as there are only 12 types

typedef struct s_bin_tree_node	t_bin_tree_node;
typedef struct s_bin_tree_node {
	t_token				*val;
	t_bin_tree_node		*left;
	t_bin_tree_node		*right;
}				t_bin_tree_node;
// maybe make val an array e.g. for flags

// ----- SETTINGS

// will output detailed logging if set to 1, and normal logging if 0
#define VERBOSE 0

// ----- FUNCTIONS

// utils / debug
void	print_tokens(t_token **tokens);
void	test_lexer();

// lexer
t_token	**lexer(char *input);