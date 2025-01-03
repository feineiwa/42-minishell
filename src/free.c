/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   free.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 14:06:17 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/03 17:21:53 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

/*static void	free_cmd(t_cmd **list)
{

}*/

void	free_all(t_shell *shell, char *error, int end)
{
	/*if (shell->cmd)
		free_cmd(&shell->cmd);*/
	if (shell->envp)
		ft_lstclear(&shell->envp, free);
	if (error)
		printf("Error during process: %s\n", error);
	rl_clear_history();
	if (end != -1)
		exit(end);
}
