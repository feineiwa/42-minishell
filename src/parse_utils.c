/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 13:16:31 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/21 22:34:09 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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

int	ft_is_quote(char c)
{
	if (c == 39 || c == 34)
		return (c);
	return (0);
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
		if (ft_is_belong(saved_str[index + i]) == '|')
		{
			i++;
			token = ft_strndup(saved_str + index, i);
			index += i;
			return (token);
		}
		while (ft_is_belong(saved_str[index + i]))
		{
			if (saved_str[index + i] == '|')
				break ;
			i++;
		}
		token = ft_strndup(saved_str + index, i);
		index += i;
		return (token);
	}
	return (token);
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
			quote = ft_is_quote(input[i++]);
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
