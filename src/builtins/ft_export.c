/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_export.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/13 08:29:09 by frahenin          #+#    #+#             */
/*   Updated: 2025/02/05 15:12:29 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	print_export(t_shell *shell)
{
	t_env	*tmp;
	t_env	*i;
	t_env	*j;

	i = shell->envp;
	while (i)
	{
		j = i->next;
		while (j)
		{
			tmp = shell->envp;
			ft_swap_env(i, j, tmp);
			j = j->next;
		}
		i = i->next;
	}
	tmp = shell->envp;
	while (tmp)
	{
		if (tmp->value)
			printf("declare -x %s=\"%s\"\n", tmp->key, tmp->value);
		else
			printf("declare -x %s\n", tmp->key);
		tmp = tmp->next;
	}
}

static int	ft_search_equ_add_env(char *s)
{
	size_t	i;

	if (!s)
		return (-1);
	i = 0;
	if ((!ft_isalpha(s[i]) && s[i] != '_'))
	{
		print_err("export: ", s, ": not a valid indentifier\n", 2);
		return (-1);
	}
	while (s[i] && s[i] != '=')
	{
		if (!ft_isalnum(s[i]) && s[i] != '_')
		{
			print_err("export: ", s, ": not a valid indentifier\n", 2);
			return (-1);
		}
		i++;
	}
	return (i);
}

static int	ft_replace_value(t_env **envp, char *arg, int i, char *key)
{
	t_env	*current;

	current = *envp;
	while (current)
	{
		if (!ft_strcmp(current->key, key) && arg[i] == '=')
		{
			ft_free(current->value);
			current->value = ft_strdup(arg + i + 1);
			return (ft_free(key), 0);
		}
		else if (!ft_strcmp(current->key, key) && arg[i] != '=')
			return (ft_free(key), 0);
		current = current->next;
	}
	return (1);
}

int	ft_add_env(t_env **envp, char *arg)
{
	int		i;
	char	*key;
	t_env	*new_node;

	i = ft_search_equ_add_env(arg);
	if (i < 0)
		return (1);
	key = ft_substr(arg, 0, i);
	if (!key)
		return (1);
	if (!ft_replace_value(envp, arg, i, key))
		return (0);
	if (arg[i] == '=')
	{
		new_node = add_new_env(key, ft_strdup(arg + i + 1));
		if (!new_node->value)
			new_node->value = ft_strdup("");
	}
	else
		new_node = add_new_env(key, NULL);
	if (*envp == NULL)
		*envp = new_node;
	else
		ft_get_last_env(*envp)->next = new_node;
	return (0);
}

int	ft_export(t_shell *shell, t_cmd *cmd)
{
	int	i;
	int	ret;

	i = 1;
	ret = 0;
	if (!cmd->argv[i])
		print_export(shell);
	while (cmd->argv[i])
	{
		if (ft_add_env(&shell->envp, cmd->argv[i]))
			ret = 1;
		i++;
	}
	return (ret);
}
