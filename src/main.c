/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:21:53 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/16 10:14:46 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_bool	execute_command(t_shell *shell)
{
	t_cmd	*temp;

	temp = shell->cmd;
	if (temp->argv[0] && !temp->next)
		return (launch_cmd_without_pipe(shell, temp));
	else if (temp->next)
		return (config_with_pipe(shell, temp));
	return (FALSE);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	char	*input;
	int		i;

	(void)ac;
	(void)av;
	printf("[%s]\n", av[0]);
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
			if (!execute_command(&shell))
			{
				ft_free(input);
				ft_free_cmd(&shell.cmd);
				continue ;
			}
			ft_free_cmd(&shell.cmd);
		}
		ft_free(input);
	}
	rl_clear_history();
	ft_free_env(&shell.envp);
	return (0);
}
