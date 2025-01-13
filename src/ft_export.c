/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:29:09 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/13 08:40:08 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_export(t_shell *shell, t_cmd *cmd)
{
	int	i;
	int	status;

	i = 1;
	status = 0;
	while (cmd->argv[i])
		ft_add_env(&shell->envp,cmd->argv[i++]);
	return (status);
}
