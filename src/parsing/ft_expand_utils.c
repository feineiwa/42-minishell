/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand_utils.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 17:27:26 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/05 10:03:21 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static t_bool	after_here_condition(char *s, int j)
{
	if (!s)
		return (FALSE);
	if ((s[j] && s[j] == '<') && (s[j + 1] && s[j + 1] == '<')
		&& (!ft_is_between(s, j)))
		return (TRUE);
	return (FALSE);
}

int	ft_is_after_here(char *s, int i)
{
	int		j;
	char	quote;

	j = 0;
	while (j <= i)
	{
		if (after_here_condition(s, j))
		{
			j += 2 + ft_skip_space(s + j + 2);
			if (ft_is_quote(s[j]))
			{
				quote = s[j++];
				while (s[j] && s[j] != quote && j < i)
					j++;
				if (s[j] == quote)
					j++;
			}
			else
				while (s[j] && !ft_isspace(s[j]) && j < i)
					j++;
			return (j == i);
		}
		j++;
	}
	return (0);
}

t_bool	ft_is_expanded(char *s, int i)
{
	if (!s)
		return (FALSE);
	if (s[i] == '$')
	{
		if ((ft_is_between(s, i) == '"' || !ft_is_between(s, i))
			&& (ft_isalnum(s[i + 1])) && !ft_is_after_here(s, i))
			return (TRUE);
		else if (!ft_is_between(s, i) && (ft_is_quote(*s + 1)))
			return (TRUE);
	}
	return (FALSE);
}

static void	ft_is_quote_after_equal(char *s, int *i, int *j)
{
	char	quote;

	quote = s[*j];
	(*j)++;
	while (s[*j] && s[*j] != quote && *j < *i)
		(*j)++;
	if (s[*j] == quote)
		(*j)++;
}

int	ft_is_after_equal(char *s, int i)
{
	int	j;

	j = 0;
	if (!s)
		return (0);
	while (j <= i)
	{
		if (s[j] && s[j] == '=')
		{
			j += 1 + ft_skip_space(s + j + 1);
			if (ft_is_quote(s[i]))
				ft_is_quote_after_equal(s, &i, &j);
			else
				while (s[j] && ft_isspace(s[i]) && j < i)
					j++;
			return (j == i);
		}
		j++;
	}
	return (0);
}
