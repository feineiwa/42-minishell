/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parsing.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/06 16:30:24 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/06 16:32:39 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
	return (ARGS);
}

t_token	*lexer_input(char *input)
{
	t_token	*tok;
	t_token	*new_tok;
	t_token	*tmp;
	char	*token;

	if (!input)
		return (NULL);
	new_tok = NULL;
	tok = NULL;
	if (count_quotes(input))
	{
		panic("Error numbers of quotes");
		return (NULL);
	}
	while (1)
	{
		token = ft_strtok_quoted(input);
		if (!token)
			break ;
		new_tok = malloc(sizeof(t_cmd));
		new_tok->value = token;
		new_tok->next = NULL;
		new_tok->type = assign_type(token);
		if (!tok)
		{
			tok = new_tok;
			tmp = tok;
		}
		else
		{
			tmp->next = new_tok;
			tmp = tmp->next;
		}
	}
	return (tok);
}

t_cmd	*parse_input(t_token *tok)
{
	t_cmd	*cmd_list;

	cmd_list = malloc(sizeof(t_cmd));
	if (!cmd_list)
		return (NULL);
	return (cmd_list);
}
