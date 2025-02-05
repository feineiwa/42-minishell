/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:44:40 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 18:48:44 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	expand_status(char *s, int *start, int *i, char **expanded)
{
	char	*value;

	value = NULL;
	*expanded = ft_strjoin_free(*expanded, ft_strndup(s + *start, *i - *start));
	value = ft_itoa(g_global()->exit_status);
	if (value)
		*expanded = ft_strjoin_free(*expanded, value);
	*i += 2;
	*start = *i;
}

static void	expand_variable(char *s, int *start, int *i, char **expanded)
{
	char	*value;

	value = NULL;
	*expanded = ft_strjoin_free(*expanded, ft_strndup(s + *start, *i - *start));
	value = extract_var(s + *i, g_global()->shell);
	if ((ft_is_between(s, *i) != '"'))
	{
		if (ft_is_after_equal(s, *i))
			value = add_double_quotes(value);
		else
			value = format_value(value);
	}
	if (value)
		*expanded = ft_strjoin_free(*expanded, value);
	else
		*expanded = ft_strjoin_s1(*expanded, "");
	*i += ft_strlen_expand(s + *i);
	*start = *i;
}

char	*ft_expand_for_hdoc(t_shell *shell, char *s)
{
	int		i;
	char	*expanded;
	int		start;

	if (!shell->envp || !s)
		return (NULL);
	i = 0;
	expanded = ft_strdup("");
	start = 0;
	g_global()->shell = shell;
	while (s[i])
	{
		if (s[i] == '$' && (s[i + 1] && (ft_isalnum(s[i + 1]) || \
			s[i + 1] == '_')))
			expand_variable(s, &start, &i, &expanded);
		else if (s[i] == '$' && (s[i + 1] && s[i + 1] == '?'))
			expand_status(s, &start, &i, &expanded);
		else
			i++;
	}
	return (expanded = ft_strjoin_free(expanded, ft_strdup(s + start)));
}

char	*ft_expand(t_shell *shell, char *s)
{
	int		i;
	char	*expanded;
	int		start;

	if (!shell->envp || !s)
		return (NULL);
	expanded = ft_strdup("");
	i = 0;
	start = 0;
	while (s[i])
	{
		if (s[i] == '$' && (!ft_is_between(s, i)) && ft_is_quote(s[i + 1]))
		{
			expanded = ft_strjoin_free(expanded, ft_strndup(s + start, i
						- start));
			start = ++i;
		}
		else if (s[i] == '$' && s[i + 1] == '?' && !ft_is_after_here(s, i))
			expand_status(s, &start, &i, &expanded);
		else if (ft_is_expanded(s, i))
			expand_variable(s, &start, &i, &expanded);
		else
			i++;
	}
	return (ft_strjoin_free(expanded, ft_strdup(s + start)));
}
