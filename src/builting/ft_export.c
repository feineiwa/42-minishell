/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:29:09 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/27 11:48:37 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

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

int	ft_export(t_shell *shell, t_cmd *cmd)
{
	int	i;

	i = 1;

	if (!cmd->argv[i])
		print_export(shell);
	while (cmd->argv[i])
		ft_add_env(&shell->envp,cmd->argv[i++]);
	return (0);
}
