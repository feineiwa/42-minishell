/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/04 15:53:42 by frahenin          #+#    #+#             */
/*   Updated: 2024/12/31 10:31:37 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_init_vars(size_t *i, size_t *j, int *index_w)
{
	*i = 0;
	*j = 0;
	*index_w = -1;
}

static void	*ft_free(char **str, int count)
{
	int	i;

	i = 0;
	while (i < count)
	{
		free(str[i]);
		i++;
	}
	free(str);
	return (NULL);
}

static char	*ft_getwords(const char *s, int start, int end)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc((end - start + 1) * sizeof(char));
	if (!word)
		return (NULL);
	while (start < end)
		word[i++] = s[start++];
	word[i] = '\0';
	return (word);
}

static size_t	count_word(const char *s, char c)
{
	int	count;
	int	x;

	x = 0;
	count = 0;
	while (*s)
	{
		if (*s != c && x == 0)
		{
			x = 1;
			count++;
		}
		else if (*s == c)
			x = 0;
		s++;
	}
	return (count);
}

char	**ft_split(const char *s, char c)
{
	char	**tab;
	size_t	i;
	size_t	j;
	int		index_w;

	tab = (char **)malloc((count_word(s, c) + 1) * sizeof(char *));
	if (!tab)
		return (0);
	ft_init_vars(&i, &j, &index_w);
	while (i <= ft_strlen(s))
	{
		if (s[i] != c && index_w < 0)
			index_w = i;
		else if ((s[i] == c || i == ft_strlen(s)) && index_w >= 0)
		{
			tab[j] = ft_getwords(s, index_w, i);
			if (!(tab[j]))
				return (ft_free(tab, j));
			j++;
			index_w = -1;
		}
		i++;
	}
	tab[j] = 0;
	return (tab);
}
