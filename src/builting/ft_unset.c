/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:39:28 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/27 11:02:39 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_unset(t_shell *shell, t_cmd *cmd)
{
	int	i;

	if (!shell || !cmd)
		return (1);
	i = 1;
	while (cmd->argv[i])
	{
		ft_unset_env(&shell->envp, cmd->argv[i]);
		i++;
	}
	return (0);
}
