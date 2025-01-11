/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 11:35:59 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/11 14:09:37 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	panic(char *s)
{
	perror(s);
	exit(EXIT_FAILURE);
}

int	ft_fork(void)
{
	int	pid;

	pid = fork();
	if (pid == -1)
		panic("fork");
	return (pid);
}

int	count_quotes(char *input)
{
	size_t	i;
	char	quote;

	if (!input)
		return (0);
	quote = 0;
	i = 0;
	while (input[i])
	{
		if (ft_is_quote(input[i]))
		{
			quote = ft_is_quote(input[i]);
			i++;
			while (input[i] && quote)
			{
				if (ft_is_quote(input[i]) == quote)
					quote = 0;
				i++;
			}
			continue ;
		}
		i++;
	}
	if (quote)
		return (1);
	return (0);
}

int	ft_isspace(char c)
{
	return ((c == 32 || (c >= 7 && c <= 13)));
}

int	ft_is_between(char *str, int index)
{
	int		i;
	char	quote;
	int		open;
	int		close;

	i = 0;
	open = 0;
	close = 0;
	while (str[i])
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			open = i;
			quote = str[i++];
			while (str[i] && str[i] != quote)
				i++;
			if (str[i] == quote)
				close = i;
			if (index >= open && index <= close)
				return (quote);
		}
		i++;
	}
	return (0);
}

int	ft_skip_space(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	return (i);
}

int	ft_is_quote(char c)
{
	if (c == 39 || c == 34)
		return (c);
	return (0);
}

int	ft_is_belong(char c)
{
	if (c == '>')
		return (c);
	else if (c == '<')
		return (c);
	else if (c == '|')
		return (c);
	return (0);
}

char	*ft_strndup(char *str, size_t n)
{
	char	*s;
	int		i;

	if (!str)
		return (NULL);
	s = malloc(sizeof(char) * n + 1);
	if (!s)
		return (NULL);
	i = 0;
	while (i < n)
	{
		s[i] = str[i];
		i++;
	}
	s[i] = 0;
	return (s);
}

char	*ft_strtok_quoted(char *str)
{
	static char		*saved_str = NULL;
	char			*token;
	static size_t	index = 0;
	char			quote;
	int				i;

	if (str)
		saved_str = str;
	if (!saved_str)
		return (NULL);
	i = 0;
	quote = 0;
	token = NULL;
	index += ft_skip_space(saved_str + index);
	if (!saved_str[index])
	{
		saved_str = NULL;
		index = 0;
		return (NULL);
	}
	i = 0;
	if (!(ft_is_belong(saved_str[index + i])))
	{
		while (saved_str[index + i])
		{
			if ((ft_is_belong(saved_str[index + i])
					|| ft_isspace(saved_str[index + i]))
				&& !ft_is_between(saved_str, index + i))
				break ;
			if (ft_is_quote(saved_str[index + i]))
			{
				quote = ft_is_quote(saved_str[index + i]);
				i++;
				while (saved_str[index + i] && quote)
				{
					if (ft_is_quote(saved_str[index + i]) == quote)
						quote = 0;
					i++;
				}
				continue ;
			}
			i++;
		}
		token = ft_strndup(saved_str + index, i);
		index += i;
	}
	else
	{
		while (ft_is_belong(saved_str[index + i]))
			i++;
		token = ft_strndup(saved_str + index, i);
		index += i;
		return (token);
	}
	return (token);
}

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*s2 - *s1);
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

int	ft_strlen_skip_quote(char *s)
{
	int		i;
	char	quote;
	int		len;

	i = 0;
	len = 0;
	quote = 0;
	while (s[i])
	{
		if (ft_is_quote(s[i]))
		{
			quote = ft_is_quote(s[i]);
			i++;
			while (s[i] && quote)
			{
				if (ft_is_quote(s[i]) == quote)
				{
					quote = 0;
					i++;
					continue ;
				}
				len++;
				i++;
			}
			if (s[i] == '\0')
				break ;
			continue ;
		}
		i++;
		len++;
	}
	return (len);
}

char	*ft_get_arg(t_shell *shell, char *tok)
{
	char	*arg;
	char	quote;
	char	*s;
	int		i;
	int		j;
	int		q;

	s = NULL;
	if (!shell->envp || !tok)
		return (NULL);
	s = ft_expand(shell, tok);
	if (!s)
		return (NULL);
	arg = ft_calloc(sizeof(char), ft_strlen_skip_quote(s) + 1);
	if (!arg)
		return (NULL);
	i = 0;
	j = 0;
	quote = 0;
	while (s[i])
	{
		if (ft_is_quote(s[i]))
		{
			quote = ft_is_quote(s[i]);
			i++;
			while (s[i] && quote)
			{
				if (ft_is_quote(s[i]) == quote)
				{
					quote = 0;
					i++;
					continue ;
				}
				arg[j++] = s[i++];
			}
			if (s[i] == '\0')
				break ;
			continue ;
		}
		arg[j++] = s[i++];
	}
	arg[j] = 0;
	return (arg);
}

void	*ft_realloc(void *ptr, size_t new_size)
{
	void	*new_ptr;

	if (new_size == 0)
	{
		ft_free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (malloc(new_size));
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, new_size);
	ft_free(ptr);
	return (new_ptr);
}

