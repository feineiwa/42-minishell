/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_cat.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/08 13:22:53 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/12 20:16:48 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int ft_cat(char *filename)
{
    int fd;
    int bytesRead;
    char buffer[4026];

    if (filename)
    {
        fd = open(filename, O_RDONLY);
        if (fd == -1)
        {
            perror("Error opening file");
            return (1);
        }
    }
    else
        fd = dup(STDIN_FILENO);
    bytesRead = read(fd, buffer, sizeof(buffer));
    write(1, buffer, bytesRead);
    if (bytesRead == -1)
    {
        perror("Error reading file");
        close(fd);
        return (1);
    }
    close(fd);
    return (0);
}
