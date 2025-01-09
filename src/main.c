/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:21:53 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/09 12:39:15 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// int	ft_count_token(char *str)
// {
// 	int		count;
// 	int		i;
// 	char	quote;

// 	count = 0;
// 	i = 0;
// 	while (str[i])
// 	{
// 		while (ft_isspace(str[i]))
// 			i++;
// 		if (str[i] == '\0')
// 			break ;
// 		if (str[i] == '\'' || str[i] == '\"')
// 		{
// 			quote = str[i++];
// 			while (str[i] && str[i] != quote)
// 				i++;
// 			if (str[i] == quote)
// 				i++;
// 		}
// 		else
// 		{
// 			while (str[i] && !ft_isspace(str[i]) && str[i] != '\''
// 				&& str[i] != '\"')
// 				i++;
// 		}
// 		count++;
// 	}
// 	// printf("count is %d\n", count);
// 	return (count);
// }



int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	char	*input;

	(void)ac;
	(void)av;
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
			// if (shell.cmd)
			// {
			// 	execute_command(shell.cmd, shell);
			// 	free_command(shell.cmd);
			// }
		}
		ft_free(input);
	}
	ft_free_env(&shell.envp);
	return (0);
}
