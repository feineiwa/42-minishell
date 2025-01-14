/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:43:12 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/13 09:35:24 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_env(t_shell *shell, t_cmd *cmd)
{
	if (cmd->argv[1])
	{
		write (STDERR_FILENO, "env: too many arguments\n", 24);
		return (127);
	}
	print_env(shell->envp);
	return (0);
}