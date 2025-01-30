/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 11:35:59 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/30 18:28:04 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	skip_quotes(char *tok, char *arg, int *i, int *j)
{
	char	quote;

	quote = ft_is_quote(tok[*i]);
	(*i)++;
	while (tok[*i] && quote)
	{
		if (ft_is_quote(tok[*i]) == quote)
		{
			(*i)++;
			return (1);
		}
		arg[(*j)++] = tok[(*i)++];
	}
	return (0);
}

char	*ft_get_arg(t_shell *shell, char *tok)
{
	char	*arg;
	int		i;
	int		j;

	if (!shell->envp || !tok)
		return (NULL);
	arg = ft_calloc(sizeof(char), ft_strlen_skip_quote(tok) + 1);
	if (!arg)
		return (NULL);
	i = 0;
	j = 0;
	while (tok[i])
	{
		if (ft_is_quote(tok[i]) && skip_quotes(tok, arg, &i, &j))
			continue ;
		arg[j++] = tok[i++];
	}
	arg[j] = 0;
	return (arg);
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

static t_cmd	*init_cmd(t_cmd *cmd)
{
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	if (!cmd)
		return (NULL);
	cmd->argc = 0;
	cmd->argv = (char **)malloc(sizeof(char *));
	cmd->argv[0] = NULL;
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

static t_bool	synthax_error(t_token *tok)
{
	if (!tok)
		return (TRUE);
	if (!ft_strcmp(tok->value, "<") && tok->type == INFILE)
		if (!tok->next || (tok->next && tok->next->type != ARGS))
			return (TRUE);
	if (!ft_strcmp(tok->value, "<<") && tok->type == HEREDOC)
		if ((tok->next && tok->next->type != ARGS) || (!tok->next))
			return (TRUE);
	if (!ft_strcmp(tok->value, ">") && tok->type == OUTFILE)
		if (!tok->next || (tok->next && tok->next->type != ARGS))
			return (TRUE);
	if (!ft_strcmp(tok->value, ">>") && tok->type == APPEND)
		if (!tok->next || (tok->next && tok->next->type != ARGS))
			return (TRUE);
	if (!ft_strcmp(tok->value, "|") && tok->type == PIPE)
		if (!tok->next)
			return (TRUE);
	return (FALSE);
}

static int	handle_argument(t_cmd **tmp, t_shell *shell, t_token **tok)
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

static int	handle_input_file(t_cmd **tmp, t_token **tok, int *error_flag,
		t_cmd *cmd_list)
{
	int	fd;

	if (synthax_error(*tok))
	{
		write(STDERR_FILENO, "syntax error near unexpected *token\n", 36);
		return (ft_free_cmd(&cmd_list), 0);
	}
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
		if ((fd = open((*tmp)->input_file, O_RDONLY)) < 0)
		{
			(*tmp)->error_file = ft_strdup((*tmp)->input_file);
			(*tmp)->flag_err = 1;
			*error_flag = 1;
		}
		close(fd);
	}
	return (1);
}

int	handle_output_file(t_cmd **tmp, t_token **tok, int *error_flag,
		t_cmd *cmd_list)
{
	int	fd;

	if (synthax_error(*tok))
	{
		write(STDERR_FILENO, "syntax error near unexpected token\n", 36);
		ft_free_cmd(&cmd_list);
		return (0);
	}
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
		if ((fd = open((*tmp)->output_file, O_CREAT | O_WRONLY | O_TRUNC,
					0644)) < 0)
		{
			(*tmp)->error_file = ft_strdup((*tmp)->output_file);
			(*tmp)->flag_err = 2;
			*error_flag = 1;
		}
		close(fd);
	}
	return (1);
}

int	handle_append_file(t_cmd **tmp, t_token **tok, int *error_flag,
		t_cmd *cmd_list)
{
	int	fd;

	if (synthax_error(*tok))
	{
		write(STDERR_FILENO, "syntax error near unexpected token\n", 36);
		ft_free_cmd(&cmd_list);
		return (0);
	}
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
		if ((fd = open((*tmp)->output_file, O_WRONLY | O_CREAT | O_APPEND,
					0644)) < 0)
		{
			(*tmp)->error_file = ft_strdup((*tmp)->output_file);
			(*tmp)->flag_err = 3;
			*error_flag = 1;
		}
	}
	close(fd);
	return (1);
}

