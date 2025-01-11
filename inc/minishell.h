/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:22:10 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/11 14:55:42 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <limits.h>
# include <errno.h>
# include <fcntl.h>
# include <stdio.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdlib.h>
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define PROMPT "minishell$ "

typedef enum e_tok_type
{
	ARGS= 0,
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
	bool			expanded;		// Mode texte brute
}					t_hdoc;

typedef struct s_cmd
{
	char **argv;
	char			*input_file;
	char			*output_file;
	t_hdoc			here_doc;
	bool			append;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_shell
{
	t_list			*envp;
	t_cmd			*cmd;
	int				exit_status;
}					t_shell;

t_shell				init_shell(char **envp);
t_tok_type			assign_type(char *s);
t_token			*lexer_input(char *input);
t_cmd			*parse_input(t_token *tok);
int		ft_cat(char *filename);
void	what_cmd(t_shell *shell, t_cmd *cmd);
void	ft_getcwd(char *buf, size_t size);
void	panic(char *s);
int		ft_pwd(void);
int		ft_echo(char **args);
int		count_quotes(char *input);
int		ft_isspace(char c);
int		ft_strcmp(char *s1, char *s2);
int		ft_is_between(char *str, int index);
int		ft_skip_space(char *str);
int		ft_is_quote(char c);
int		ft_is_belong(char c);
bool	is_valid_cmd(char *cmd);
bool    handle_heredoc(t_cmd *cmd);
bool    launch_cmd(t_shell *shell, t_cmd *cmd);
bool	config_with_pipe(t_shell *shell, t_cmd *cmd);
char	*ft_strndup(char *str, size_t n);
char	*ft_strtok_quoted(char *str);
char    *read_fd(int fd);

#endif