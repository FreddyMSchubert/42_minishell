/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nburchha <nburchha@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/30 11:30:11 by fschuber          #+#    #+#             */
/*   Updated: 2024/04/27 10:31:25 by fschuber         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

// ------ INCLUDES

#include <stdio.h>
#include <stdbool.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
// directory operations
#include <dirent.h>
// readline
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
// will not print logo if set to 1
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

typedef struct s_node
{
	t_tok			**val;
	struct s_node	*l;
	struct s_node	*r;
	struct s_node	*parent;
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
}	t_data;

typedef struct s_cmd_path
{
	char			*path;
	char			**args;
}				t_cmd_path;

typedef struct s_exp
{
	char			*buffer;
	const char		*str;
	int				buffer_size;
	int				buf_pos;
	int				i;
}	t_exp;

// ----- FUNCTIONS

// --- input loop
void				run_input_loop(t_data *sh);
char				*get_input(t_data *sh);
int					handle_empty_input(t_data *sh, char **input);
int					execute_expander(char **input, t_data *sh);
int					execute_lexer(char *input, t_data *sh, t_list \
					**tokenified_input);
int					execute_validator(t_list *tokenified_input, t_data *sh);
int					execute_parser(t_list *tokenified_input, t_data *sh, \
					t_node **tree);
int					execute_executor(t_node *tree, t_data *sh);

// - garbage collector
t_list				*gc_create(void);
int					gc_append_element(t_list *gc, void *content);
void				gc_cleanup(t_list *gc);
void				gc_clean_and_reinit(t_list **gc);
// - util
void				exit_error(char *message, int exit_code, t_list *gc);
// - signals
void				setup_sigs(void);
// - visuals
void				*print_logo(void);

// --- 0-lexer
t_list				*lexer(char *input, t_data *data);
char				*put_space_between_tokens(char *input, t_data *data);
char				**ms_split(char *input, t_data *data);
int					count_tokens(const char *s);
t_list				*detect_token_type(char *input, \
								int is_first_or_after_operator, t_data *data);
t_list				*split_token_if_operator_in_quotes(t_tok **tok, t_data *sh);
int					is_operator_symbol(char c, char d);
bool				is_redirect(char c);
int					same_str(char *str1, char *str2);
char				*get_rid_of_quotes(char *str);
t_list				*split_token_if_operator_in_quotes(t_tok **tok, t_data *sh);

// --- 1-validate
int					validate(t_list *tokens);
int					check_files(t_list *files, int flag);
int					check_token_errors(t_list *tok, t_tok *token);
int					check_brace_errors(t_list *tok, t_tok *token, \
										int *brace_opened);
int					check_first_token(t_list *tok);
int					check_last_token(t_tok *token);
int					check_braces(int brace_opened);
void				throw_syntax_error(char *error);

// --- 2-expander
char				*expand(const char *str, t_data *data, bool heredoc);
bool				should_expand(const char *str, int i, char exp_type);
void				append_to_buffer(t_exp *exp, char *to_append);
// env
void				handle_dollar_expansion(t_exp *exp, t_data *data);
void				handle_dollar_quote_expansion(t_exp *exp);
void				handle_dollar_question_expansion(t_exp *exp, t_data *data);
// tilde
void				handle_tilde_expansion(t_exp *exp, t_data *data);
// wildcard
void				handle_wildcard_expansion(t_exp *exp, t_data *data);
char				*get_pattern(const char *str, int index, t_data *sh);
// expand util
int					find_closing_quote(const char *str, int *i);
char				*get_envcp(const char *var_name, t_data *sh);
bool				is_valid_variable(const char *var);
char				*isolate_var(char *var);
char				*quote_operators(char *envcp_value);

// --- 3-parser
// parser
t_node				*parse(t_list *tokens, t_data *sh);
// util
t_list				*sub_list(t_list *tokens, int start, int end, t_data *data);
t_list				*switch_redir_args(t_list *tokens);
int					toklen(t_list *tokens);
t_tok				**t_list_to_token_arr(t_list	*tokens, t_data	*data);
t_tok				*get_token_at_index(t_list *tokens, int index);
t_list				*check_substring(t_list *curr);
// defaults
t_node				*create_default_node(t_data *sh);
t_tok				**create_default_token_arr(t_data *sh);

// --- 4-executing
// general
pid_t				execute(t_node *tree, t_data *sh, t_pid_list **pid_list);
int					execute_node(t_node *node, t_data *data, \
										t_pid_list **pid_list);
void				child_process_exit(t_data *data, int exitcode);
// operators
int					logical_and(t_node *node, t_data *sh, t_pid_list **pidlist);
int					logical_or(t_node *node, t_data *sh, t_pid_list **pidlist);
int					redirect(t_node *node, t_data *data);
int					heredoc(t_node *node, t_data	*sh);
void				setup_pipe(t_node *node);
char				*get_filename(t_node *node);
// "normal" commands
t_cmd_path			*create_cmd_struct(char	**envp, t_tok	**cmd);
// builtins
int					execute_echo(t_tok **inputs, int out_fd, t_data *sh);
int					execute_env(t_data *sh, int out_fd);
int					execute_exit(t_tok **tokens, t_data *data, \
								int out_fd);
int					execute_cd(t_tok **tokens, t_data *sh);
int					execute_pwd(int out_fd, t_data *sh);
int					execute_export(t_tok **node, int out_fd, t_data *data);
int					execute_unset(t_tok **node, t_data *data);
// env utils
char				*get_envcp_var(char *var, char **envcp);
int					set_envcp_var(char *var, char *value, char createnew, \
									t_data *sh);
int					create_envcp_var(char *vr, char *vl, t_data *data);
int					delete_envcp_var(char *var, char **envcp);
// - utils
void				close_fds(t_node *node);
// fds closing utils
void				close_fds_loop(void);
int					add_to_pid_list(pid_t pid, t_pid_list **pid_list, \
										bool is_builtin);
void				*resolve_pid_list(t_data *sh, t_pid_list **pid_list);

// --- util
bool				in_quote(const char *str, char quote, \
								const char *current_char);
void				log_err(char *error, char *path1, char *path2);
char				*ft_strjoinfree(char *s1, char *s2);
// printing
void				print_tokens(t_list *tokens);
void				print_token(t_tok *token);
void				print_node_with_children(t_node *tree, int tabs);
