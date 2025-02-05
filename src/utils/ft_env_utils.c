/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_utils.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 09:50:41 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 12:10:33 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

t_env	*add_new_env(char *key, char *value)
{
	t_env	*new;

	new = ft_calloc(sizeof(t_env), 1);
	if (!new)
		return (NULL);
	new->key = key;
	new->value = value;
	new->next = NULL;
	return (new);
}
