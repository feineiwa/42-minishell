/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:21:53 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/05 18:12:23 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	init_global(t_shell *shell)
{
	g_global()->is_runing = 2;
	g_global()->hdoc_fd = NULL;
	g_global()->pipfd[0] = -1;
	g_global()->pipfd[1] = -1;
	g_global()->shell = shell;
}

void	execute_command(t_shell *shell)
{
	t_cmd	*temp;
	int		sa_std[2];

	init_global(shell);
	save_fds_standart(&sa_std[0], &sa_std[1]);
	temp = shell->cmd;
	if (temp && !temp->next)
	{
		g_global()->use_pipe = 0;
		g_global()->exit_status = launch_cmd(shell, temp, sa_std);
		restore_standard(sa_std);
	}
	else if (temp->next)
	{
		g_global()->use_pipe = 1;
		init_global_hdoc_fd(shell->cmd);
		g_global()->exit_status = config_with_pipe(shell, temp, sa_std);
		if (g_global()->hdoc_fd)
			ft_free(g_global()->hdoc_fd);
		restore_standard(sa_std);
	}
}

static void	prompt_loop(t_shell *shell)
{
	char	*input;

	while (42)
	{
		input = NULL;
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
				g_global()->exit_status = g_global()->exit_status;
			else
			{
				execute_command(shell);
				if (shell->cmd)
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
	return (ft_free_env(&shell.envp), g_global()->exit_status);
}
