/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 09:52:43 by frahenin          #+#    #+#             */
/*   Updated: 2024/12/17 10:45:20 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "test.h"

int	main()
{
	int	f;

	int	*o = &f;
	describe_group("resaka pipe 👑");
	describe_test("test momban zao ");
	should_be_int("resaka int", 5, 5);
	should_be_str("resaka str", "faneva", "faneva");
	should_be_ptr("message ity", &f, &o);
	return (0);
}