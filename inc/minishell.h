/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:22:10 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/04 17:32:51 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define PROMPT "minishell$ "
# define HDOC "> "

typedef struct s_sig	t_g_sig;
typedef struct s_shell	t_shell;

typedef enum e_bool
{
	FALSE,
	TRUE
}						t_bool;

typedef enum e_tok_type
{
	ARGS = 0,
	PIPE,
	INFILE,
	OUTFILE,
	APPEND,
	HEREDOC,
	NONE
}						t_tok_type;

typedef struct s_token
{
	char				*value;
	t_tok_type			type;
	struct s_token		*next;
}						t_token;

typedef struct s_hdoc
{
	char				*del;
	t_bool				expanded;
	struct s_hdoc		*next;
}						t_hdoc;

typedef struct s_env
{
	char				*key;
	char				*value;
	struct s_env		*next;
}						t_env;

typedef struct s_cmd
{
	char				**argv;
	int					argc;
	char				*input_file;
	char				*output_file;
	char				*error_file;
	int					flag_err;
	int					append;
	t_hdoc				*hdoc;
	struct s_cmd		*next;
}						t_cmd;

struct					s_sig
{
	int					is_runing;
	int					exit_status;
	int					pipfd[2];
	int					*hdoc_fd;
	int					use_pipe;
	t_shell				*shell;
};

struct					s_shell
{
	t_env				*envp;
	t_cmd				*cmd;
};

void					ft_free(void *ptr);
char					*ft_getcwd(char *s);

// ENV
char					*ft_get_env_value(t_env *envp, char *key);
void					print_env(t_env *envp);
int						ft_add_env(t_env **envp, char *arg);
t_env					*ft_get_last_env(t_env *envp);
t_shell					init_shell(char **envp);
char					*ft_strndup(char *str, size_t n);
int						ft_strcmp(char *s1, char *s2);
char					**convert_env_to_array(t_env *envp);
void					ft_unset_env(t_env **envp, char *key);
void					ft_swap_env(t_env *i, t_env *j, t_env *tmp);
int						ft_search_equ(char *s);
t_env					*add_new_env(char *key, char *value);

// PARSING
t_cmd					*parsing(t_shell *shell, char *input);
char					*ft_expand_for_hdoc(t_shell *shell, char *s);

// PARSE UTILS
int						count_quotes(char *input);

int						ft_isspace(char c);
int						ft_is_between(char *str, int index);
int						ft_skip_space(char *str);
int						ft_is_quote(char c);
int						ft_is_belong(char c);
char					*ft_strtok_quoted(char *str);
t_tok_type				assign_type(char *s);
t_token					*lexer_input(char *input);
int						ft_strlen_skip_quote(char *s);
char					*ft_get_arg(t_shell *shell, char *tok);
t_cmd					*parse_into_cmd(t_shell *shell, t_token *tok);

// expand;
char					*extract_var(char *s, t_shell *shell);
char					*ft_expand(t_shell *shell, char *s);
char					*add_double_quotes(char *s);
char					*ft_expand_for_hdoc(t_shell *shell, char *s);
int						ft_strlen_expand(char *s);
int						ft_is_after_here(char *s, int i);
int						ft_is_after_equal(char *s, int i);
void					format_value_for_quotes(char *s, int *i, int *j,
							char **value);
void					ft_is_quote_after_equal(char *s, int *i, int *j);
char					*format_value(char *s);
char					*extract_var(char *s, t_shell *shell);
t_bool					ft_is_special(char c);
t_bool					ft_is_expanded(char *s, int i);

// ft_free
void					ft_free_token(t_token *tok);
void					ft_free_cmd(t_cmd **cmd);
void					ft_free_env(t_env **envp);
void					ft_free_all(t_shell *shell);
void					ft_free_arr(char **arr);
void					ft_free_pipe(int pipefd[2]);

// extra_libft
void					*ft_realloc(void *ptr, size_t old_size,
							size_t new_size);
char					*ft_strjoin3(char *s1, char *s2, char *s3);
char					*ft_strjoin_free(char *s1, char *s2);
char					*ft_strjoin_s1(char *s1, char *s2);
int						ft_cmdsize(t_cmd *cmd);
int						is_only_dot_or_slash(char *s);

// EXEC
int						launch_cmd(t_shell *shell, t_cmd *cmd, int sa_std[2]);
int						config_with_pipe(t_shell *shell, t_cmd *cmd,
							int sa_stds[2]);
void					what_cmd_without_pipe(t_shell *shell, t_cmd *cmd,
							int stdin, int stdout);
void					what_cmd_with_pipe(t_shell *shell, t_cmd *cmd,
							int stdin, int stdout);
int						handler_input_redirection(char *input_file);
int						handler_output_redirection(t_cmd *cmd);
int						handle_heredoc(t_cmd *cmd, t_shell *shell,
							int std_fds[2]);
int						handle_heredoc_with_pipe(t_cmd *cmd, t_shell *shell,
							int std_fds[2]);
int						handle_dot_cmd(t_shell *shell, t_cmd *cmd);
void					handle_ctrl_c(char *content, int pipe_fd[2],
							int std_fds[2], pid_t pid);
void					close_unused_hdoc_fd(int hdoc_fd);
int						other_cmd_without_pipe(t_shell *shell, t_cmd *cmd,
							int stdin, int stdout);
int						other_cmd_with_pipe(t_shell *shell, t_cmd *cmd);
t_bool					handler_error_flag(t_cmd *cmd, int *input_fd,
							int *output_fd);
char					*resolve_cmd_path(t_shell *shell, char *cmd);
int						is_special_case(t_cmd *cmd, t_shell *shell);
char					*expand_content_if_needed(char *content, t_hdoc *hdoc);
void					write_content_to_pipe(char *content, int pipe_fd[2]);
int						init_global_hdoc_fd(t_cmd *cmd);
int						handle_single_heredoc(t_cmd *tmp, t_shell *shell,
							int std_fds[2], int i);
int						handle_relatif_cmd(t_shell *shell, t_cmd *cmd,
							char **envp);
int						handle_absolute_cmd(t_cmd *cmd, char **envp);
int						check_execution(char **envp, char *cmd_path,
							t_cmd *cmd);
t_bool					retore_fds_standart(int input_fd, int output_fd,
							int stdin, int stdout);

// BUILTINS
int						ft_echo(char **args);
int						ft_pwd(void);
int						ft_export(t_shell *shell, t_cmd *cmd);
int						ft_env(t_shell *shell, t_cmd *cmd);
int						ft_unset(t_shell *shell, t_cmd *cmd);
int						ft_exit(t_shell *shell, char **argv, int stdin,
							int stdout);
int						ft_cd(t_cmd *cmd, t_env *envp);

// SIGNALS
int						handler_signal_pipe(pid_t pid);
void					handler_signal_fork(pid_t pid);
int						handler_signal_hdoc(int *pipe_fd, pid_t pid, t_cmd *cmd,
							int std_fds[2]);
void					handle_sigint(int sig);
void					handle_sigint_for_hdoc(int sig);
t_g_sig					*g_global(void);
void					setup_signal(void);
void					setup_signal_for_hdoc(void);
void					close_hdoc_fd_inherited_from_parent(void);

void					print_err(char *s1, char *s2, char *s3, int fd);

void					save_fds_standart(int *saved_stdin, int *saved_stdout);

#endif