/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:58:21 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/06 17:15:44 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_g_sig	*g_global(void)
{
	static t_g_sig	value = {0, 0, {-1, -1}, NULL, 0, NULL};

	return (&value);
}

static void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		g_global()->exit_status = 130;
		write(STDOUT_FILENO, "\n", 1);
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
}

void	setup_signal(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_flags = 0;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigemptyset(&sa_quit.sa_mask);
	signal(SIGPIPE, SIG_IGN);
	if (g_global()->is_runing == 1)
	{
		sa_int.sa_handler = SIG_DFL;
		sa_quit.sa_handler = SIG_DFL;
	}
	else if (g_global()->is_runing == 2)
	{
		sa_int.sa_handler = SIG_IGN;
		sa_quit.sa_handler = SIG_IGN;
	}
	else
	{
		sa_int.sa_handler = &handle_sigint;
		sa_quit.sa_handler = SIG_IGN;
	}
	sigaction(SIGINT, &sa_int, NULL);
	sigaction(SIGQUIT, &sa_quit, NULL);
}

static void	handle_sigint_for_hdoc(int sig)
{
	if (sig == SIGINT)
	{
		g_global()->is_runing = SIGINT;
		close(0);
		close(1);
		close(2);
	}
	else
		g_global()->is_runing = 3;
}

void	setup_signal_for_hdoc(void)
{
	struct sigaction	sa_int;
	struct sigaction	sa_quit;

	sa_int.sa_flags = 0;
	sa_quit.sa_flags = 0;
	sigemptyset(&sa_int.sa_mask);
	sigemptyset(&sa_quit.sa_mask);
	sa_int.sa_handler = &handle_sigint_for_hdoc;
	sa_quit.sa_handler = SIG_IGN;
	sigaction(SIGINT, &sa_int, NULL);
}
