/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:22:10 by nrasamim          #+#    #+#             */
/*   Updated: 2024/12/31 10:49:54 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <errno.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>

# define PROMPT "minishell$ "

typedef enum s_type
{
	EXEC = 1, // commande normale
	REDIR,    // redirection (<, >, >>)
	PIPE,     // Pipe (|)
	LIST,     // list (; ou &&/||)
}			t_type;

typedef struct s_cmd
{
	char	**args;
	int		type;
	struct s_cmd *next; // commande suivante (pour les pipes/lists)
}			t_cmd;

typedef struct s_shell
{
	char **env; // copie des variables d'environnement
	int status; // Dernier code de retour d'execution
	char *current_dir; //the current working directory
}			t_shell;

#endif