/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:21:53 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/05 18:29:21 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	panic(char *s)
{
	perror(s);
}

int	count_quotes(char *input)
{
	size_t	i;
	size_t	sq;
	size_t	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (input[i])
	{
		if (input[i] == 39)
			sq++;
		if (input[i] == 34)
			dq++;
		i++;
	}
	if (sq % 2 != 0 || dq % 2 != 0)
		return (1);
	return (0);
}

int	ft_isspace(char c)
{
	return ((c == 32 || (c >= 7 && c <= 13)));
}

// int	ft_count_token(char *str)
// {
// 	int		count;
// 	int		i;
// 	char	quote;

// 	count = 0;
// 	i = 0;
// 	while (str[i])
// 	{
// 		while (ft_isspace(str[i]))
// 			i++;
// 		if (str[i] == '\0')
// 			break ;
// 		if (str[i] == '\'' || str[i] == '\"')
// 		{
// 			quote = str[i++];
// 			while (str[i] && str[i] != quote)
// 				i++;
// 			if (str[i] == quote)
// 				i++;
// 		}
// 		else
// 		{
// 			while (str[i] && !ft_isspace(str[i]) && str[i] != '\''
// 				&& str[i] != '\"')
// 				i++;
// 		}
// 		count++;
// 	}
// 	// printf("count is %d\n", count);
// 	return (count);
// }

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
		if (str[i] == '\"' || str[i] == '\'')
		{
			open = i;
			quote = str[i++];
			while (str[i] && str[i] != quote)
				i++;
			if (str[i] == quote)
				close = i;
			if (index >= open && index <= close)
				return (1);
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
	char			del;

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
		if (!ft_isspace(saved_str[index + i]) || !ft_is_belong(saved_str[index
				+ i]))
		{
			while (saved_str[index + i] && (!ft_is_belong(saved_str[index + i])
					&& !ft_isspace(saved_str[index + i])))
			{
				i++;
				if (ft_is_belong(saved_str[index + i])
					&& ft_is_between(saved_str, index + i))
					i++;
			}
		}
		token = ft_strndup(saved_str + index, i);
		index += i;
	}
	else
	{
		if (ft_is_belong(saved_str[index + i]))
		{
			del = ft_is_belong(saved_str[index + i]);
			i++;
			if (ft_is_belong(saved_str[index + i]) == del && (del == '<'
					|| del == '>'))
				i++;
			token = ft_strndup(saved_str + index, i);
			index += i;
			return (token);
		}
		while (saved_str[index + i] && (!ft_is_belong(saved_str[index + i])
				&& !ft_isspace(saved_str[index + i])))
		{
			i++;
			if (ft_is_belong(saved_str[index + i]) && ft_is_between(saved_str,
					index + i))
				i++;
		}
		token = ft_strndup(saved_str + index, i);
		index += i;
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
	// while (tok)
	// {
		
	// 	tok = tok->next;
	// }
	return (cmd_list);
}

t_cmd	*parsing(t_shell *shell, char *input)
{
	t_cmd	*cmd_list;
	t_token	*tok;

	tok = lexer_input(input);
	if (!tok)
		return (NULL);
	cmd_list = parse_input(tok);
	if (!cmd_list)
		return (NULL);
	return (cmd_list);
}

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	char	*input;

	(void)ac;
	(void)av;
	shell = init_shell(envp);
	while (42)
	{
		input = readline(PROMPT);
		if (input == NULL)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (input && *input)
		{
			add_history(input);
			shell.cmd = parsing(&shell, input);
			// if (shell.cmd)
			// {
			// 	execute_command(shell.cmd, shell);
			// 	free_command(shell.cmd);
			// }
		}
		free(input);
	}
	ft_lstclear(&shell.envp, free);
	return (0);
}