int	handle_heredoc_parsing(t_cmd **tmp, t_token **tok, t_cmd *cmd_list)
{
	t_hdoc	*new_hdoc;
	t_hdoc	*last;

	if (synthax_error(*tok))
	{
		write(STDERR_FILENO, "syntax error near unexpected *token\n", 36);
		ft_free_cmd(&cmd_list);
		return (0);
	}
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
	{
		last = (*tmp)->hdoc;
		while (last->next)
			last = last->next;
		last->next = new_hdoc;
	}
	return (1);
}

int	handle_pipeline(t_cmd **tmp, t_token **tok, int *error_flag,
		t_cmd *cmd_list)
{
	if (!(cmd_list->argv[0] || cmd_list->error_file || cmd_list->hdoc
			|| cmd_list->input_file || cmd_list->output_file))
	{
		write(STDERR_FILENO, "syntax error near unexpected token '|'\n", 40);
		ft_free_cmd(&cmd_list);
		return (0);
	}
	if (synthax_error(*(tok)))
	{
		write(STDERR_FILENO, "syntax error near unexpected token '|'\n", 40);
		ft_free_cmd(&cmd_list);
		return (0);
	}
	(*tmp)->next = init_cmd((*tmp)->next);
	(*tmp) = (*tmp)->next;
	*error_flag = 0;
	return (1);
}

t_cmd	*parse_into_cmd(t_shell *shell, t_token *tok)
{
	t_cmd	*tmp;
	t_cmd	*cmd_list;
	int		error_flag;
	int		ret;

	if (!tok || !shell)
		return (NULL);
	tmp = init_cmd(NULL);
	if (!tmp)
		return (NULL);
	cmd_list = tmp;
	error_flag = 0;
	ret = 1;
	while (tok)
	{
		if (tok->type == ARGS)
			ret = handle_argument(&tmp, shell, &tok);
		else if (tok->type == INFILE)
			ret = handle_input_file(&tmp, &tok, &error_flag, cmd_list);
		else if (tok->type == OUTFILE)
			ret = handle_output_file(&tmp, &tok, &error_flag, cmd_list);
		else if (tok->type == APPEND)
			ret = handle_append_file(&tmp, &tok, &error_flag, cmd_list);
		else if (tok->type == HEREDOC)
			ret = handle_heredoc_parsing(&tmp, &tok, cmd_list);
		else if (tok->type == PIPE)
			ret = handle_pipeline(&tmp, &tok, &error_flag, cmd_list);
		else
		{
			write(STDOUT_FILENO, "syntax error near unexpected token\n", 36);
			return (ft_free_cmd(&cmd_list), NULL);
		}
		if (!ret)
			return (NULL);
		tok = tok->next;
	}
	return (cmd_list);
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

t_token	*lexer_input(char *input)
{
	t_token	*tok;
	t_token	*tmp;
	char	*token;

	if (!input)
		return (NULL);
	tok = NULL;
	token = NULL;
	while (1)
	{
		token = ft_strtok_quoted(input);
		if (!token)
			break ;
		if (!tok)
		{
			tok = add_new_token(token);
			tmp = tok;
		}
		else
		{
			tmp->next = add_new_token(token);
			tmp = tmp->next;
		}
	}
	return (tok);
}

t_cmd	*parsing(t_shell *shell, char *input)
{
	t_token	*tok;
	char	*expand;

	tok = NULL;
	if (count_quotes(input))
	{
		write(STDERR_FILENO, "Error: the quotes should be close\n", 35);
		return (NULL);
	}
	g_global()->shell = shell;
	expand = ft_expand(shell, input);
	if (!expand)
		return (NULL);
	tok = lexer_input(expand);
	if (!tok)
		return (ft_free(expand), NULL);
	ft_free(expand);
	shell->cmd = parse_into_cmd(shell, tok);
	if (!shell->cmd)
		return (ft_free_token(tok), NULL);
	return (ft_free_token(tok), shell->cmd);
}
