/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_env.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/03 09:50:41 by frahenin          #+#    #+#             */
/*   Updated: 2025/01/07 17:08:57 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

int	ft_free(void *ptr)
{
	if (!ptr)
		return (0);
	free(ptr);
	return (1);
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
	int		j;
	t_env	*current;
	t_env	*last;

	i = 0;
	j = 0;
	key = NULL;
	i = ft_search_equ(arg);
	key = ft_substr(arg, 0, i);
	if (!key)
		return ;
	current = *envp;
	while (current)
	{
		if (!ft_strcmp(current->key, key))
			break ;
		current = current->next;
	}
	if (current)
	{
		ft_free(current->value);
		ft_free(key);
		current->value = ft_strdup(arg + i + 1);
	}
	else
	{
		last = ft_get_last_env(*envp);
		last = malloc(sizeof(t_env));
		last->key = key;
		last->value = ft_strdup(arg + i + 1);
		last->next = NULL;
	}
	// printf("[%s]", ft_get_last_env(*envp)->key);
}

char	*ft_get_env_value(t_env *envp, char *key)
{
	if (key[0] != '$')
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
			tmp = tmp = tmp->next;
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
