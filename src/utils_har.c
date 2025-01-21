/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils_har.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/30 10:28:39 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/21 17:46:39 by frahenin         ###   ########.fr       */
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

// char	*ft_strtok_quoted(char *str)
// {
// 	static char		*saved_str = NULL;
// 	char			*token;
// 	static size_t	index = 0;
// 	char			quote;
// 	int				i;
// 	char			del;

// 	if (str)
// 		saved_str = str;
// 	if (!saved_str)
// 		return (NULL);
// 	i = 0;
// 	quote = 0;
// 	token = NULL;
// 	index += ft_skip_space(saved_str + index);
// 	if (!saved_str[index])
// 	{
// 		saved_str = NULL;
// 		index = 0;
// 		return (NULL);
// 	}
// 	i = 0;
// 	if (ft_is_quote(saved_str[index + i]))
// 	{
// 		quote = ft_is_quote(saved_str[index + i]);
// 		i++;
// 		while (saved_str[index + i] && quote)
// 		{
// 			if (ft_is_quote(saved_str[index + i]) == quote)
// 				quote = 0;
// 			i++;
// 		}
// 		if (!ft_isspace(saved_str[index + i]) || !ft_is_belong(saved_str[index
// 				+ i]))
// 		{
// 			while (saved_str[index + i] && (!ft_is_belong(saved_str[index + i])
// 					&& !ft_isspace(saved_str[index + i])))
// 			{
// 				i++;
// 				if (ft_is_belong(saved_str[index + i])
// 					&& ft_is_between(saved_str, index + i))
// 					i++;
// 			}
// 		}
// 		token = ft_strndup(saved_str + index, i);
// 		index += i;
// 	}
// 	else
// 	{
// 		// else if (ft_is_belong(saved_str[index + i]))
// 		// {
// 		// 	del = ft_is_belong(saved_str[index + i]);
// 		// 	i++;
// 		// 	// if (ft_is_belong(saved_str[index + i]) == del && (del == '<'
// 		// 	// 		|| del == '>'))
// 		// 	// 	i++;
// 		// 	token = ft_strndup(saved_str + index, i);
// 		// 	index += i;
// 		// 	return (token);
// 		// }
// 		// while (saved_str[index + i] && (!ft_is_belong(saved_str[index + i])
// 		// 		&& !ft_isspace(saved_str[index + i])))
// 		// {
// 		// 	i++;
// 		// 	if (ft_is_belong(saved_str[index + i]) && ft_is_between(saved_str,
// 		// 			index + i))
// 		// 		i++;
// 		// }
// 		token = ft_strndup(saved_str + index, i);
// 		index += i;
// 	}
// 	return (token);
// }

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*s2 - *s1);
}
