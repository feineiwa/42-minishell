/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_unset.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 09:39:28 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 12:10:30 by frahenin         ###   ########.fr       */
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
