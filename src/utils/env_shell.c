/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_shell.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/01 15:17:44 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/05 17:32:43 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**convert_env_to_array(t_env *envp)
{
	t_env	*tmp;
	size_t	len;
	char	**arr_envp;
	size_t	i;

	len = 0;
	tmp = envp;
	while (tmp)
	{
		len++;
		tmp = tmp->next;
	}
	arr_envp = malloc(sizeof(char *) * (len + 1));
	if (!arr_envp)
		return (NULL);
	i = 0;
	tmp = envp;
	while (tmp)
	{
		arr_envp[i] = ft_strjoin3(tmp->key, "=", tmp->value);
		i++;
		tmp = tmp->next;
	}
	arr_envp[i] = NULL;
	return (arr_envp);
}

void	ft_swap_env(t_env *i, t_env *j, t_env *tmp)
{
	char	*tmp_key;
	char	*tmp_value;

	while (tmp)
	{
		if (ft_strcmp(i->key, j->key) < 0)
		{
			tmp_key = i->key;
			i->key = j->key;
			j->key = tmp_key;
			tmp_value = i->value;
			i->value = j->value;
			j->value = tmp_value;
		}
		tmp = tmp->next;
	}
}

static t_env	*populate_env_list(char **envp)
{
	t_env	*env_list;
	t_env	*tmp;
	char	*key;
	size_t	i;
	size_t	j;

	i = -1;
	env_list = NULL;
	while (envp[++i])
	{
		j = ft_search_equ(envp[i]);
		if (envp[i][j] == '=')
			key = ft_substr(envp[i], 0, j);
		if (!env_list)
		{
			env_list = add_new_env(key, ft_strdup(envp[i] + j + 1));
			tmp = env_list;
		}
		else
		{
			tmp->next = add_new_env(key, ft_strdup(envp[i] + j + 1));
			tmp = tmp->next;
		}
	}
	return (env_list);
}

t_shell	init_shell(char **envp)
{
	t_shell	shell;

	shell.envp = populate_env_list(envp);
	if (!shell.envp)
	{
		shell.cmd = NULL;
		return (shell);
	}
	shell.cmd = NULL;
	return (shell);
}
