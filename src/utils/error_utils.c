/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_utils.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/29 14:36:34 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 16:46:17 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	print_synthax_error(t_cmd **cmd_list)
{
	write(STDERR_FILENO, "syntax error near unexpected token\n", 36);
	ft_free_cmd(cmd_list);
}

void	print_err(char *s1, char *s2, char *s3, int fd)
{
	if (s1)
		ft_putstr_fd(s1, fd);
	if (s2)
		ft_putstr_fd(s2, fd);
	if (s3)
		ft_putstr_fd(s3, fd);
}

t_bool	synthax_error(t_token *tok)
{
	if (!tok)
		return (TRUE);
	if (!ft_strcmp(tok->value, "<") && tok->type == INFILE)
		if (!tok->next || (tok->next && tok->next->type != ARGS))
			return (TRUE);
	if (!ft_strcmp(tok->value, "<<") && tok->type == HEREDOC)
		if ((tok->next && tok->next->type != ARGS) || (!tok->next))
			return (TRUE);
	if (!ft_strcmp(tok->value, ">") && tok->type == OUTFILE)
		if (!tok->next || (tok->next && tok->next->type != ARGS))
			return (TRUE);
	if (!ft_strcmp(tok->value, ">>") && tok->type == APPEND)
		if (!tok->next || (tok->next && tok->next->type != ARGS))
			return (TRUE);
	if (!ft_strcmp(tok->value, "|") && tok->type == PIPE)
		if (!tok->next)
			return (TRUE);
	return (FALSE);
}

int	update_pwd(t_env *envp, char *path)
{
	char	*pwd_update;

	pwd_update = ft_strjoin("PWD=", path);
	if (!pwd_update)
		return (1);
	ft_add_env(&envp, pwd_update);
	ft_free(pwd_update);
	return (0);
}

int	update_oldpwd(t_env *envp, char *old_pwd)
{
	char	*old_pwd_update;

	old_pwd_update = ft_strjoin("OLDPWD=", old_pwd);
	if (!old_pwd_update)
	{
		perror(old_pwd_update);
		return (1);
	}
	ft_add_env(&envp, old_pwd_update);
	ft_free(old_pwd_update);
	return (0);
}
