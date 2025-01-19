/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_signal.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/19 17:58:21 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/19 18:42:30 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	handle_sigint(int sig)
{
	if (sig == SIGINT)
	{
		printf("\n");
		rl_on_new_line();
		rl_replace_line("", 0);
		rl_redisplay();
	}
	// get_signal_value()
}

t_g_sig	*handle_signal()
{
	static t_g_sig value = {0};
	return (&value);
}

void set_signal_value(int new_value)
{
	handle_signal()->signum = new_value;
}

int	get_signal_value()
{
	return (handle_signal()->signum);
}