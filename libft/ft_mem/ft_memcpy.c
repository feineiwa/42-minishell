/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 14:28:26 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 15:10:03 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dst, const void *src, size_t n)
{
	unsigned char	*dst_tmp;
	unsigned char	*src_tmp;

	if (dst == (void *)0 && src == (void *)0)
		return (dst);
	dst_tmp = (unsigned char *) dst;
	src_tmp = (unsigned char *) src;
	while (n > 0)
	{
		*(dst_tmp++) = *(src_tmp++);
		n--;
	}
	return (dst);
}
