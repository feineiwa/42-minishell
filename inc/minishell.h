/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:22:10 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/07 17:02:38 by frahenin         ###   ########.fr       */
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
	t_bool			interp;
	char			*var;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_shell
{
	t_env			*envp;
	t_cmd			*cmd;
	int				exit_status;
}					t_shell;

// ENV
char				*ft_get_env_value(t_env *envp, char *key);
void				print_env(t_env *envp);
void				ft_add_env(t_env **envp, char *arg);
t_env				*ft_get_last_env(t_env *envp);

t_shell				init_shell(char **envp);
char				*ft_strndup(char *str, size_t n);
int					ft_strcmp(char *s1, char *s2);

#endif