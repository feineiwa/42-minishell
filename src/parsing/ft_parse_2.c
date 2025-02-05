/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:44:46 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 12:21:21 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	handle_argument(t_cmd **tmp, t_shell *shell, t_token **tok)
{
	(*tmp)->argv = (char **)ft_realloc((*tmp)->argv, sizeof(char *)
			* ((*tmp)->argc), sizeof(char *) * ((*tmp)->argc + 2));
	if (!(*tmp)->argv)
		return (0);
	(*tmp)->argv[(*tmp)->argc] = ft_get_arg(shell, (*tok)->value);
	if (!(*tmp)->argv)
		return (0);
	(*tmp)->argc++;
	(*tmp)->argv[(*tmp)->argc] = NULL;
	return (1);
}

int	handle_input_file(t_cmd **tmp, t_token **tok, int *error_flag,
		t_cmd *cmd_list)
{
	int	fd;

	if (synthax_error(*tok))
		return (print_synthax_error(&cmd_list), 0);
	(*tok) = (*tok)->next;
	if (!(*tok))
		return (0);
	if (!*error_flag)
	{
		if ((*tmp)->input_file)
		{
			ft_free((*tmp)->input_file);
			(*tmp)->input_file = NULL;
		}
		(*tmp)->input_file = ft_get_arg(g_global()->shell, (*tok)->value);
		fd = open((*tmp)->input_file, O_RDONLY);
		if (fd < 0)
			set_error_file(tmp, (*tmp)->input_file, 1, error_flag);
		else
			close(fd);
	}
	return (1);
}

int	handle_output_file(t_cmd **tmp, t_token **tok, int *error_flag,
		t_cmd *cmd_list)
{
	int	fd;

	if (synthax_error(*tok))
		return (print_synthax_error(&cmd_list), 0);
	(*tok) = (*tok)->next;
	if (!(*tok))
		return (0);
	if (!*error_flag)
	{
		if ((*tmp)->output_file)
		{
			ft_free((*tmp)->output_file);
			(*tmp)->output_file = NULL;
		}
		(*tmp)->output_file = ft_get_arg(g_global()->shell, (*tok)->value);
		fd = open((*tmp)->output_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		if (fd < 0)
			set_error_file(tmp, (*tmp)->output_file, 2, error_flag);
		else
			close(fd);
	}
	return (1);
}

int	handle_append_file(t_cmd **tmp, t_token **tok, int *error_flag,
		t_cmd *cmd_list)
{
	int	fd;

	if (synthax_error(*tok))
		return (print_synthax_error(&cmd_list), 0);
	(*tmp)->append = TRUE;
	(*tok) = (*tok)->next;
	if (!(*tok))
		return (0);
	if (!*error_flag)
	{
		if ((*tmp)->output_file)
		{
			ft_free((*tmp)->output_file);
			(*tmp)->output_file = NULL;
		}
		(*tmp)->output_file = ft_get_arg(g_global()->shell, (*tok)->value);
		fd = open((*tmp)->output_file, O_WRONLY | O_CREAT | O_APPEND, 0644);
		if (fd < 0)
			set_error_file(tmp, (*tmp)->output_file, 3, error_flag);
		else
			close(fd);
	}
	return (1);
}

int	handle_heredoc_parsing(t_cmd **tmp, t_token **tok, t_cmd *cmd_list)
{
	t_hdoc	*new_hdoc;

	if (synthax_error(*tok))
		return (print_synthax_error(&cmd_list), 0);
	(*tok) = (*tok)->next;
	if (!(*tok))
		return (0);
	if ((*tmp)->input_file)
	{
		ft_free((*tmp)->input_file);
		(*tmp)->input_file = NULL;
	}
	new_hdoc = init_hdoc(NULL);
	if (!new_hdoc)
		return (0);
	if (!ft_strchr((*tok)->value, '\'') && !ft_strchr((*tok)->value, '"'))
		new_hdoc->expanded = TRUE;
	new_hdoc->del = ft_get_arg(g_global()->shell, (*tok)->value);
	if (!(*tmp)->hdoc)
		(*tmp)->hdoc = new_hdoc;
	else
		get_last_hdoc((*tmp)->hdoc)->next = new_hdoc;
	return (1);
}
