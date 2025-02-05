/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 09:45:14 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 12:29:06 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef EXEC_H
# define EXEC_H

int		launch_cmd(t_shell *shell, t_cmd *cmd, int sa_std[2]);
int		config_with_pipe(t_shell *shell, t_cmd *cmd, int sa_std[2]);
void	save_fds_standart(int *saved_stdin, int *saved_stdout);
void	restore_standard(int sa_std[2]);
void	close_saved_std(int sa_std[2]);
void	restore_standard(int sa_std[2]);

// Single CMD
void	what_cmd_without_pipe(t_shell *shell, t_cmd *cmd, int sa_stdin,
			int sa_stdout);

// Pipe
void	what_cmd_with_pipe(t_shell *shell, t_cmd *cmd, int sa_stdin,
			int sa_stdout);

// Redirection
t_bool	handler_error_flag(t_cmd *cmd, int *input_fd, int *output_fd);
int		handler_input_redirection(char *input_file, int *in_fd);
int		handler_output_redirection(t_cmd *cmd, int *out_fd);

// Heredocument
int		handle_heredoc(t_cmd *cmd, t_shell *shell, int std_fds[2]);
int		handle_heredoc_with_pipe(t_cmd *cmd, t_shell *shell, int std_fds[2]);
int		init_global_hdoc_fd(t_cmd *cmd);
void	handle_ctrl_c(char *content, int pipe_fd[2], int std_fds[2], pid_t pid);
void	close_hdoc_fd_inherited_from_parent(void);
void	write_content_to_pipe(char *content, int pipe_fd[2]);
char	*expand_content_if_needed(char *content, t_hdoc *hdoc);

// Others CMD
char	*resolve_cmd_path(t_shell *shell, char *cmd);
int		handle_dot_cmd(t_shell *shell, t_cmd *cmd);
int		handle_relatif_cmd(t_shell *shell, t_cmd *cmd, char **envp);
int		handle_absolute_cmd(t_cmd *cmd, char **envp);
int		check_execution(char **envp, char *cmd_path, t_cmd *cmd);

#endif