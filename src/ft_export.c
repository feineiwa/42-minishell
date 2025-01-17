/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:29:09 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/15 07:47:15 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	print_export(t_shell *shell)
{
	t_env	*tmp;
	t_env	*i;
	t_env	*j;
	char	*tmp_key;
	char	*tmp_value;

	i = shell->envp;
	while (i)
	{	
		j = i->next;
		while (j)
		{
			tmp = shell->envp;
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
			j = j->next;
		}
		i = i->next;
	}
	tmp = shell->envp;
	while (tmp)
	{
		if (tmp->value)
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		else
			printf("declare -x %s\n", tmp->key);
		tmp = tmp->next;
	}
	
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
	if (i < 0)
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
	if (arg[i] == '=')
	{
		new_node->key = key;
		ft_free(new_node->value);
		new_node->value = ft_strdup(arg + i + 1);
		if (!new_node->value)
			new_node->value = ft_strdup("");
		new_node->next = NULL;
	}
	else
	{
		new_node->key = key;
		new_node->value = NULL;
		new_node->next = NULL;
	}
	if (*envp == NULL)
		*envp = new_node;
	else
	{
		last = ft_get_last_env(*envp);
		last->next = new_node;
	}
}

int	ft_export(t_shell *shell, t_cmd *cmd)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	if (!cmd->argv[i])
		print_export(shell);
	while (cmd->argv[i])
		ft_add_env(&shell->envp,cmd->argv[i++]);
	return (status);
}
