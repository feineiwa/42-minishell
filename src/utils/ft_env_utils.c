/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 09:50:41 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/01 15:19:33 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

void	ft_free_one_env(t_env *tmp)
{
	ft_free(tmp->key);
	ft_free(tmp->value);
	ft_free(tmp);
}

void	ft_unset_env(t_env **envp, char *key)
{
	t_env	*to_free;
	t_env	*tmp;

	if (!key)
		return ;
	tmp = *envp;
	if (!strcmp(tmp->key, key))
	{
		*envp = (*envp)->next;
		ft_free_one_env(tmp);
	}
	else
	{
		while (tmp && tmp->next)
		{
			if (!strcmp(tmp->next->key, key))
			{
				to_free = tmp->next;
				tmp->next = tmp->next->next;
				ft_free_one_env(to_free);
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
