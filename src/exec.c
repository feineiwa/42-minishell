/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:51:40 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/12 20:09:29 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

t_bool	is_valid_cmd(char *cmd)
{
	if (!cmd)
		return (FALSE);
	if (!ft_strcmp("echo", cmd) || !ft_strcmp("cd", cmd) \
	|| !ft_strcmp("pwd", cmd) || !ft_strcmp("export", cmd) \
	|| !ft_strcmp("unset", cmd) || !ft_strcmp("env", cmd) \
	|| !ft_strcmp("exit", cmd) || !ft_strcmp("cat", cmd))
		return (TRUE);
	return (FALSE);
}

void	what_cmd(t_shell *shell, t_cmd *cmd)
{
	if (!ft_strcmp("echo", cmd->argv[0]))
		shell->exit_status = ft_echo(cmd->argv);
	else if (!ft_strcmp("pwd", cmd->argv[0]))
		shell->exit_status = ft_pwd();
    else if (!ft_strcmp("cat", cmd->argv[0]))
		shell->exit_status = ft_cat(cmd->argv[1]);
}

/*char    *read_fd(int fd)
{
    char    buffer[4026];
    int bytes_read;

    bytes_read = read(fd, buffer, sizeof(buffer));
    if (bytes_read == -1)
    {
        perror("Error reading file");
        close(fd);
        return (NULL);
    }
    return (buffer);
}*/

t_bool    launch_cmd(t_shell *shell, t_cmd *cmd)
{
	int input_fd;
    int output_fd;
	int saved_stdin;
    int saved_stdout;

	saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    if (cmd->hdoc->del)
        if (!handle_heredoc(cmd))
            return (FALSE);
	input_fd = -1;
    if (cmd->input_file)
    {
        input_fd = open(cmd->input_file, O_RDONLY);
        if (input_fd < 0)
        {
            perror("minishell: input_file");
            return (FALSE);
        }
        if (dup2(input_fd, STDIN_FILENO) < 0)
        {
            perror("minishell: dup2 input");
            close(input_fd);
            return (FALSE);
        }
    }
	output_fd = -1;
    if (cmd->output_file)
    {
        int flags = O_WRONLY | O_CREAT;
		if (cmd->append)
        	flags |= O_APPEND;
		else
			flags |= O_TRUNC;
        output_fd = open(cmd->output_file, flags, 0644);
        if (output_fd < 0)
        {
            perror("minishell: output_file");
            if (input_fd != -1)
                close(input_fd);
            return (FALSE);
        }
        if (dup2(output_fd, STDOUT_FILENO) < 0)
        {
            perror("minishell: dup2 output");
            close(output_fd);
            if (input_fd != -1)
                close(input_fd);
            return (FALSE);
        }
    }
	what_cmd(shell, cmd);
	if (input_fd != -1)
	{
		dup2(saved_stdin, STDIN_FILENO);
        close(input_fd);
	}
    if (output_fd != -1)
	{
		dup2(saved_stdout, STDOUT_FILENO);
        close(output_fd);
	}
    if (cmd->hdoc->del)
        dup2(saved_stdin, STDIN_FILENO);
	return (TRUE);
}
