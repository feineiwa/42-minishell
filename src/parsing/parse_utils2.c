/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_utils2.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 17:13:21 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/08 14:11:24 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	skip_quotes_for_strlen_quote(char *s, int *i, int *len, char *quote)
{
	while (s[*i] && *quote)
	{
		if (ft_is_quote(s[*i]) == *quote)
		{
			*quote = 0;
			(*i)++;
			continue ;
		}
		if (s[*i] == '\0')
			break ;
		(*len)++;
		(*i)++;
	}
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
			skip_quotes_for_strlen_quote(s, &i, &len, &quote);
			continue ;
		}
		i++;
		len++;
	}
	return (len);
}

int	skip_quotes(char *tok, char *arg, int *i, int *j)
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

int	ft_is_between(char *str, int index)
{
	int		i;
	char	quote;
	int		open;
	int		close;

	i = -1;
	open = 0;
	close = 0;
	while (str[++i] && str)
	{
		if (str[i] == '"' || str[i] == '\'')
		{
			open = i;
			quote = str[i++];
			while (str[i] && str[i] != quote)
				i++;
			if (str[i] && str[i] == quote)
				close = i;
			if (index >= open && index <= close)
				return (quote);
			if (str[i] == '\0')
				return (0);
		}
	}
	return (0);
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
		return ((g_global()->exit_status = 1), 1);
	return (0);
}