t_cmd	*init_cmd(t_cmd *cmd)
{
	cmd = (t_cmd *)malloc(sizeof(t_cmd));
	cmd->argc = 0;
	cmd->argv = NULL;
	cmd->append = FALSE;
	cmd->input_file = NULL;
	cmd->output_file = NULL;
	cmd->hdoc = (t_hdoc *)malloc(sizeof(t_hdoc));
	cmd->hdoc->del = NULL;
	cmd->hdoc->expanded = FALSE;
	cmd->next = NULL;
	return (cmd);
}

// t_token	*ft_refract_token(t_shell *shell, t_token *tok)
// {
// 	t_token	*tmp;
// 	char	*arg;

// 	if (!shell || !tok)
// 		return (NULL);
// 	tmp = tok;
// 	arg = NULL;
// 	while (tmp)
// 	{
// 		arg = ft_get_arg(shell, tmp->value);
// 		if (!arg)
// 			return (NULL);
// 		ft_free(tmp->value);
// 		tmp->value = arg;
// 		if (ft_strcmp("<<", arg) && tmp->next && (ft_strchr(tmp->next->value,
// 					'"') || ft_strchr(tmp->next->value, '\'')))
// 		{

// 			tmp = tmp->next;
// 			arg = ft_get_arg(shell, tmp->value);
// 			if (!arg)
// 				return (NULL);
// 			ft_free(tmp->value);
// 			tmp->value = arg;
// 		}
// 		tmp = tmp->next;
// 	}
// 	return (tok);
// }

t_cmd	*parse_into_cmd(t_shell *shell, t_token *tok)
{
	t_cmd	*tmp;
	t_cmd	**cmd_list;
	int		i;

	tmp = NULL;
	if (!tok || !shell)
		return (NULL);
	cmd_list = malloc(sizeof(t_cmd *));
	tmp = init_cmd(tmp);
	if (!tmp)
		return (NULL);
	*cmd_list = tmp;
	i = 0;
	while (tok)
	{
		if (tok->type == ARGS)
		{
			tmp->argv = (char **)ft_realloc(tmp->argv, sizeof(char *)
					* (tmp->argc + 2));
			tmp->argv[tmp->argc] = ft_get_arg(shell, tok->value);
			tmp->argc++;
			tmp->argv[tmp->argc] = NULL;
		}
		else if (tok->type == INFILE)
		{
			tok = tok->next;
			if (!tok)
				return (NULL);
			tmp->input_file = ft_get_arg(shell, tok->value);
			if (open(tmp->input_file, O_RDONLY) < 0)
				printf("error mila amboarina free\n");
		}
		else if (tok->type == OUTFILE)
		{
			tok = tok->next;
			if (!tok)
				return (NULL);
			tmp->output_file = ft_get_arg(shell, tok->value);
			if (open(ft_get_arg(shell, tok->value),
					O_WRONLY | O_CREAT | O_TRUNC, 0644) < 0)
				printf("error mila amboarina free\n");
		}
		else if (tok->type == APPEND)
		{
			tmp->append = TRUE;
			tok = tok->next;
			if (!tok)
				return (NULL);
			tmp->output_file = ft_get_arg(shell, tok->value);
			if (open(ft_get_arg(shell, tok->value), O_WRONLY | O_CREAT,
					0644) < 0)
				printf("error mila amboarina free\n");
		}
		else if (tok->type == HEREDOC)
		{
			tok = tok->next;
			if (!tok)
				return (NULL);
			tmp->hdoc = (t_hdoc *)(malloc(sizeof(t_hdoc)));
			if (!tmp->hdoc)
				return (NULL);
			if (!ft_strchr(tok->value, '\'') && !ft_strchr(tok->value, '"'))
				tmp->hdoc->expanded = TRUE;
			tmp->hdoc->del = ft_get_arg(shell, tok->value);
		}
		else if (tok->type == PIPE)
		{
			tmp = tmp->next;
			tmp = init_cmd(tmp);
		}
		else
		{
			printf("ERROR NGEZA BE\n");
		}
		tok = tok->next;
	}
	while ((*cmd_list))
	{
		i = 0;
		while ((*cmd_list)->argv[i])
			printf("%s\n", (*cmd_list)->argv[i++]);
		printf("argc = %d\n", (*cmd_list)->argc);
		printf("input file = %s\n", (*cmd_list)->input_file);
		printf("output file = %s\n", (*cmd_list)->output_file);
		printf("APPEND = %d\n", (*cmd_list)->append);
		printf("delimiter heredoc = %s\n", (*cmd_list)->hdoc->del);
		printf("del expanded = %d\n", (*cmd_list)->hdoc->expanded);
		(*cmd_list) = (*cmd_list)->next;
		if ((*cmd_list))
		{
			printf("PIPE\n");
			printf("-----------------------------------------------\n");
		}
	}
	return ((*cmd_list));
}

t_cmd	*parsing(t_shell *shell, char *input)
{
	t_cmd	*cmd_list;
	t_token	*tok;

	if (count_quotes(input))
	{
		panic("Error numbers of quotes");
		return (NULL);
	}
	tok = lexer_input(input);
	if (!tok)
		return (NULL);
	cmd_list = parse_into_cmd(shell, tok);
	if (!cmd_list)
		return (NULL);
	// while (cmd_list)
	// {
	// 	int	i = 0;
	// 	while (cmd_list->argv[i])
	// 		printf("[%s]\n", cmd_list->argv[i++]);
	// 	cmd_list = cmd_list->next;
	// }
	return (cmd_list);
}
