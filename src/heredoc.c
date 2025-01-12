/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   heredoc.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nrasamim <nrasamim@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/10 11:30:42 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/11 15:28:18 by nrasamim         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void  between_heredoc_and_cmd(char *end_here_doc)
{
	char    *content;
	int fd_tmp;

	fd_tmp = open(".heredoc.tmp", O_CREAT | O_WRONLY | O_TRUNC, 0644);
	if (fd_tmp == -1)
	{
		perror("minishell: create .heredoc.tmp");
		exit(1);
	}
	while (42)
	{
		content = readline("> ");
		if (content == NULL)
        {
            perror("Warning : There're not delimiter in the heredoc");
            break ;
        }
        if (!ft_strcmp(content, end_here_doc))
			break ;
        // variable d'environnement??
        write(fd_tmp, content, ft_strlen(content));
		write(fd_tmp, "\n", 1);
		free(content);
	}
	free(content);
	close(fd_tmp);
}

bool    handle_heredoc(t_cmd *cmd)
{
    int inputfd;

    between_heredoc_and_cmd(cmd->here_doc.del);
    inputfd = open(".heredoc.tmp", O_RDONLY);
    if (inputfd < 0)
    {
        perror("minishell: read .heredoc.tmp");
        unlink(".heredoc.tmp");
        return (false);
    }
    if (dup2(inputfd, STDIN_FILENO) < 0)
    {
        perror("minishell: dup2 input");
        unlink(".heredoc.tmp");
        return (false);
    }
    unlink(".heredoc.tmp");
    return (true);
}