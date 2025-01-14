/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 11:35:59 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/14 17:30:52 by frahenin         ###   ########.fr       */
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
	int		q;


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
	cmd->hdoc = (t_hdoc *)malloc(sizeof(t_hdoc));
	if (!cmd->hdoc)
	{
		ft_free(cmd->argv);
		ft_free(cmd);
	}
	cmd->hdoc->del = NULL;
	cmd->hdoc->expanded = FALSE;
	cmd->next = NULL;
	return (cmd);
}

t_cmd	*parse_into_cmd(t_shell *shell, t_token *tok)
{
	t_cmd	*tmp;
	t_cmd	*cmd_list;
	int		i;
	int		fd;
	int		n;

	tmp = NULL;
	if (!tok || !shell)
		return (NULL);
	tmp = init_cmd(tmp);
	if (!tmp)
		return (NULL);
	cmd_list = tmp;
	fd = 0;
	n = 1;
	while (tok)
	{
		if (tok->type == ARGS)
		{
			tmp->argv = (char **)ft_realloc(tmp->argv, sizeof(char *)
					* (tmp->argc), sizeof(char *) * (tmp->argc + 2));
			tmp->argv[tmp->argc] = ft_get_arg(shell, tok->value);
			tmp->argc++;
			tmp->argv[tmp->argc] = NULL;
			// if (!ft_strcmp(tmp->argv[tmp->argc], "echo"))
			// {
			// 	if (tok->next->type == ARGS)
			// 	{
			// 		tok = tok->next;
			// 		if (!ft_strcmp(tok->value, ""))
			// 	}
			// }
		}
		else if (tok->type == INFILE)
		{
			tok = tok->next;
			if (!tok)
				return (NULL);
			if (tmp->input_file)
				ft_free(tmp->input_file);
			tmp->input_file = ft_get_arg(shell, tok->value);
			if ((fd = open(tmp->input_file, O_RDONLY)) < 0)
				printf("error mila amboarina free\n");
			close(fd);
		}
		else if (tok->type == OUTFILE)
		{
			tok = tok->next;
			if (!tok)
				return (NULL);
			if (tmp->output_file)
				ft_free(tmp->output_file);
			tmp->output_file = ft_get_arg(shell, tok->value);
			if ((fd = open(ft_get_arg(shell, tmp->output_file),
						O_WRONLY | O_CREAT | O_TRUNC, 0644)) < 0)
				printf("error mila amboarina free\n");
			close(fd);
		}
		else if (tok->type == APPEND)
		{
			tmp->append = TRUE;
			tok = tok->next;
			if (!tok)
				return (NULL);
			if (tmp->output_file)
				ft_free(tmp->output_file);
			tmp->output_file = ft_get_arg(shell, tok->value);
			if ((fd = open(ft_get_arg(shell, tmp->output_file),
						O_WRONLY | O_CREAT, 0644)) < 0)
				printf("error mila amboarina free\n");
			close(fd);
		}
		else if (tok->type == HEREDOC)
		{
			tok = tok->next;
			if (!tok)
				return (NULL);
			if (!tmp->hdoc)
				return (NULL);
			if (!ft_strchr(tok->value, '\'') && !ft_strchr(tok->value, '"'))
				tmp->hdoc->expanded = TRUE;
			tmp->hdoc->del = ft_get_arg(shell, tok->value);
			if (tmp->input_file)
				ft_free(tmp->input_file);
		}
		else if (tok->type == PIPE)
		{
			if (tok->next && tok->next->type == PIPE)
			{
				perror("syntax error near unexpected token `|'");
				break ;
			}
			tmp->next = init_cmd(tmp->next);
			tmp = tmp->next;
			n++;
		}
		else
		{
			printf("ERROR NGEZA BE\n");
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
	t_cmd	*cmd_list;
	t_token	*tok;
	char	*expand;

	cmd_list = NULL;
	tok = NULL;
	if (count_quotes(input))
	{
		panic("Error numbers of quotes"); // mila amboarina ny message d'erreur
		ft_free_env(&shell->envp);
		ft_free(input);
		exit(EXIT_FAILURE);
	}
	expand = ft_expand(shell, input);
	if (!expand)
		return (NULL);
	tok = lexer_input(expand);
	if (!tok)
		return (NULL);
	cmd_list = parse_into_cmd(shell, tok);
	if (!cmd_list)
		return (NULL);
	ft_free_token(tok);
	return (cmd_list);
}
