/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strrchr.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/02 08:11:06 by frahenin          #+#    #+#             */
/*   Updated: 2024/12/31 10:32:11 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

char	*ft_strrchr(const char *s, int c)
{
	unsigned int	i;
	char			to_find;
	char			*res;

	to_find = (char) c;
	i = 0;
	res = NULL;
	while (s[i])
	{
		if (s[i] == to_find)
			res = (char *)(s + i);
		i++;
	}
	if (s[i] == to_find)
		res = (char *)(s + i);
	return (res);
}
