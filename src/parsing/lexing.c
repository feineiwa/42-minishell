/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   lexing.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:07:37 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 17:39:44 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static int	skip_quotes_strtok(char *saved_str, size_t *index, int *i,
		char *quote)
{
	if (ft_is_quote(saved_str[*index + *i]))
	{
		*quote = ft_is_quote(saved_str[*index + *i]);
		(*i)++;
		while (saved_str[*index + *i] && *quote)
		{
			if (ft_is_quote(saved_str[*index + *i]) == *quote)
			{
				*quote = 0;
			}
			(*i)++;
		}
		return (1);
	}
	return (0);
}

static char	*handle_first_case(char *saved_str, size_t *index, int *i,
		char *quote)
{
	int		j;
	char	*token;

	j = *i;
	token = NULL;
	while (saved_str[*index + j])
	{
		if ((ft_is_belong(saved_str[*index + j]) || \
			ft_isspace(saved_str[*index + j])) \
				&& !ft_is_between(saved_str, *index + j))
			break ;
		if (skip_quotes_strtok(saved_str, index, &j, quote))
			continue ;
		j++;
	}
	token = ft_strndup(saved_str + *index, j);
	*index += j;
	return (token);
}

static char	*handle_second_case(char *saved_str, size_t *index, int *i)
{
	int		j;
	char	*token;

	j = *i;
	token = NULL;
	if (ft_is_belong(saved_str[*index + j]) == '|')
	{
		j++;
		token = ft_strndup(saved_str + *index, j);
		*index += j;
		return (token);
	}
	while (ft_is_belong(saved_str[*index + j]))
	{
		if (saved_str[*index + j] == '|')
			break ;
		j++;
	}
	token = ft_strndup(saved_str + *index, j);
	*index += j;
	return (token);
}

static char	*ft_strtok_quoted(char *str)
{
	static char		*saved_str = NULL;
	static size_t	index = 0;
	char			quote;
	int				i;

	if (str)
		saved_str = str;
	if (!saved_str)
		return (NULL);
	i = 0;
	quote = 0;
	index += ft_skip_space(saved_str + index);
	if (!saved_str[index])
	{
		saved_str = NULL;
		index = 0;
		return (NULL);
	}
	i = 0;
	if (!(ft_is_belong(saved_str[index + i])))
		return (handle_first_case(saved_str, &index, &i, &quote));
	else
		return (handle_second_case(saved_str, &index, &i));
	return (NULL);
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
