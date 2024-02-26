/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:30:11 by fschuber          #+#    #+#             */
/*   Updated: 2024/02/26 09:44:36 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ------ INCLUDES

#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>

#include "./colors.h"

// ----- SUBMODULES

#include "../submodules/42_ft_printf/ft_printf.h"
#include "../submodules/42_get_next_line/get_next_line.h"
#include "../submodules/42_libft/libft.h"

// ----- TOKEN TYPES

#define TOK_CMD_ARG 0      // Commands or Arguments
#define TOK_S_QUOTE 1      // Single Quotes (')
#define TOK_D_QUOTE 2      // Double Quotes (")
// #define TOK_VAR_EXP 3      // Dollar Sign ($) for variable expansion
#define TOK_EXIT_STAT 4    // Exit Status ($?)
#define TOK_REDIR 5        // Redirections (<, >, >>, <<)
#define TOK_PIPE 6         // Pipe (|)
#define TOK_LOG_OP 7       // Logical Operators (&&, ||)
// #define TOK_WILDCARD 8     // Wildcard (*)
#define TOK_BUILTIN 9      // Builtins (echo, cd, pwd, export, unset, env, exit)
#define TOK_CTRL_SEQ 10    // Control Sequences (Ctrl-C, Ctrl-D, Ctrl-Z)
#define TOK_OPEN_BRACE 11  // Open Brace -> (
#define TOK_CLOSE_BRACE 12 // Close Brace -> )

// ----- STRUCTS

typedef struct s_token {
	char			type;
	char			*value;
	char			ignored;
}	t_token;
// char used for type to save space, as there are only 12 types
// ignored is 0 if not, 1 if ignored

typedef struct s_bin_tree_node	t_bin_tree_node;
typedef struct s_bin_tree_node {
	t_token			**val;
	t_bin_tree_node	*l;
	t_bin_tree_node	*r;
}	t_bin_tree_node;

typedef struct s_program_data {
	char			exit_flag;		// 0 by default, 1 to exit & cleanup
	int				exit_status;	// exit status to be returned
	char			**envcp;		// internal copy of envp
	t_list			*gc;			// garbage collector
}	t_program_data;

typedef struct s_cmd_path {
	char			*path;
	char			**args;
}				t_cmd_path;

// ----- SETTINGS

// will output detailed logging if set to 1, and normal logging if 0
#define VERBOSE 1

// ----- FUNCTIONS

// --- utils / debug
// printing
void				print_tokens(t_token **tokens);
void				print_token(t_token *token);
void				print_binary_tree(t_bin_tree_node *tree, int tabs);
// testing
void				test_validator(void);
void				test_lexer(char *input, t_program_data *data);
void				test_expander(t_program_data *data, t_token **tokens);

// --- input loop
int					run_crash_interface(t_program_data *program_data);

// --- 0-lexing
t_token				**lexer(char *input, t_program_data *data);
char				**ms_split(char *input);
int					count_tokens(const char *s);
int					is_operator_symbol(char c, char d);

// --- 1-validation
int					validator(t_token **token_arr);
int					check_files(t_list *files, int flag);

// --- 2-expanding
char				*get_envcp(char *var_name, t_program_data *program_data);
t_token				**expander(t_token **tokens, t_program_data *program_data);
char				*list_matching_files(const char *pattern);
// --- 3-parsing
// util
t_token				**sub_tok_arr(t_token **token_arr, int start, int end);
int					toklen(t_token **token_arr);
// parser
t_bin_tree_node		*tok_to_bin_tree(t_token **token_arr);
t_bin_tree_node		*tok_to_bin_tree(t_token **token_arr);

// --- 4-executing
// general
void				execute(t_bin_tree_node *tree, t_program_data *data);
int					execute_node(t_bin_tree_node *node, t_program_data *data);
// operators
int					logical_op(t_bin_tree_node *node, t_program_data *data);
int					logical_and(t_bin_tree_node *node, t_program_data *data);
int					logical_or(t_bin_tree_node *node, t_program_data *data);
// "normal" commands
int	execute_command(t_bin_tree_node *node, t_program_data *program_data, int cmd_start_index);
t_cmd_path	*create_cmd_struct(char	**envp, t_token	**cmd, int cmd_start_index);
// builtins
int	execute_builtin(t_bin_tree_node *node, t_program_data *program_data, int cmd_start_index);
int	execute_echo(t_token **inputs, int cmd_start_index);
int					execute_env(t_program_data *program_data);
int	execute_exit(t_token **tokens, t_program_data *program_data, int cmd_start_index);
int	execute_cd(t_token **tokens, t_program_data *program_data, int cmd_start_index);
int					execute_pwd(void);
int	execute_export(t_token **node, t_program_data *program_data, int cmd_start_index);
int	execute_unset(t_token **node, t_program_data *program_data, int cmd_start_index);
// env utils
char				*get_envcp_var(char *var, char **envcp);
int					set_envcp_var(char *var, char *value, char createnew, \
									t_program_data *program_data);
int					create_envcp_var(char *var, char *value, char **envcp, \
									t_program_data *program_data);
int					delete_envcp_var(char *var, char **envcp);

// --- util

char				*strjoin_null_compatible(char const *s1, char const *s2);
// garbage collector
t_list				*create_garbage_collector(void);
int					append_element(t_list *gc, void *content);
void				append_element_array(t_list *gc, void *content);
void				cleanup(t_list *gc);
void				exit_error(char *message, int exit_code, t_list *gc);
// errors
void				broadcast_validator_error(int error, char *arg);
void				broadcast_builtin_error(char *builtin, int error, char *arg);
// general
char				*ft_strjoinfree(char *s1, char *s2);