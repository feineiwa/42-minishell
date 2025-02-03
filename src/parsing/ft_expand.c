/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:44:40 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/03 18:46:44 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	ft_strlen_expand(char *s)
{
	int	i;

	i = 0;
	if (!s)
		return (0);
	if (s[i] == '$')
	{
		i++;
		if (ft_isdigit(s[i]))
		{
			i++;
			return (i);
		}
		while (s[i] && (ft_isalnum(s[i]) || s[i] == '_'))
			i++;
	}
	return (i);
}

void	format_value_for_quotes(char *s, int *i, int *j, char **value)
{
	char	spec[1];
	char	*tmp;

	spec[1] = '\0';
	if (s[*i] == '\'')
	{
		spec[0] = s[*i];
		*value = ft_strjoin_free(*value, ft_substr(s, *j, *i - *j));
		tmp = ft_strjoin3("\"", spec, "\"");
		*value = ft_strjoin_free(*value, tmp);
		*j = *i + 1;
	}
	else if (s[*i] == '"')
	{
		spec[0] = s[*i];
		*value = ft_strjoin_free(*value, ft_substr(s, *j, *i - *j));
		tmp = ft_strjoin3("'", spec, "'");
		*value = ft_strjoin_free(*value, tmp);
		*j = *i + 1;
	}
}

char	*format_value(char *s)
{
	char	*value;
	char	spec[1];
	char	*tmp;
	int		i;
	int		j;

	value = ft_strdup("");
	i = -1;
	j = 0;
	spec[1] = '\0';
	if (!s)
		return (value);
	while (s[++i])
	{
		if (ft_is_belong(s[i]))
		{
			spec[0] = ft_is_belong(s[i]);
			value = ft_strjoin_free(value, ft_substr(s, j, i - j));
			tmp = ft_strjoin3("'", spec, "'");
			value = ft_strjoin_free(value, tmp);
			j = i + 1;
		}
		else if (s[i] == '\'' || s[i] == '"')
			format_value_for_quotes(s, &i, &j, &value);
	}
	value = ft_strjoin_free(value, ft_strdup(s + j));
	return (ft_free(s), value);
}

char	*extract_var(char *s, t_shell *shell)
{
	int		i;
	int		len;
	char	*key;
	char	*value;

	if (!s || !shell->envp)
		return (NULL);
	i = -1;
	value = NULL;
	key = NULL;
	len = ft_strlen_expand(s);
	if (len <= 1 || s[0] != '$')
		return (NULL);
	key = malloc(sizeof(char) * (len + 1));
	if (!key)
		return (NULL);
	while (++i < len)
		key[i] = s[i];
	key[i] = '\0';
	value = ft_strdup(ft_get_env_value(shell->envp, key));
	if (!value)
		return (ft_free(key), NULL);
	return (ft_free(key), value);
}

char	*ft_strjoin_s1(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s2)
		return (NULL);
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
	while (s1[i])
		str[j++] = s1[i++];
	i = 0;
	while (s2[i])
		str[j++] = s2[i++];
	str[j] = 0;
	ft_free(s1);
	return (str);
}

char	*ft_strjoin_free(char *s1, char *s2)
{
	char	*str;
	int		i;
	int		j;

	i = 0;
	j = 0;
	if (!s2)
		return (NULL);
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + 1));
	if (!str)
		return (NULL);
	while (s1[i])
		str[j++] = s1[i++];
	i = 0;
	while (s2[i])
		str[j++] = s2[i++];
	str[j] = 0;
	ft_free(s1);
	ft_free(s2);
	return (str);
}

t_bool	ft_is_special(char c)
{
	if (c == '\'')
		return (1);
	else if (c == '"')
		return (1);
	return (0);
}

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
		value = format_value(value);
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
		if (s[i] == '$' && (s[i + 1] && (ft_isalnum(s[i + 1]) || s[i
					+ 1] == '_')))
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
