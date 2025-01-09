/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:22:10 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/09 18:45:36 by frahenin         ###   ########.fr       */
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

# define PROMPT "minishell$ "

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
	HEREDOC
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
	char			*content;
}					t_hdoc;

typedef struct s_env
{
	char			*key;
	char			*value;
	struct s_env	*next;
}					t_env;

typedef struct s_cmd
{
	char **argv; // array of arguments
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
	int				pipefd[2];
}					t_shell;

int					ft_free(void *ptr);
// ENV
char				*ft_get_env_value(t_env *envp, char *key);
void				print_env(t_env *envp);
void				ft_add_env(t_env **envp, char *arg);
t_env				*ft_get_last_env(t_env *envp);
void				ft_free_env(t_env **envp);
t_shell				init_shell(char **envp);
char				*ft_strndup(char *str, size_t n);
int					ft_strcmp(char *s1, char *s2);

// PARSE

void				panic(char *s);
int					ft_fork(void);
int					count_quotes(char *input);
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
t_cmd				*parsing(t_shell *shell, char *input);

// expand;
char				*extract_var(char *s, t_shell *shell);
char				*ft_expand(t_shell *shell, char *s);
#endif