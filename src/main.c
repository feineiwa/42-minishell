/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:21:53 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/02 19:12:38 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	panic(char *s)
{
	perror(s);
}

static t_list	*populate_env_list(char **envp)
{
	t_list	*env_list;
	t_list	*new_node;
	char	*env_var;
	size_t	i;

	i = 0;
	env_list = NULL;
	while (envp[i])
	{
		env_var =  ft_strdup(envp[i]);
		if (!env_var)
		{
			ft_lstclear(&env_list, free);
			return (NULL);
		}
		new_node = ft_lstnew(env_var);
		if (!new_node)
		{
			free(env_var);
			ft_lstclear(&env_list, free);
			return (NULL);
		}
		ft_lstadd_back(&env_list, new_node);
		i++;
	}
	return (env_list);
}

t_shell	init_shell(char **envp)
{
	t_shell	shell;

	shell.envp = populate_env_list(envp);
	if (!shell.envp)
	{
		shell.exit_status = -1;
		shell.cmd = NULL;
		return (shell);
	}
	shell.cmd = NULL;
	shell.exit_status = 0;
	return (shell);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	char	*input;

	(void)ac;
	(void)av;
	shell = init_shell(envp);
	while (42)
	{
		input = readline(PROMPT);
		if (input == NULL)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (input && *input)
		{
			add_history(input);
			// parse_input(&mini, input);
			// if (cmd)
			// {
			// 	execute_command(cmd, shell);
			// 	free_command(cmd);
			// }
		}
		free(input);
	}
	ft_lstclear(&shell.envp, free);
	return (0);
}
