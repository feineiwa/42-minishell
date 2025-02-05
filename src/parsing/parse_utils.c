/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 13:16:31 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 12:10:33 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

t_hdoc	*get_last_hdoc(t_hdoc *first)
{
	if (!first)
		return (NULL);
	while (first->next)
		first = first->next;
	return (first);
}

t_tok_type	assign_type(char *s)
{
	if (!ft_strcmp(s, "|"))
		return (PIPE);
	else if (!ft_strcmp(s, "<"))
		return (INFILE);
	else if (!ft_strcmp(s, ">"))
		return (OUTFILE);
	else if (!ft_strcmp(s, ">>"))
		return (APPEND);
	else if (!ft_strcmp(s, "<<"))
		return (HEREDOC);
	else if (ft_is_belong(s[0]))
		return (NONE);
	return (ARGS);
}

t_token	*add_new_token(char *s)
{
	t_token	*new_tok;

	new_tok = (t_token *)malloc(sizeof(t_token));
	if (!new_tok)
		return (NULL);
	new_tok->value = s;
	new_tok->next = NULL;
	new_tok->type = assign_type(s);
	return (new_tok);
}

t_hdoc	*init_hdoc(t_hdoc *hdoc)
{
	hdoc = (t_hdoc *)malloc(sizeof(t_hdoc));
	if (!hdoc)
		return (NULL);
	hdoc->del = NULL;
	hdoc->expanded = FALSE;
	hdoc->next = NULL;
	return (hdoc);
}

t_cmd	*init_cmd(t_cmd *cmd)
{
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argc = 0;
	cmd->argv = (char **)malloc(sizeof(char *));
	if (!cmd->argv)
	{
		ft_free(cmd);
		return (NULL);
	}
	cmd->append = FALSE;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->error_file = NULL;
	cmd->flag_err = 0;
	cmd->hdoc = NULL;
	cmd->next = NULL;
	return (cmd);
}
