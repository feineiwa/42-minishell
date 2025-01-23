/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 09:50:41 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/22 14:24:59 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_free(void *ptr)
{
	if (!ptr)
		return ;
	free(ptr);
	ptr = NULL;
}

int	ft_search_equ(char *s)
{
	size_t	i;

	if (!s)
		return (-1);
	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	return (i);
}

t_env	*ft_get_last_env(t_env *envp)
{
	if (!envp)
		return (NULL);
	while (envp->next)
		envp = envp->next;
	return (envp);
}

void	ft_unset_env(t_env **envp, char *key)
{
	t_env	*to_free;
	t_env	*tmp;

	if (!key)
		return ;
	to_free = NULL;
	tmp = *envp;
	if (!strcmp(tmp->key, key))
	{
		*envp = (*envp)->next;
		ft_free(tmp->key);
		ft_free(tmp->value);
		ft_free(tmp);
	}
	else
	{
		while (tmp && tmp->next)
		{
			if (!strcmp(tmp->next->key, key))
			{
				to_free = tmp->next;
				tmp->next = tmp->next->next;
				ft_free(to_free->key);
				ft_free(to_free->value);
				ft_free(to_free);
				return ;
			}
			tmp = tmp->next;
		}
	}
}

char	*ft_get_env_value(t_env *envp, char *key)
{
	if (key[0] != '$' || !key)
		return (NULL);
	while (envp)
	{
		if (!ft_strcmp(key + 1, envp->key))
			return (envp->value);
		envp = envp->next;
	}
	return (NULL);
}

static t_env	*populate_env_list(char **envp)
{
	t_env	*env_list;
	t_env	*new_node;
	t_env	*tmp;
	char	*key;
	char	*value;
	size_t	i;
	size_t	j;

	i = 0;
	env_list = NULL;
	while (envp[i])
	{
		j = 0;
		j += ft_search_equ(envp[i]);
		if (envp[i][j] == '=')
		{
			key = ft_substr(envp[i], 0, j);
			value = ft_strdup(envp[i] + j + 1);
		}
		new_node = ft_calloc(sizeof(t_env), 1);
		new_node->key = key;
		new_node->value = value;
		new_node->next = NULL;
		if (!env_list)
		{
			env_list = new_node;
			tmp = env_list;
		}
		else
		{
			tmp->next = new_node;
			tmp = tmp->next;
		}
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
		g_global()->exit_status = 1;
		shell.cmd = NULL;
		return (shell);
	}
	return (shell);
}

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

void	ft_free_env(t_env **envp)
{
	t_env	*tmp;

	if (!*envp || !envp)
		return ;
	while (*envp)
	{
		tmp = (*envp)->next;
		ft_free((*envp)->key);
		ft_free((*envp)->value);
		ft_free(*envp);
		*envp = tmp;
	}
	ft_free(*envp);
	*envp = NULL;
}
