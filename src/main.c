/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:21:53 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/11 15:25:01 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_cmd	*parsing(t_shell *shell, char *input)
{
	t_cmd	*cmd_list;
	t_token	*tok;

	tok = lexer_input(input);
	if (!tok)
		return (NULL);
	cmd_list = parse_input(tok);
	if (!cmd_list)
		return (NULL);
	return (cmd_list);
}

static bool	execute_command(t_shell *shell)
{
	t_cmd	*temp;

	temp = shell->cmd;
	if (temp->argv[0] && !temp->next && is_valid_cmd(temp->argv[0]))
		return (launch_cmd(shell, temp));
	else if (temp->next)
		return (config_with_pipe(shell, temp));
	return (false);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	char	*input;

	(void)ac;
	(void)av;
	shell = init_shell(envp);
	while (42)
	{
		input = readline(PROMPT);
		if (input == NULL)
		{
			printf("exit\n");
			break ;
		}
		if (input && *input)
		{
			add_history(input);
			shell.cmd = parsing(&shell, input);
			if (shell.cmd)
			{
				// example commande
				shell.cmd->argv = (char *[]){ "cat", NULL };
				shell.cmd->input_file = NULL;
				shell.cmd->output_file = NULL;
				shell.cmd->here_doc.del = ft_strdup("end");
				shell.cmd->here_doc.expanded = false;
				shell.cmd->append = false;

				t_cmd *next_cmd = malloc(sizeof(t_cmd));
				next_cmd->argv = (char *[]){ "cat", NULL };
				next_cmd->input_file = NULL;
				next_cmd->output_file = NULL;
				next_cmd->append = false;

				t_cmd *last_cmd = malloc(sizeof(t_cmd));
				last_cmd->argv = (char *[]){ "cat", NULL };
				last_cmd->input_file = NULL;
				last_cmd->output_file = ft_strdup("txt.txt");
				last_cmd->append = true;

				shell.cmd->next = NULL;
				next_cmd->next = last_cmd;
				last_cmd->next = NULL;

			 	if (!execute_command(&shell))
				{
					perror("Error during excecution");
					break ;
				}
			 	// free_cmd(shell.cmd);
			}
		}
		free(input);
	}
	rl_clear_history();
	ft_lstclear(&shell.envp, free);
	return (0);
}
