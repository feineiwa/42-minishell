/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:21:53 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/12 00:02:13 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	char	*input;

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
		input = readline(PROMPT);
		if (input == NULL)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (input && *input)
		{
			add_history(input);
			shell.cmd = parsing(&shell, input);
			// if (!shell.cmd)
			// {
			// 	ft_free(input);
			// 	ft_free_env(&shell.envp);
			// 	exit(1);
			// }
			if (shell.cmd)
			{
			// 	execute_command(shell.cmd, shell);
				ft_free_cmd(&shell.cmd);
				ft_free(input);
				ft_free_env(&shell.envp);
				rl_clear_history();
				exit(0);
			}
		}
		ft_free(input);
	}
	ft_free_env(&shell.envp);
	rl_clear_history();
	return (0);
}
