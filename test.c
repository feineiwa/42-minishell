/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 12:18:07 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/04 12:36:44 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "stdio.h"
#include "unistd.h"

int	ft_is_after_equal(char *s, int i)
{
	int		j;
	char	quote;

	j = 0;
	if (!s)
		return (0);
	while (j <= i)
	{
		if (s[j] == '=')
		{
			j += 1;
			while (s[j] && s[j] == ' ')
				j++;
			if (s[j] == '"')
			{
				quote = s[j++];
				while (s[j] && s[j] != quote && j < i)
					j++;
				if (s[j] == quote)
					j++;
			}
			else
				while (s[j] && s[i] == ' ' && j < i)
					j++;
			return (j == i);
		}
		j++;
	}
	return (0);
}

// char	*add_double_quotes(char *s)
// {
// 	char	*value;

// 	value = NULL;
// 	value = ft_strjoin3("\"", s, "\"");
// 	if (!value)
// 		return (NULL);
// 	ft_free(s);
// 	return (value);
// }

// static void	expand_variable(char *s, int *start, int *i, char **expanded)
// {
// 	char	*value;

// 	value = NULL;
// 	*expanded = ft_strjoin_free(*expanded, ft_strndup(s + *start, *i - *start));
// 	value = extract_var(s + *i, g_global()->shell);
// 	if ((ft_is_between(s, *i) != '"'))
// 	{
// 		if (ft_is_after_equal(s, *i))
// 			value = add_double_quotes(value);
// 		else
// 			value = format_value(value);
// 	}
// 	if (value)
// 		*expanded = ft_strjoin_free(*expanded, value);
// 	else
// 		*expanded = ft_strjoin_s1(*expanded, "");
// 	*i += ft_strlen_expand(s + *i);
// 	*start = *i;
// }

int	main(void)
{
	printf("[%d]\n", ft_is_after_equal("faneva dia=mihina", 10));
}