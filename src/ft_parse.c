/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 11:35:59 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/28 16:47:40 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_fork(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		panic("fork");
	return (pid);
}

char	*ft_get_arg(t_shell *shell, char *tok)
{
	char	*arg;
	char	quote;
	int		i;
	int		j;

	if (!shell->envp || !tok)
		return (NULL);
	arg = ft_calloc(sizeof(char), ft_strlen_skip_quote(tok) + 1);
	if (!arg)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (tok[i])
	{
		if (ft_is_quote(tok[i]))
		{
			quote = ft_is_quote(tok[i]);
			i++;
			while (tok[i] && quote)
			{
				if (ft_is_quote(tok[i]) == quote)
				{
					quote = 0;
					i++;
					continue ;
				}
				arg[j++] = tok[i++];
			}
			if (tok[i] == '\0')
				break ;
			continue ;
		}
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

t_bool	synthax_error(t_token *tok)
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

t_cmd	*parse_into_cmd(t_shell *shell, t_token *tok)
{
	t_cmd	*tmp;
	t_cmd	*cmd_list;
	int		fd;
	int		error_flag;
	t_hdoc	*last;
	t_hdoc	*new_hdoc;

	tmp = NULL;
	if (!tok || !shell)
		return (NULL);
	tmp = init_cmd(tmp);
	if (!tmp)
		return (NULL);
	cmd_list = tmp;
	fd = 0;
	error_flag = 0;
	while (tok)
	{
		if (tok->type == ARGS)
		{
			tmp->argv = (char **)ft_realloc(tmp->argv, sizeof(char *)
					* (tmp->argc), sizeof(char *) * (tmp->argc + 2));
			if (!tmp->argv)
				return (NULL);
			tmp->argv[tmp->argc] = ft_get_arg(shell, tok->value);
			if (!tmp->argv)
				return (NULL);
			tmp->argc++;
			tmp->argv[tmp->argc] = NULL;
		}
		else if (tok->type == INFILE)
		{
			if (synthax_error(tok))
			{
				write(STDERR_FILENO, "syntax error near unexpected token\n",
					36);
				ft_free_cmd(&cmd_list);
				return (NULL);
			}
			tok = tok->next;
			if (!tok)
				return (NULL);
			if (!error_flag)
			{
				if (tmp->input_file)
				{
					ft_free(tmp->input_file);
					tmp->input_file = NULL;
				}
				tmp->input_file = ft_get_arg(shell, tok->value);
				if ((fd = open(tmp->input_file, O_RDONLY)) < 0)
				{
					tmp->error_file = ft_strdup(tmp->input_file);
					tmp->flag_err = 1;
					error_flag = 1;
				}
				close(fd);
			}
		}
		else if (tok->type == OUTFILE)
		{
			if (synthax_error(tok))
			{
				write(STDERR_FILENO, "syntax error near unexpected token\n",
					36);
				ft_free_cmd(&cmd_list);
				return (NULL);
			}
			tok = tok->next;
			if (!tok)
				return (NULL);
			if (!error_flag)
			{
				if (tmp->output_file)
				{
					ft_free(tmp->output_file);
					tmp->output_file = NULL;
				}
				tmp->output_file = ft_get_arg(shell, tok->value);
				if ((fd = open(tmp->output_file, O_CREAT | O_WRONLY | O_TRUNC,
							0644)) < 0)
				{
					tmp->error_file = ft_strdup(tmp->output_file);
					tmp->flag_err = 2;
					error_flag = 1;
				}
				close(fd);
			}
		}
		else if (tok->type == APPEND)
		{
			if (synthax_error(tok))
			{
				write(STDERR_FILENO, "syntax error near unexpected token\n",
					36);
				ft_free_cmd(&cmd_list);
				return (NULL);
			}
			tmp->append = TRUE;
			tok = tok->next;
			if (!tok)
				return (NULL);
			if (tmp->output_file)
			{
				ft_free(tmp->output_file);
				tmp->output_file = NULL;
			}
			tmp->output_file = ft_get_arg(shell, tok->value);
			if ((fd = open(ft_get_arg(shell, tmp->output_file),
						O_WRONLY | O_CREAT | O_APPEND, 0644)) < 0)
			{
				tmp->error_file = ft_strdup(tmp->output_file);
				tmp->flag_err = 3;
				error_flag = 1;
			}
			close(fd);
		}
		else if (tok->type == HEREDOC)
		{
			if (synthax_error(tok))
			{
				write(STDERR_FILENO, "syntax error near unexpected token\n",
					36);
				ft_free_cmd(&cmd_list);
				return (NULL);
			}
			tok = tok->next;
			if (!tok)
				return (NULL);
			if (tmp->input_file)
			{
				ft_free(tmp->input_file);
				tmp->input_file = NULL;
			}
			new_hdoc = init_hdoc(NULL);
			if (!new_hdoc)
				return (NULL);
			if (!ft_strchr(tok->value, '\'') && !ft_strchr(tok->value, '"'))
				new_hdoc->expanded = TRUE;
			new_hdoc->del = ft_get_arg(shell, tok->value);
			if (!tmp->hdoc)
				tmp->hdoc = new_hdoc;
			else
			{
				last = tmp->hdoc;
				while (last->next)
					last = last->next;
				last->next = new_hdoc;
			}
		}
		else if (tok->type == PIPE)
		{
			if (!(cmd_list->argv[0] || cmd_list->error_file || cmd_list->hdoc
					|| cmd_list->input_file || cmd_list->output_file))
			{
				write(STDERR_FILENO, "syntax error near unexpected token '|'\n",
					40);
				ft_free_cmd(&cmd_list);
				return (NULL);
			}
			if (synthax_error(tok))
			{
				write(STDERR_FILENO, "syntax error near unexpected token '|'\n",
					40);
				ft_free_cmd(&cmd_list);
				return (NULL);
			}
			tmp->next = init_cmd(tmp->next);
			tmp = tmp->next;
			error_flag = 0;
		}
		else
		{
			write(STDOUT_FILENO, "syntax error near unexpected token\n", 36);
			ft_free_cmd(&cmd_list);
			return (NULL);
		}
		tok = tok->next;
	}
	return (cmd_list);
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
	token = NULL;
	while (1)
	{
		token = ft_strtok_quoted(input);
		if (!token)
			break ;
		new_tok = (t_token *)malloc(sizeof(t_token));
		if (!new_tok)
			return (NULL);
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
	expand = ft_expand(shell, input);
	if (!expand)
		return (NULL);
	tok = lexer_input(expand);
	if (!tok)
	{
		ft_free(expand);
		return (NULL);
	}
	ft_free(expand);
	shell->cmd = parse_into_cmd(shell, tok);
	if (!shell->cmd)
	{
		ft_free_token(tok);
		return (NULL);
	}
	ft_free_token(tok);
	return (shell->cmd);
}
