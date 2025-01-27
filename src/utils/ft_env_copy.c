/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env_copy.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 09:50:41 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/27 11:49:18 by nrasamim         ###   ########.fr       */
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

void	ft_add_env(t_env **envp, char *arg)
{
	int		i;
	char	*key;
	t_env	*current;
	t_env	*new_node;
	t_env	*last;

	i = 0;
	key = NULL;
	i = ft_search_equ(arg);
	if (i < 0 || arg[i] == '\0')
		return ;
	key = ft_substr(arg, 0, i);
	if (!key)
		return ;
	current = *envp;
	while (current)
	{
		if (!ft_strcmp(current->key, key))
		{
			ft_free(current->value);
			current->value = ft_strdup(arg + i + 1);
			ft_free(key);
			return ;
		}
		current = current->next;
	}
	new_node = ft_calloc(sizeof(t_env), 1);
	if (!new_node)
		return ;
	new_node->key = key;
	new_node->value = ft_strdup(arg + i + 1);
	new_node->next = NULL;
	if (*envp == NULL)
		*envp = new_node;
	else
	{
		last = ft_get_last_env(*envp);
		last->next = new_node;
	}
}
