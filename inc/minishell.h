/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:22:10 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/03 09:52:23 by frahenin         ###   ########.fr       */
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

enum				s_tokens
{
	PIPE = 1,
	INFILE,
	OUTFILE,
	HDOC,
	APPEND,
	ARGS,
};

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
	int				exit_status;
}					t_shell;

// ENV

t_shell				init_shell(char **envp);

#endif