/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strlcat.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:21:16 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 15:10:44 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

size_t	ft_strlcat(char *dst, const char *src, size_t dst_size)
{
	size_t	len_src;
	size_t	len_dst;
	size_t	i;
	size_t	ans;

	i = 0;
	ans = 0;
	len_src = ft_strlen(src);
	len_dst = ft_strlen(dst);
	if (dst_size > len_dst)
		ans = len_src + len_dst;
	else
		ans = len_src + dst_size;
	while (src[i] != 0 && (len_dst + 1 + i) < dst_size)
	{
		dst[len_dst + i] = src[i];
		i++;
	}
	dst[len_dst + i] = '\0';
	return (ans);
}
