/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:22:10 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/03 17:12:11 by nrasamim         ###   ########.fr       */
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
# include <stdbool.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

# define INPUT		1	//"<"
# define HEREDOC	2	//"<<"
# define OUTPUT		3	//">"
# define APPEND		4	//">>"
# define PIPE		5	//"|"
# define SEPARATOR	6	//";"
# define CMD		7
# define ARG		8

typedef struct s_cmd
{
	char			**args;
	int				type;
	struct s_cmd	*next;
}					t_cmd;

typedef struct s_shell
{
	t_list			*envp;
	t_cmd			*cmd;
	int 			exit_status;
}					t_shell;

void	panic(char *str);
void	ft_getcwd(char *buf, size_t size);
void	free_all(t_shell *shell, char *error, int end);

#endif