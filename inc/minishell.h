/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:22:10 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/23 16:50:02 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define PROMPT "\033[36mminishell$\033[0m "
// # define PROMPT "minishell$ "
# define HDOC "\033[1;33m>\033[0m "

typedef struct g_sig	t_g_sig;

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

struct					g_sig
{
	int					is_runing;
	int					exit_status;
};

typedef struct s_shell
{
	t_env				*envp;
	t_cmd				*cmd;
	int					exit_status;
}						t_shell;

void					ft_free(void *ptr);

// ENV
char					*ft_get_env_value(t_env *envp, char *key);
void					print_env(t_env *envp);
void					ft_add_env(t_env **envp, char *arg);
t_env					*ft_get_last_env(t_env *envp);
t_shell					init_shell(char **envp);
char					*ft_strndup(char *str, size_t n);
int						ft_strcmp(char *s1, char *s2);
char					**convert_env_to_array(t_env *envp);
void					ft_unset_env(t_env **envp, char *key);
int						ft_search_equ(char *s);

// PARSING
t_cmd					*parsing(t_shell *shell, char *input);
char					*ft_expand_for_hdoc(t_shell *shell, char *s);

// PARSE UTILS
int						count_quotes(char *input);

int						ft_fork(void);
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
char					*ft_strjoin3_free(char *s1, char *s2, char *s3);

// ft_free
void					ft_free_token(t_token *tok);
void					ft_free_cmd(t_cmd **cmd);
void					ft_free_env(t_env **envp);
void					ft_free_all(t_shell *shell);
void					ft_free_arr(char **arr);

// extra_libft
void					*ft_realloc(void *ptr, size_t old_size,
							size_t new_size);
char					*ft_strjoin3(char *s1, char *s2, char *s3);
char					*ft_strjoin_free(char *s1, char *s2);
char					*ft_strjoin_s1(char *s1, char *s2);
int						ft_cmdsize(t_cmd *cmd);

// EXEC
t_bool					launch_cmd_without_pipe(t_shell *shell, t_cmd *cmd);
int						launch_cmd_with_pipe(t_shell *shell, t_cmd *cmd);
t_bool					config_with_pipe(t_shell *shell, t_cmd *cmd);
void					what_cmd(t_shell *shell, t_cmd *cmd, int stdin,
							int stdout);
int						handle_heredoc(t_cmd *cmd, t_shell *shell);
int						*handle_heredoc_with_pipe(t_cmd *cmd, t_shell *shell);
int						other_cmd_with_pipe(t_shell *shell, t_cmd *cmd);
int						other_cmd_without_pipe(t_shell *shell, t_cmd *cmd);
char					*resolve_cmd_path(t_shell *shell, t_cmd *cmd);

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
t_g_sig					*g_global(void);
void					setup_signal(void);

#endif