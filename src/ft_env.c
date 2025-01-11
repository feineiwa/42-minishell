/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 09:50:41 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/11 23:24:43 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	ft_free(void *ptr)
{
	if (!ptr)
		return ;
	free(ptr);
	ptr = NULL;
}

int	ft_search_equ(char *s)
{
	size_t	i;

	if (!s)
		return (-1);
	i = 0;
	while (s[i] && s[i] != '=')
		i++;
	return (i);
}

void	print_env(t_env *envp)
{
	while (envp)
	{
		ft_putstr_fd(envp->key, 1);
		ft_putchar_fd('=', 1);
		ft_putstr_fd(envp->value, 1);
		ft_putchar_fd('\n', 1);
		envp = envp->next;
	}
}

t_env	*ft_get_last_env(t_env *envp)
{
	if (!envp)
		return (NULL);
	while (envp->next)
		envp = envp->next;
	return (envp);
}

void	ft_add_env(t_env **envp, char *arg)
{
	int		i;
	char	*key;
	t_env	*current;
	t_env	*new_node;
	t_env	*last;

	i = 0;
	key = NULL;
	i = ft_search_equ(arg);
	if (i < 0 || arg[i] == '\0')
		return ;
	key = ft_substr(arg, 0, i);
	if (!key)
		return ;
	current = *envp;
	while (current)
	{
		if (!ft_strcmp(current->key, key))
		{
			ft_free(current->value);
			current->value = ft_strdup(arg + i + 1);
			ft_free(key);
			return ;
		}
		current = current->next;
	}
	new_node = ft_calloc(sizeof(t_env), 1);
	if (!new_node)
		return ;
	new_node->key = key;
	new_node->value = ft_strdup(arg + i + 1);
	new_node->next = NULL;
	if (*envp == NULL)
		*envp = new_node;
	else
	{
		last = ft_get_last_env(*envp);
		last->next = new_node;
	}
}

char	*ft_get_env_value(t_env *envp, char *key)
{
	if (key[0] != '$' || !key)
		return (NULL);
	while (envp)
	{
		if (!ft_strcmp(key + 1, envp->key))
			return (envp->value);
		envp = envp->next;
	}
	return (NULL);
}

static t_env	*populate_env_list(char **envp)
{
	t_env	*env_list;
	t_env	*new_node;
	t_env	*tmp;
	char	*key;
	char	*value;
	size_t	i;
	size_t	j;

	i = 0;
	env_list = NULL;
	while (envp[i])
	{
		j = 0;
		j += ft_search_equ(envp[i]);
		if (envp[i][j] == '=')
		{
			key = ft_substr(envp[i], 0, j);
			value = ft_strdup(envp[i] + j + 1);
		}
		new_node = ft_calloc(sizeof(t_env), 1);
		new_node->key = key;
		new_node->value = value;
		new_node->next = NULL;
		if (!env_list)
		{
			env_list = new_node;
			tmp = env_list;
		}
		else
		{
			tmp->next = new_node;
			tmp = tmp->next;
		}
		i++;
	}
	return (env_list);
}

t_shell	init_shell(char **envp)
{
	t_shell	shell;

	shell.envp = populate_env_list(envp);
	if (!shell.envp)
	{
		shell.exit_status = -1;
		shell.cmd = NULL;
		return (shell);
	}
	shell.cmd = NULL;
	shell.exit_status = 0;
	return (shell);
}

void	ft_free_env(t_env **envp)
{
	t_env	*tmp;

	if (!*envp || !envp)
		return ;
	while (*envp)
	{
		tmp = (*envp)->next;
		ft_free((*envp)->key);
		ft_free((*envp)->value);
		ft_free(*envp);
		*envp = tmp;
	}
	ft_free(*envp);
}
