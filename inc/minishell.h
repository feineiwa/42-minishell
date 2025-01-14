/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:22:10 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/14 18:09:25 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define PROMPT "\033[36mminishell$\033[0m "

typedef enum e_bool
{
	FALSE,
	TRUE
}					t_bool;

typedef enum e_tok_type
{
	ARGS = 0,
	PIPE,
	INFILE,
	OUTFILE,
	APPEND,
	HEREDOC,
	NONE
}					t_tok_type;

typedef struct s_token
{
	char			*value;
	t_tok_type		type;
	struct s_token	*next;
}					t_token;

typedef struct s_hdoc
{
	char			*del;
	t_bool			expanded;
	int				pipefd[2];
}					t_hdoc;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_cmd
{
	char			**argv;
	int				argc;
	char			*input_file;
	char			*output_file;
	int				append;
	t_hdoc			*hdoc;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_shell
{
	t_env			*envp;
	t_cmd			*cmd;
	int				exit_status;
}					t_shell;

void				ft_free(void *ptr);
// ENV
char				*ft_get_env_value(t_env *envp, char *key);
void				print_env(t_env *envp);
void				ft_add_env(t_env **envp, char *arg);
t_env				*ft_get_last_env(t_env *envp);
void				ft_free_env(t_env **envp);
t_shell				init_shell(char **envp);
char				*ft_strndup(char *str, size_t n);
int					ft_strcmp(char *s1, char *s2);
char				**convert_env_to_array(t_env *envp);
void				ft_unset_env(t_env **envp, char *key);
int					ft_search_equ(char *s);
// PARSING
t_cmd				*parsing(t_shell *shell, char *input);

// PARSE UTILS
int					count_quotes(char *input);

void				panic(char *s);
int					ft_fork(void);
int					ft_isspace(char c);
int					ft_is_between(char *str, int index);
int					ft_skip_space(char *str);
int					ft_is_quote(char c);
int					ft_is_belong(char c);
char				*ft_strtok_quoted(char *str);
t_tok_type			assign_type(char *s);
t_token				*lexer_input(char *input);
int					ft_strlen_skip_quote(char *s);
char				*ft_get_arg(t_shell *shell, char *tok);
t_cmd				*parse_into_cmd(t_shell *shell, t_token *tok);

// expand;
char				*extract_var(char *s, t_shell *shell);
char				*ft_expand(t_shell *shell, char *s);

// ft_free
void				ft_free_token(t_token *tok);
void				ft_free_cmd(t_cmd **cmd);

// extra_libft
void				*ft_realloc(void *ptr, size_t old_size, size_t new_size);
char				*ft_strjoin3(char *s1, char *s2, char *s3);
char				*ft_strjoin_free(char *s1, char *s2);

// EXEC
t_bool				is_valid_cmd(char *cmd);
t_bool				launch_cmd(t_shell *shell, t_cmd *cmd);
t_bool				config_with_pipe(t_shell *shell, t_cmd *cmd);
t_bool				handle_heredoc(t_cmd *cmd);

// BUILTINS
int					ft_cat(char *filename);
int					ft_echo(char **args);
int					ft_pwd(void);
int					ft_export(t_shell *shell, t_cmd *cmd);
int					ft_env(t_shell *shell, t_cmd *cmd);
int					ft_unset(t_shell *shell, t_cmd *cmd);
#endif