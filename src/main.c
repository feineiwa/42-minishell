/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:21:53 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/01 14:01:41 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	execute_command(t_shell *shell)
{
	t_cmd	*temp;

	g_global()->is_runing = 2;
	g_global()->hdoc_fd = NULL;
	g_global()->pipfd[0] = -1;
	g_global()->pipfd[1] = -1;
	temp = shell->cmd;
	if (temp && !temp->next)
		g_global()->exit_status = launch_cmd(shell, temp, 0);
	else if (temp->next)
		g_global()->exit_status = config_with_pipe(shell, temp);
}

static void	prompt_loop(t_shell *shell)
{
	char	*input;

	input = NULL;
	while (42)
	{
		g_global()->is_runing = 0;
		setup_signal();
		input = readline(PROMPT);
		if (!input)
			break ;
		if (input && *input)
		{
			add_history(input);
			shell->cmd = parsing(shell, input);
			if (!shell->cmd)
				g_global()->exit_status = 1;
			else
			{
				execute_command(shell);
				ft_free_cmd(&shell->cmd);
			}
		}
	}
	rl_clear_history();
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;

	(void)av;
	if (ac != 1)
		return (print_err("just write ./minishell to run the program\n", NULL,
				NULL, 2), 1);
	shell = init_shell(envp);
	if (!shell.envp)
		return (print_err("error env\n", NULL, NULL, 2), 1);
	prompt_loop(&shell);
	ft_putstr_fd("exit\n", STDOUT_FILENO);
	return (ft_free_env(&shell.envp), g_global()->exit_status);
}
