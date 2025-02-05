/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:33:52 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 15:10:06 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memmove(void *dst, const void *src, size_t len)
{
	unsigned char	*dst_tmp;
	unsigned char	*src_tmp;
	size_t			i;

	if (!dst && !src)
		return (NULL);
	dst_tmp = (unsigned char *) dst;
	src_tmp = (unsigned char *) src;
	i = 0;
	if (dst_tmp > src_tmp)
	{
		while (len-- > 0)
		{
			*(dst_tmp + len) = *(src_tmp + len);
		}
	}
	else
	{
		while (i < len)
		{
			dst_tmp[i] = src_tmp[i];
			i++;
		}
	}
	return (dst);
}
