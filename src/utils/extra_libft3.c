/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_libft3.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 11:29:10 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 14:44:23 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	is_only_dot_or_slash(char *s)
{
	if (!s)
		return (1);
	while (*s)
	{
		if (*s != '.' && *s != '/')
			return (0);
		s++;
	}
	return (1);
}

char	*add_double_quotes(char *s)
{
	char	*value;

	value = NULL;
	value = ft_strjoin3("\"", s, "\"");
	if (!value)
		return (NULL);
	ft_free(s);
	return (value);
}

t_bool	ft_is_special(char c)
{
	if (c == '\'')
		return (1);
	else if (c == '"')
		return (1);
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

int	ft_cmdsize(t_cmd *cmd)
{
	t_cmd	*tmp;
	int		size;

	tmp = cmd;
	size = 0;
	if (!tmp)
		return (0);
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	return (size);
}
