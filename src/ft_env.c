/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 09:50:41 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/03 09:51:18 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
		env_var = ft_strdup(envp[i]);
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
