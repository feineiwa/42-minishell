/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 11:35:59 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 12:10:33 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	cmd_valid(t_cmd **tmp, t_token **tok, int *error_flag,
		t_cmd *cmd_list)
{
	int	ret;

	ret = 1;
	if ((*tok)->type == ARGS)
		ret = handle_argument(tmp, g_global()->shell, tok);
	else if ((*tok)->type == INFILE)
		ret = handle_input_file(tmp, tok, error_flag, cmd_list);
	else if ((*tok)->type == OUTFILE)
		ret = handle_output_file(tmp, tok, error_flag, cmd_list);
	else if ((*tok)->type == APPEND)
		ret = handle_append_file(tmp, tok, error_flag, cmd_list);
	else if ((*tok)->type == HEREDOC)
		ret = handle_heredoc_parsing(tmp, tok, cmd_list);
	else if ((*tok)->type == PIPE)
		ret = handle_pipeline(tmp, tok, error_flag, cmd_list);
	if (ret == 0)
	{
		g_global()->exit_status = 2;
		return (0);
	}
	return (1);
}

static t_cmd	*parse_into_cmd(t_shell *shell, t_token *tok)
{
	t_cmd	*tmp;
	t_cmd	*cmd_list;
	int		error_flag;

	if (!tok || !shell)
		return (NULL);
	g_global()->shell = shell;
	tmp = init_cmd(NULL);
	if (!tmp)
		return (NULL);
	cmd_list = tmp;
	error_flag = 0;
	while (tok)
	{
		if (tok->type == NONE)
			return (print_synthax_error(&cmd_list), NULL);
		else if (!cmd_valid(&tmp, &tok, &error_flag, cmd_list))
			return (NULL);
		tok = tok->next;
	}
	return (cmd_list);
}

t_cmd	*parsing(t_shell *shell, char *input)
{
	t_token	*tok;
	char	*expand;

	tok = NULL;
	if (count_quotes(input))
	{
		ft_free(input);
		return (ft_putstr_fd("Error: Unmatched quotes detected\n", 2), NULL);
	}
	g_global()->shell = shell;
	expand = ft_expand(shell, input);
	if (!expand)
	{
		g_global()->exit_status = 0;
		return (ft_free(input), NULL);
	}
	ft_free(input);
	tok = lexer_input(expand);
	if (!tok)
		return (ft_free(expand), NULL);
	ft_free(expand);
	shell->cmd = parse_into_cmd(shell, tok);
	if (!shell->cmd)
		return (ft_free_token(tok), NULL);
	return (ft_free_token(tok), shell->cmd);
}
