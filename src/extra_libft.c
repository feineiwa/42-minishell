/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra_libft.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 13:56:42 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/22 17:55:00 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_isspace(char c)
{
	return ((c == 32 || (c >= 7 && c <= 13)));
}

int	ft_cmdsize(t_cmd *cmd)
{
	t_cmd	*tmp;
	int	size;

	tmp = cmd;
	size = 0;
	while (tmp)
	{
		size++;
		tmp = tmp->next;
	}
	return (size);
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

char	*ft_strndup(char *str, size_t n)
{
	char	*s;
	size_t	i;

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

int	ft_strcmp(char *s1, char *s2)
{
	while (*s1 && *s2 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}
	return (*s2 - *s1);
}

void	*ft_realloc(void *ptr, size_t old_size, size_t new_size)
{
	void	*new_ptr;

	if (new_size == 0)
	{
		ft_free(ptr);
		return (NULL);
	}
	if (!ptr)
		return (malloc(new_size));
	new_ptr = malloc(new_size);
	if (!new_ptr)
		return (NULL);
	ft_memcpy(new_ptr, ptr, old_size);
	ft_free(ptr);
	return (new_ptr);
}

char	*ft_strjoin3(char *s1, char *s2, char *s3)
{
	char	*str;
	int		i;
	int		j;

	j = 0;
	if (!s2 || !s1 || !s3)
		return (NULL);
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3)
				+ 1));
	if (!str)
		return (NULL);
	i = 0;
	while (s1[i])
		str[j++] = s1[i++];
	i = 0;
	while (s2[i])
		str[j++] = s2[i++];
	i = 0;
	while (s3[i])
		str[j++] = s3[i++];
	str[j] = 0;
	return (str);
}

char	*ft_strjoin3_free(char *s1, char *s2, char *s3)
{
	char	*str;
	int		i;
	int		j;

	j = 0;
	if (!s2 || !s1 || !s3)
		return (NULL);
	str = malloc(sizeof(char) * (ft_strlen(s1) + ft_strlen(s2) + ft_strlen(s3)
				+ 1));
	if (!str)
		return (NULL);
	i = 0;
	while (s1[i])
		str[j++] = s1[i++];
	i = 0;
	while (s2[i])
		str[j++] = s2[i++];
	i = 0;
	while (s3[i])
		str[j++] = s3[i++];
	ft_free(s1);
	ft_free(s2);
	str[j] = 0;
	return (str);
}
