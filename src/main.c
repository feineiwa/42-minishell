/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:21:53 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/04 18:56:57 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// void	panic(char *s)
// {
// 	perror(s);
// }

// int	count_quotes(char *input)
// {
// 	size_t	i;
// 	size_t	sq;
// 	size_t	dq;

// 	i = 0;
// 	sq = 0;
// 	dq = 0;
// 	while (input[i])
// 	{
// 		if (input[i] == 39)
// 			sq++;
// 		if (input[i] == 34)
// 			dq++;
// 		i++;
// 	}
// 	if (sq % 2 != 0 || dq % 2 != 0)
// 		return (1);
// 	return (0);
// }

int	ft_isspace(char c)
{
	return ((c == 32 || (c >= 7 && c <= 13)));
}

// int	ft_count_token(char *str)
// {
// 	int		count;
// 	int		i;
// 	char	quote;

// 	count = 0;
// 	i = 0;
// 	while (str[i])
// 	{
// 		while (ft_isspace(str[i]))
// 			i++;
// 		if (str[i] == '\0')
// 			break ;
// 		if (str[i] == '\'' || str[i] == '\"')
// 		{
// 			quote = str[i++];
// 			while (str[i] && str[i] != quote)
// 				i++;
// 			if (str[i] == quote)
// 				i++;
// 		}
// 		else
// 		{
// 			while (str[i] && !ft_isspace(str[i]) && str[i] != '\''
// 				&& str[i] != '\"')
// 				i++;
// 		}
// 		count++;
// 	}
// 	// printf("count is %d\n", count);
// 	return (count);
// }

// int	ft_is_between(char *str, int index)
// {
// 	int		i;
// 	char	quote;
// 	int		open;
// 	int		close;

// 	i = 0;
// 	open = 0;
// 	close = 0;
// 	while (str[i])
// 	{
// 		if (str[i] == '\"' || str[i] == '\'')
// 		{
// 			open = i;
// 			quote = str[i++];
// 			while (str[i] && str[i] != quote)
// 				i++;
// 			if (str[i] == quote)
// 				close = i;
// 			if (index >= open && index <= close)
// 				return (1);
// 		}
// 		i++;
// 	}
// 	return (0);
// }

int	ft_skip_space(char *str)
{
	int	i;

	if (!str)
		return (0);
	i = 0;
	while (str[i] && ft_isspace(str[i]))
		i++;
	return (i);
}

int	ft_is_quote(char c)
{
	if (c == 39 || c == 34)
		return (c);
	return (0);
}

int	ft_is_belong(char c)
{
	if (c == '>')
		return (c);
	else if (c == '<')
		return (c);
	else if (c == '|')
		return (c);
	return (0);
}

// int	delimiter_count(char *s)
// {
// 	int		i;
// 	int		count;
// 	char	quote;
// 	char	d_cmp;

// 	i = 0;
// 	count = 0;
// 	quote = 0;
// 	while (s[i])
// 	{
// 		if (quote)
// 		{
// 			if (s[i] == quote)
// 				quote = 0;
// 			i++;
// 			continue ;
// 		}
// 		if (s[i] == '\'' || s[i] == '"')
// 			quote = s[i];
// 		if (ft_is_belong(s[i]) && !ft_isspace(s[i + 1]))
// 		{
// 			d_cmp = ft_is_belong(s[i]);
// 			if (ft_is_belong(s[i + 1]) == d_cmp && (s[i + 2] && !ft_isspace(s[i
// 						+ 2])))
// 				i++;
// 			count++;
// 		}
// 		if (ft_is_belong(s[i]) && (!ft_isspace(s[i ])))
// 		i++;
// 	}
// 	return (count);
// }

// char	*ft_extend_space(char *s)
// {
// 	char	*ret;
// 	int		del_count;

// 	del_count = delimiter_count(s);
// 	printf("%d\n", del_count);
// 	ret = NULL;
// 	return (ret);
// }

char	*ft_strndup(char *str, size_t n)
{
	char	*s;
	int		i;

	if (!str)
		return (NULL);
	s = malloc(sizeof(char) * n + 1);
	if (!s)
		return (NULL);
	i = 0;
	while (i < n)
	{
		s[i] = str[i];
		i++;
	}
	s[i] = 0;
	return (s);
}

char	*ft_strtok_quoted(char *str)
{
	static char		*saved_str = NULL;
	char			*token;
	static size_t	index = 0;
	char			quote;
	int				i;
	
	if (str)
		saved_str = str;
	if (!saved_str)
		return (NULL);
	i = 0;
	quote = 0;
	token = NULL;
	index += ft_skip_space(saved_str + index);
	if (!saved_str[index])
	{
		saved_str = NULL;
		index = 0;
		return (NULL);
	}
	i = 0;
	if (ft_is_quote(saved_str[index + i]))
	{
		quote = ft_is_quote(saved_str[index + i]);
		i++;
		while (saved_str[index + i] && quote)
		{
			if (ft_is_quote(saved_str[index + i]) == quote)
				quote = 0;
			i++;
		}
		token = ft_strndup(saved_str + index, i);
		index += i;
	}
	else
	{
		while (saved_str[index + i] && !ft_isspace(saved_str[index + i]))
		{
			if (ft_is_belong(index + i))
				break ;
			i++;
		}
		token = ft_strndup(saved_str + index, i);
		index += i;
	}
	// printf("the index is at %ld\n", index);
	// if (saved_str[index + i] == quote)
	// {
	// 	quote = 0;
	// 	i++;
	// }
	// token = malloc(sizeof(char) * (i));
	// if (!token)
	// 	return (NULL);
	// i = 0;
	// while (saved_str[index + i] && ft_is_quote(saved_str[index
	// + i]) != quote)
	// {
	// 	token[i] = saved_str[index + i];
	// 	i++;
	// }
	// token[i] = 0;
	// index += i;
	return (token);
}

t_cmd	*parse_input(t_shell *shell, char *input)
{
	t_cmd	*cmd_list;
	int		i;
	char	*token;

	// char	**args;
	// char	*tmp;
	i = 0;
	if (!input)
		return (NULL);
	// tmp = ft_extend_space(input);
	cmd_list = NULL;
	token = ft_strtok_quoted(input);
	if (!token)
		return (NULL);
	while (token != NULL)
	{
		printf("%s\n", token);
		token = ft_strtok_quoted(NULL);
	}
	// if (token)
	// {
	// 	if (!cmd_list)
	// 	{
	// 		cmd_list->args = token;
	// 		cmd_list->next = NULL;
	// 	}
	// }
	// printf("%s\n", input);
	return (cmd_list);
}

// int	main(void)
// {
// 	int	i;

// 	char str[] = "            fas";
// 	i = ft_skip_space(str + 6);
// 	printf("space is %d", i);
// }

int	main(int ac, char **av, char **envp)
{
	t_shell	shell;
	char	*input;

	(void)ac;
	(void)av;
	shell = init_shell(envp);
	while (42)
	{
		input = readline(PROMPT);
		if (input == NULL)
		{
			write(1, "exit\n", 5);
			break ;
		}
		if (input && *input)
		{
			add_history(input);
			shell.cmd = parse_input(&shell, input);
			// if (shell.cmd)
			// {
			// 	execute_command(shell.cmd, shell);
			// 	free_command(shell.cmd);
			// }
		}
		free(input);
	}
	ft_lstclear(&shell.envp, free);
	return (0);
}
