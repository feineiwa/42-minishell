/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:21:53 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/21 17:07:27 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_bool	execute_command(t_shell *shell)
{
	t_cmd	*temp;

	g_global()->is_runing = 2;
	temp = shell->cmd;
	if (temp && !temp->next)
		return (config_with_pipe(shell, temp));
		// return (launch_cmd_without_pipe(shell, temp));
	else if (temp->next)
		return (config_with_pipe(shell, temp));
	return (FALSE);
}

int	main(int ac, char **av, char **envp)
{
	t_shell				shell;
	char				*input;

	(void)ac;
	(void)av;
	input = NULL;
	shell = init_shell(envp);
	if (!shell.envp)
	{
		perror("env error");
		exit(EXIT_FAILURE);
	}
	while (42)
	{
		g_global()->is_runing = 0;
		setup_signal();
		input = readline(PROMPT);
		if (input == NULL)
		{
			ft_putstr_fd("exit\n", STDOUT_FILENO);
			break ;
		}
		if (input && *input)
		{
			add_history(input);
			shell.cmd = parsing(&shell, input);
			if (!shell.cmd)
			{
				g_global()->exit_status = 1;
				ft_free(input);
				continue ;
			}
			if (!execute_command(&shell))
			{
				ft_free(input);
				ft_free_cmd(&shell.cmd);
				continue ;
			}
			ft_free_cmd(&shell.cmd);
		}
		// ft_free(input);
	}
	rl_clear_history();
	ft_free_env(&shell.envp);
	return (g_global()->exit_status);
}
