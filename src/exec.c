/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 14:51:40 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/17 16:46:53 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_free_arr(char **arr)
{
	int	i;

	i = 0;
	if (!arr | !*arr)
		return ;
	while (arr[i])
	{
		ft_free(arr[i]);
		i++;
	}
	ft_free(arr);
}

char	*resolve_cmd_path(t_shell *shell, t_cmd *cmd)
{
	char	*path;
	char	**paths;
	int		i;
	char	*cmd_path;

	path = ft_get_env_value(shell->envp, "$PATH");
	if (!path)
		return (NULL);
	paths = ft_split(path, ':');
	i = 0;
	while (paths[i])
	{
		cmd_path = ft_strjoin3(paths[i], "/", cmd->argv[0]);
		if (access(cmd_path, X_OK) == 0)
		{
			ft_free_arr(paths);
			return (cmd_path);
		}
		ft_free(cmd_path);
		i++;
	}
	ft_free_arr(paths);
	return (NULL);
}

int	other_cmd(t_shell *shell, t_cmd *cmd)
{
	pid_t	pid;
	char	*cmd_path;
	char	**envp;
	int		status;

	pid = fork();
	if (pid == -1)
	{
		perror("fork in what_cmd");
		shell->exit_status = 1;
		return (1);
	}
	if (pid == 0)
	{
		cmd_path = resolve_cmd_path(shell, cmd);
		if (!cmd_path)
		{
			perror(cmd->argv[0]);
			return (1);
		}
		envp = convert_env_to_array(shell->envp);
		if (!envp)
		{
			ft_free(cmd_path);
			ft_free_arr(envp);
			return (1);
		}
		if (execve(cmd_path, cmd->argv, envp) == -1)
		{
			cmd->err = ft_strdup(strerror(errno));
			return (1);
		}
		exit(0);
	}
	else
	{
		waitpid(pid, &status, 0);
		if (WIFEXITED(status))
			shell->exit_status = WEXITSTATUS(status);
		else
			shell->exit_status = 1;
	}
	return (0);
}

void	what_cmd(t_shell *shell, t_cmd *cmd)
{
	if (!ft_strcmp("echo", cmd->argv[0]))
		shell->exit_status = ft_echo(cmd->argv);
	else if (!ft_strcmp("pwd", cmd->argv[0]))
		shell->exit_status = ft_pwd();
	else if (!ft_strcmp("export", cmd->argv[0]))
		shell->exit_status = ft_export(shell, cmd);
	else if (!ft_strcmp("env", cmd->argv[0]))
		shell->exit_status = ft_env(shell, cmd);
	else if (!ft_strcmp("unset", cmd->argv[0]))
		shell->exit_status = ft_unset(shell, cmd);
	else
		shell->exit_status = other_cmd(shell, cmd);
}

t_bool	launch_cmd_without_pipe(t_shell *shell, t_cmd *cmd)
{
	int	saved_stdin;
	int	saved_stdout;
	int	input_fd;
	int	output_fd;
	int	flags;
	
	saved_stdin = dup(STDIN_FILENO);
	saved_stdout = dup(STDOUT_FILENO);
	input_fd = -1;
	t_hdoc	*hdoc;

	if (cmd->hdoc)
	{
		input_fd = handle_heredoc(cmd, shell);
		close(input_fd);
	}
	if (cmd->input_file)
	{
		input_fd = open(cmd->input_file, O_RDONLY);
		if (input_fd < 0)
		{
			perror(cmd->input_file);
			close(input_fd);
			return (FALSE);
		}
		if (dup2(input_fd, STDIN_FILENO) < 0)
		{
			perror(cmd->input_file);
			close(input_fd);
			return (FALSE);
		}
	}
	output_fd = -1;
	if (cmd->output_file)
	{
		flags = O_WRONLY | O_CREAT;
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
	close(saved_stdin);
	close(saved_stdout);
	return (TRUE);
}

// 

t_bool launch_cmd_with_pipe(t_shell *shell, t_cmd *cmd)
{
    int     saved_stdin;
    int     saved_stdout;
    int     input_fd;
    int     output_fd;
    int     flags;

    saved_stdin = dup(STDIN_FILENO);
    saved_stdout = dup(STDOUT_FILENO);
    input_fd = -1;
    if (cmd->input_file)
    {
        input_fd = open(cmd->input_file, O_RDONLY);
        if (input_fd < 0)
        {
			fprintf(stderr, "%s\n", "mandalo ato");
            return (FALSE);
        }
        if (dup2(input_fd, STDIN_FILENO) < 0)
        {
            cmd->err = ft_strdup("minishell: dup2 input failed");
            close(input_fd);
            return (FALSE);
        }
    }

    output_fd = -1;
    if (cmd->output_file)
    {
        flags = O_WRONLY | O_CREAT;
        if (cmd->append)
            flags |= O_APPEND;
        else
            flags |= O_TRUNC;
        output_fd = open(cmd->output_file, flags, 0644);
        if (output_fd < 0)
        {
            cmd->err = ft_strdup(strerror(errno));  // Store the error message
            if (input_fd != -1)
                close(input_fd);
            return (FALSE);
        }
        if (dup2(output_fd, STDOUT_FILENO) < 0)
        {
            cmd->err = ft_strdup("minishell: dup2 output failed");
            close(output_fd);
            if (input_fd != -1)
                close(input_fd);
            return (FALSE);
        }
    }

    // Execute the command
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

    close(saved_stdin);
    close(saved_stdout);
    return (TRUE);
}
