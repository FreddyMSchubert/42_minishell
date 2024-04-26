/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fschuber <fschuber@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:30:11 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/26 07:58:54 by fschuber         ###   ########.fr       */
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

#include "../submodules/42_libft/libft.h"
#include "../submodules/42_get_next_line/get_next_line.h"

// ----- SETTINGS

// will output detailed logging if set to 1, and normal logging if 0
#define VERBOSE 0
// will make everything work with tester provided executable is called minishell
#define DEBUG 1

// ----- TOKEN TYPES

#define TOK_WORD 0			// Commands or Arguments
#define TOK_S_QUOTE 1		// Single Quote
#define TOK_D_QUOTE 2		// Double Quote
#define TOK_BUILTIN 3		// Builtin (echo, cd, pwd, export, unset, env, exit)
#define TOK_REDIR 4			// Redirections (<, >, >>, <<)
#define TOK_PIPE 5			// Pipe (|)
#define TOK_LOG_OR 6		// Logical OR ||
#define TOK_LOG_AND 7		// Logical AND &&
#define TOK_OPEN_BRACE 8	// Open Brace -> (
#define TOK_CLOSE_BRACE 9	// Close Brace -> )

// ----- GLOBALS

extern int	g_sigint_received;

// ----- STRUCTS

typedef struct s_tok
{
	char			type;
	char			*val;
}	t_tok;
// char used for type to save space, as there are only 8 types

typedef struct s_node	t_node;
typedef struct s_node
{
	t_tok			**val;
	t_node          *l;
	t_node          *r;
	t_node          *parent;
	bool			redirected;
	int				in_fd;
	int				out_fd;
}	t_node;
// redirected is true if node is a redirection and was already executed

typedef struct s_pid_list
{
	pid_t				pid;
	bool				is_builtin;
	struct s_pid_list	*next;
}	t_pid_list;
//if last element of list is a builtin we use the existing programdata exit code

typedef struct s_data
{
	bool			exit_flag;
	int				exit_status;
	char			**envcp;
	t_list			*gc;
	t_pid_list		*pid_list;
}	t_data;

typedef struct s_cmd_path
{
	char			*path;
	char			**args;
}				t_cmd_path;


// ----- FUNCTIONS

// --- input loop
int					run_input_loop(t_data *program_data);
// - garbage collector
t_list				*gc_create(void);
int					gc_append_element(t_list *gc, void *content);
void				gc_cleanup(t_list *gc);
// - util
void				exit_error(char *message, int exit_code, t_list *gc);
// - signals
void				setup_sigs(void);
// - visuals
void				*print_logo(void);

// --- 0-lex
t_list				*lex(char *input, t_data *data);
char				**ms_split(char *input, t_data *data);
int					count_tokens(const char *s);
int					is_operator_symbol(char c, char d);
bool				is_redirect(char c);
int					same_str(char *str1, char *str2);
char				*get_rid_of_quotes(char *str);
t_list				*split_token_if_operator_in_quotes(t_tok **token, t_data *data);

// --- 1-validate
int					validate(t_list *tokens);
int					check_files(t_list *files, int flag);

// --- 2-expander
char				*expand(char *str, t_data *program_data, bool heredoc);
char				*get_envcp(char *var_name, t_data *program_data);
char				*isolate_var(char *var);
int					find_closing_quote(char *str, int *i);
char				*quote_operators(char *envcp_value);
bool				is_valid_variable(char *var);
// wildcard
char				*list_matching_files(char *pattern);
char				*get_pattern(char *str, int index, t_data *program_data);
char				*get_rid_of_quotes_wildcard(char *str);

// --- 3-parser
// parser
t_node		*parse(t_list *tokens, t_data *program_data);
// util
t_list				*sub_token_t_list(t_list *tokens, int start, int end, t_data *program_data);
t_list				*switch_redir_args(t_list *tokens);
int					toklen(t_list *tokens);
t_tok				**t_list_to_token_arr(t_list	*tokens, t_data	*program_data);
t_tok				*get_token_at_index(t_list *tokens, int index);

// --- 4-executing
// general
pid_t				execute(t_node *tree, t_data *program_data, t_pid_list **pid_list);
int					execute_node(t_node *node, t_data *data, t_pid_list **pid_list);
void				child_process_exit(t_data	*data, int	exitcode);
// operators
int					logical_and(t_node *node, t_data *program_data);
int					logical_or(t_node *node, t_data *program_data);
int					redirect(t_node *node, t_data *data);
int					heredoc(t_node *node, t_data	*program_data);
void				setup_pipe(t_node *node);
char				*get_filename(t_node *node);
// "normal" commands
t_cmd_path			*create_cmd_struct(char	**envp, t_tok	**cmd);
// builtins
int					execute_echo(t_tok **inputs, int out_fd, t_data *program_data);
int					execute_env(t_data *program_data, int out_fd);
int					execute_exit(t_tok **tokens, t_data *data, \
								int out_fd);
int					execute_cd(t_tok **tokens, t_data *program_data);
int					execute_pwd(int out_fd, t_data *program_data);
int					execute_export(t_tok **node, int out_fd, t_data *data);
int					execute_unset(t_tok **node, t_data *data);
// env utils
char				*get_envcp_var(char *var, char **envcp);
int					set_envcp_var(char *var, char *value, char createnew, \
									t_data *program_data);
int					create_envcp_var(char *vr, char *vl, t_data *data);
int					delete_envcp_var(char *var, char **envcp);
// fds closing utils
void				close_fds_loop(void);
int					add_to_pid_list(pid_t pid, t_pid_list **pidlist, bool is_builtin);
void				*wait_and_free(t_data *program_data, t_pid_list **pid_list);

// --- util
bool				is_in_quote(char *str, char quote, char *current_char);
void				log_err(char *error, char *path1, char *path2);
char				*ft_strjoinfree(char *s1, char *s2);
// printing
void				print_tokens(t_list *tokens);
void				print_token(t_tok *token);
void				print_node_with_children(t_node *tree, int tabs);
