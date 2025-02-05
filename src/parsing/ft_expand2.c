/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_expand2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/04 17:27:08 by nrasamim          #+#    #+#             */
/*   Updated: 2025/02/05 15:13:20 by frahenin         ###   ########.fr       */
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

static void	format_value_for_quotes(char *s, int *i, int *j, char **value)
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
			value = ft_strjoin_free(value, ft_strjoin3("'", spec, "'"));
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
