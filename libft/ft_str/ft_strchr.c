/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 07:32:51 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 15:10:31 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strchr(const char *s, int c)
{
	unsigned int	i;
	char			to_find;

	i = 0;
	to_find = (char) c;
	while (s[i])
	{
		if (s[i] == to_find)
			return ((char *)(s + i));
		i++;
	}
	if (s[i] == to_find)
		return ((char *) &s[i]);
	return (NULL);
}
