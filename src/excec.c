/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   excec.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:51:40 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/07 12:03:59 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

bool	is_builtin(char *cmd)
{
	if (!cmd)
		return (false);
	if (!ft_strcmp("echo", cmd) || !ft_strcmp("cd", cmd) \
	|| !ft_strcmp("pwd", cmd) || !ft_strcmp("export", cmd) \
	|| !ft_strcmp("unset", cmd) || !ft_strcmp("env", cmd) \
	|| !ft_strcmp("exit", cmd))
		return (true);
	return (false);
}

static void	what_cmd(t_shell *shell, t_cmd *cmd)
{
	if (!ft_strcmp("echo", cmd->argv[0]))
		shell->exit_status = ft_echo(cmd->argv);
	else if (!ft_strcmp("pwd", cmd->argv[0]))
		shell->exit_status = ft_pwd();
}

bool    launch_cmd(t_shell *shell, t_cmd *cmd)
{
	if (!cmd->input_file && !cmd->output_file)
	{
		what_cmd(shell, cmd);
		return (true);
	}
	return (false);
}
