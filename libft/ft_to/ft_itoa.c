/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 16:25:48 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 15:11:29 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

long	number_len(long nbr)
{
	int	count;

	count = 0;
	if (nbr < 0)
	{
		count++;
		nbr = -nbr;
	}
	if (nbr == 0)
		count++;
	while (nbr != 0)
	{
		nbr /= 10;
		count++;
	}
	return (count);
}

char	*ft_itoa(int n)
{
	int		len;
	char	*res;
	long	nbr;
	int		i;

	nbr = n;
	len = number_len(nbr);
	res = (char *)malloc((len + 1) * sizeof(char));
	if (!res)
		return (NULL);
	res[0] = '0';
	i = len - 1;
	if (n < 0)
	{
		res[0] = '-';
		nbr = -nbr;
	}
	while (nbr != 0)
	{
		res[i] = ((nbr % 10) + 48);
		nbr = nbr / 10;
		i--;
	}
	res[len] = 0;
	return (res);
}
