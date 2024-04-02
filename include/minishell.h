/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:30:11 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/02 12:28:37 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ------ INCLUDES

#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <dirent.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
// readline
#include <stdio.h>
#include <readline/readline.h>
#include <readline/history.h>
// signals
#include <signal.h>
#include <termios.h>
#include <sys/ioctl.h>
// waitpid
#include <sys/wait.h>

#include "./colors.h"

// ----- SUBMODULES

#include "../submodules/42_ft_printf/ft_printf.h"
#include "../submodules/42_libft/libft.h"

// ----- TOKEN TYPES

#define TOK_WORD 0			// Commands or Arguments
#define TOK_BUILTIN 1		// Builtin (echo, cd, pwd, export, unset, env, exit)
#define TOK_REDIR 2			// Redirections (<, >, >>, <<)
#define TOK_PIPE 3			// Pipe (|)
#define TOK_LOG_OR 4		// Logical OR ||
#define TOK_LOG_AND 5		// Logical AND &&
#define TOK_OPEN_BRACE 6	// Open Brace -> (
#define TOK_CLOSE_BRACE 7	// Close Brace -> )

// ----- STRUCTS

typedef struct s_token
{
	char			type;
	char			*value;
}	t_token;
// char used for type to save space, as there are only 8 types

typedef struct s_bin_tree_node	t_bin_tree_node;
typedef struct s_bin_tree_node
{
	t_token			**val;
	t_bin_tree_node	*l;
	t_bin_tree_node	*r;
	t_bin_tree_node	*parent;
	int				input_fd;
	int				output_fd;
}	t_bin_tree_node;

typedef struct s_program_data
{
	char			exit_flag;		// 0 by default, 1 to exit & cleanup
	int				exit_status;	// exit status to be returned
	char			**envcp;		// internal copy of envp
	t_list			*gc;			// garbage collector
}	t_program_data;

typedef struct s_cmd_path
{
	char			*path;
	char			**args;
}				t_cmd_path;

// ----- SETTINGS

// will output detailed logging if set to 1, and normal logging if 0
#define VERBOSE 0
// will make everything work with tester provided executable is called minishell
#define DEBUG 1

// ----- FUNCTIONS

// --- utils / debug
// printing
void				print_tokens(t_list *tokens);
void				print_token(t_token *token);
void				print_binary_tree(t_bin_tree_node *tree, int tabs);
void				print_pipes(int in_fd, int out_fd);
t_token				**list_to_token_array(t_list *list);
// testing
void				test_validator(void);
void				test_lexer(char *input, t_program_data *data);
void				test_expander(t_program_data *data, t_token **tokens);

// --- input loop
int					run_crash_interface(t_program_data *program_data);
// - garbage collector
t_list				*create_garbage_collector(void);
int					gc_append_element(t_list *gc, void *content);
void				gc_append_element_array(t_list *gc, void *content);
void				gc_append_t_list(t_list *gc, t_list *linkedlist);
void				gc_cleanup(t_list *gc);
// - util
void				exit_error(char *message, int exit_code, t_list *gc);
// - signals
int					setup_signals(void);
// - visuals
void				*print_logo(void);

// --- 0-lexing
t_list				*lexer(char *input, t_program_data *data);
char				**ms_split(char *input);
int					count_tokens(const char *s);
int					is_operator_symbol(char c, char d);
int					is_redirect(char c);
int					same_str(char *str1, char *str2);
char				*get_rid_of_quotes(char *str, t_program_data *program_data);

// --- 1-validation
int					validator(t_list *tokens);
int					check_files(t_list *files, int flag);

// --- 2-expanding
char				*get_envcp(char *var_name, t_program_data *program_data);
t_list				*expander(t_list *tokens, t_program_data *program_data);
char				*list_matching_files(const char *pattern);
// --- 3-parsing
// util
t_token				**switch_args_for_redir(t_token **arr);
t_list				*sub_token_t_list(t_list *tokens, int start, int end, t_program_data *program_data);
int					toklen(t_list *tokens);
t_token				**t_list_to_token_arr(t_list	*tokens, t_program_data	*program_data);
t_token				*get_token_at_index(t_list *tokens, int index);
// parser
t_bin_tree_node		*tok_to_bin_tree(t_list *tokens, t_program_data *program_data);

// --- 4-executing
// general
int					execute(t_bin_tree_node *tree, t_program_data *data);
int					execute_node(t_bin_tree_node *node, t_program_data *data);
int					execute_input(t_program_data *program_data, char *input);
void				child_process_exit(t_program_data	*program_data, int	exitcode);
// operators
// int					logical_op(t_bin_tree_node *node, t_program_data *data);
int					logical_and(t_bin_tree_node *node, t_program_data *data);
int					logical_or(t_bin_tree_node *node, t_program_data *data);
int					redirect(t_bin_tree_node *node, t_program_data *program_data);
void				setup_pipe(t_bin_tree_node *node, t_program_data *program_data);
// "normal" commands
t_cmd_path			*create_cmd_struct(char	**envp, t_token	**cmd, int cmd_start_index);
// builtins
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
// errors
void				broadcast_validator_error(int error, char *arg);
void				builtin_err(char *builtin, int error, char *arg);
void				exec_error(int error);
// general
char				*ft_strjoinfree(char *s1, char *s2);