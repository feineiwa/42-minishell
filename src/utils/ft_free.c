/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_free.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/11 22:33:14 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 18:18:13 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	ft_free_token(t_token *tok)
{
	t_token	*next;

	if (!tok)
		return ;
	while (tok)
	{
		next = tok->next;
		if (tok->value)
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

	if (*cmd == NULL || cmd == NULL)
		return ;
	while (*cmd)
	{
		next = (*cmd)->next;
		if ((*cmd)->input_file)
			ft_free((*cmd)->input_file);
		if ((*cmd)->output_file)
			ft_free((*cmd)->output_file);
		if ((*cmd)->error_file)
			ft_free((*cmd)->error_file);
		if ((*cmd)->hdoc)
			ft_free_hdoc(&(*cmd)->hdoc);
		if ((*cmd)->argv)
			ft_free_arg((*cmd)->argv);
		ft_free(*cmd);
		*cmd = next;
	}
	ft_free(*cmd);
	*cmd = NULL;
}

void	ft_free_env(t_env **envp)
{
	t_env	*tmp;

	if (!*envp || !envp)
		return ;
	while (*envp)
	{
		tmp = (*envp)->next;
		ft_free((*envp)->key);
		ft_free((*envp)->value);
		ft_free(*envp);
		*envp = tmp;
	}
	ft_free(*envp);
	*envp = NULL;
}
