/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:21:53 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/03 17:27:12 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_list	*check_env_var(char **envp)
{
	t_list	*env_list;
	t_list	*new_list;
	char	*env_var;
	size_t	i;

	env_list = NULL;
	i = 0;
	while (envp[i])
	{
		env_var = ft_strdup(envp[i]);
		new_list = ft_lstnew(env_var);
		if (!env_var || !new_list)
		{
			free(env_var);
			ft_lstclear(&env_list, free);
			return (NULL);
		}
		ft_lstadd_back(&env_list, new_list);
		i++;
	}
	return (env_list);
}

static t_shell	init_shell(t_shell	*shell, char **envp)
{
	shell->envp = check_env_var(envp);
	shell->cmd = NULL;
	shell->exit_status = 0;
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	char	*input;

	(void)ac;
	(void)av;
	init_shell(&shell, envp);
	if (!shell.envp)
		free_all(&shell, "Undefined Environment Variable", -1);
	while (42)
	{
		input = readline("minishell$ ");
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (input && *input)
		{
			add_history(input);
			// Excecution??
		}
		free(input);
	}
	free_all(&shell, NULL, -1);
	return (0);
}
