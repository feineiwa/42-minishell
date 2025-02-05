/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_parse_3.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/05 10:59:13 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 15:13:26 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	set_error_file(t_cmd **tmp, char *file, int flag, int *error_flag)
{
	(*tmp)->error_file = ft_strdup(file);
	(*tmp)->flag_err = flag;
	*error_flag = 1;
}

char	*ft_get_arg(t_shell *shell, char *tok)
{
	char	*arg;
	int		i;
	int		j;

	if (!shell->envp || !tok)
		return (NULL);
	arg = ft_calloc(sizeof(char), ft_strlen_skip_quote(tok) + 1);
	if (!arg)
		return (NULL);
	i = 0;
	j = 0;
	while (tok[i])
	{
		if (ft_is_quote(tok[i]) && skip_quotes(tok, arg, &i, &j))
			continue ;
		arg[j++] = tok[i++];
	}
	arg[j] = 0;
	return (arg);
}

int	handle_pipeline(t_cmd **tmp, t_token **tok, int *error_flag,
		t_cmd *cmd_list)
{
	if (!(cmd_list->argv[0] || cmd_list->error_file || cmd_list->hdoc
			|| cmd_list->input_file || cmd_list->output_file))
	{
		write(STDERR_FILENO, "syntax error near unexpected token '|'\n", 40);
		ft_free_cmd(&cmd_list);
		return (0);
	}
	if (synthax_error(*(tok)))
	{
		write(STDERR_FILENO, "syntax error near unexpected token '|'\n", 40);
		ft_free_cmd(&cmd_list);
		return (0);
	}
	(*tmp)->next = init_cmd((*tmp)->next);
	(*tmp) = (*tmp)->next;
	*error_flag = 0;
	return (1);
}
