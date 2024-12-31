/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:21:53 by nrasamim          #+#    #+#             */
/*   Updated: 2024/12/31 14:04:41 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	free_shell(t_shell *shell)
{
	int	i;

	if (!shell)
		return ;
	if (shell->env)
	{
		i = 0;
		while (shell->env[i])
		{
			free(shell->env[i]);
			i++;
		}
		free(shell->env);
	}
	if (shell->current_dir)
		free(shell->current_dir);
	free(shell);
}

t_shell	*init_shell(char **envp)
{
	t_shell	*shell;
	int		i;
	int		env_len;

	shell = (t_shell *)malloc(sizeof(t_shell));
	if (!shell)
	{
		perror("malloc");
		exit(EXIT_FAILURE);
	}
	env_len = 0;
	while (envp[env_len])
		env_len++;
	shell->env = (char **)malloc(sizeof(char *) * ((env_len) + 1));
	if (!shell->env)
	{
		perror("nalloc");
		free(shell);
		exit(EXIT_FAILURE);
	}
	i = 0;
	while (envp[i])
	{
		shell->env[i] = ft_strdup(envp[i]);
		if (!shell->env[i])
		{
			perror("ft_strdup");
			free_shell(shell);
			exit(EXIT_FAILURE);
		}
		i++;
	}
	shell->env[i] = NULL;
	shell->status = 0;
	shell->current_dir = getcwd(NULL, 0);
	return (shell);
}

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*s1 - *s2);
}

t_cmd *parse_input(char *input, t_shell *shell)
{
	t_cmd	*cmd;

	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	cmd->args
	return (cmd);
}

int	main(int ac, char **av, char **envp)
{
	t_shell *shell;
	char *input;
	t_cmd *cmd;

	(void)ac;
	(void)av;
	shell = init_shell(envp);

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
			cmd = parse_input(input, shell);
			// if (cmd)
			// {
			// 	execute_command(cmd, shell);
			// 	free_command(cmd);
			// }
		}
		free(input);
	}
	free_shell(shell);
	return (0);
}