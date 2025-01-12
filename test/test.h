/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   test.h                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/17 09:38:08 by frahenin          #+#    #+#             */
/*   Updated: 2024/12/26 16:09:05 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef TEST_H
# define TEST_H

# include <stdio.h>
# include <unistd.h>
# include <stdlib.h>

# define BLACK "\033[30m"
# define RED "\033[31m"
# define GREEN "\033[32m"
# define YELLOW "\033[33m"
# define BLUE "\033[34m"
# define MAGENTA "\033[35m"
# define CYAN "\033[36m"
# define WHITE "\033[37m"
# define RESET	"\033[0m"

void	describe_group(char *msg);
void	describe_test(char *msg);
int		should_be_ptr(char *msg, void *ptr1, void *ptr2);
int		should_be_str(char *msg, char *s1, char *s2);
void	describe_test(char *msg);
void	describe_group(char *msg);
int	should_be_int(char *msg, int a, int b);

#endif