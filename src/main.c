/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:21:53 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/24 12:29:21 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_bool	execute_command(t_shell *shell)
{
	t_cmd	*temp;

	g_global()->is_runing = 2;
	temp = shell->cmd;
	if (temp)
		return (config_with_pipe(shell, temp));
	return (FALSE);
}

int	main(int ac, char **av, char **envp)
{
	t_shell				shell;
	t_bool				status;
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
			status = execute_command(&shell);
			if (status == FALSE)
			{
				ft_free(input);
				ft_free_cmd(&shell.cmd);
				break ;
			}
			else if (status == EXIT)
			{
				write(STDERR_FILENO, "exit\n", 6);
				ft_free(input);
				ft_free_cmd(&shell.cmd);
				break ;
			}
			ft_free_cmd(&shell.cmd);
		}
	}
	rl_clear_history();
	ft_free_env(&shell.envp);
	return (g_global()->exit_status);
}
