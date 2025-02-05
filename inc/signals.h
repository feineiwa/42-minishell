/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   signals.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 09:46:16 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 12:25:29 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef SIGNALS_H
# define SIGNALS_H

t_g_sig	*g_global(void);
void	setup_signal_for_hdoc(void);
void	setup_signal(void);

// Signal Child Process
int		handler_signal_hdoc(int *pipe_fd, pid_t pid, t_cmd *cmd,
			int std_fds[2]);
void	handler_signal_fork(pid_t pid);
int		handler_signal_pipe(pid_t pid);

#endif