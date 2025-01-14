/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 14:44:40 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/11 23:10:41 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
		while (s[i] && ft_isalnum(s[i]))
			i++;
	}
	return (i);
}

char	*extract_var(char *s, t_shell *shell)
{
	int		i;
	int		len;
	char	*key;
	char	*value;

	if (!s || !shell->envp)
		return (NULL);
	i = 0;
	value = NULL;
	key = NULL;
	len = ft_strlen_expand(s);
	if (len <= 1 || s[0] != '$')
		return (NULL);
	key = malloc(sizeof(char) * (len + 1));
	if (!key)
		return (NULL);
	while (i < len)
	{
		key[i] = s[i];
		i++;
	}
	key[i] = '\0';
	value = ft_strdup(ft_get_env_value(shell->envp, key));
	if (!value)
	{
		ft_free(key);
		return (NULL);
	}
	ft_free(key);
	return (value);
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
	ft_free(s1);
	ft_free(s2);
	str[j] = 0;
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

t_bool	ft_is_expanded(char *s, int i)
{
	if (!s)
		return (FALSE);
	if (s[i] == '$')
	{
		if ((ft_is_between(s, i) == '"' || !ft_is_between(s, i))
			&& (ft_isalnum(s[i + 1])))
			return (TRUE);
		else if (!ft_is_between(s, i) && (ft_is_quote(*s + 1)))
			return (TRUE);
	}
	return (FALSE);
}

char	*ft_expand(t_shell *shell, char *s)
{
	int		i;
	char	*expanded;
	char	*value;
	int		start;

	if (!shell->envp || !s)
		return (NULL);
	i = 0;
	expanded = ft_strdup("");
	start = 0;
	while (s[i])
	{
		if (s[i] == '$' && (!ft_is_between(s, i)) && ft_is_quote(s[i + 1]))
		{
			expanded = ft_strjoin_free(expanded, ft_strndup(s + start, i - start));
			i++;
			start = i;
		}
		else if (s[i] == '$' && s[i + 1] == '?')
		{
			expanded = ft_strjoin_free(expanded, ft_strndup(s + start, i - start));
			value = ft_itoa(shell->exit_status);
			if (value)
				expanded = ft_strjoin_free(expanded, value);
			i += 2;
			start = i;
		}
		else if (ft_is_expanded(s, i))
		{
			expanded = ft_strjoin_free(expanded, ft_strndup(s + start, i - start));
			value = extract_var(s + i, shell);
			if (value)
				expanded = ft_strjoin_free(expanded, value);
			else
				expanded = ft_strjoin(expanded, "");
			i += ft_strlen_expand(s + i);
			start = i;
		}
		else
			i++;
	}
	expanded = ft_strjoin_free(expanded, ft_strdup(s + start));
	return (expanded);
}
