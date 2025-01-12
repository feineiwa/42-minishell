/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memchr.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 09:49:02 by frahenin          #+#    #+#             */
/*   Updated: 2024/12/31 10:30:52 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memchr(const void *s, int c, size_t n)
{
	unsigned char	*str;
	unsigned char	car;
	size_t			i;

	str = (unsigned char *) s;
	car = (unsigned char) c;
	i = 0;
	while (i < n)
	{
		if (str[i] == car)
			return ((void *)(str + i));
		i++;
	}
	return (NULL);
}
