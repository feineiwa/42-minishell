/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 22:33:14 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/27 14:01:49 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_free_token(t_token *tok)
{
	t_token	*next;

	while (tok)
	{
		next = tok->next;
		ft_free(tok->value);
		ft_free(tok);
		tok = next;
	}
	ft_free(tok);
	tok = NULL;
}

void	ft_free_arg(char **argv)
{
	int	i;

	if (!argv || !*argv)
		return ;
	i = 0;
	while (argv[i])
	{
		ft_free(argv[i]);
		i++;
	}
	ft_free(argv);
	argv = NULL;
}

void	ft_free_hdoc(t_hdoc **hdoc)
{
	t_hdoc	*tmp;

	if (!hdoc || !*hdoc)
		return ;
	while (*hdoc)
	{
		tmp = (*hdoc)->next;
		ft_free((*hdoc)->del);
		(*hdoc)->expanded = FALSE;
		ft_free(*hdoc);
		*hdoc = tmp;
	}
}

void	ft_free_cmd(t_cmd **cmd)
{
	t_cmd	*next;

	while (*cmd)
	{
		next = (*cmd)->next;
		ft_free((*cmd)->input_file);
		ft_free((*cmd)->output_file);
		ft_free((*cmd)->error_file);
		ft_free_hdoc(&(*cmd)->hdoc);
		ft_free_arg((*cmd)->argv);
		ft_free(*cmd);
		*cmd = next;
	}
	ft_free(*cmd);
	*cmd = NULL;
}
