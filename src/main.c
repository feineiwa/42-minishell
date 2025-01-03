/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: frahenin <frahenin@student.42antananari    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/16 11:21:53 by nrasamim          #+#    #+#             */
/*   Updated: 2025/01/03 16:03:44 by frahenin         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void	panic(char *s)
{
	perror(s);
}

int	count_quotes(char *input)
{
	size_t	i;
	size_t	sq;
	size_t	dq;

	i = 0;
	sq = 0;
	dq = 0;
	while (input[i])
	{
		if (input[i] == 39)
			sq++;
		if (input[i] == 34)
			dq++;
		i++;
	}
	if (sq % 2 != 0 || dq % 2 != 0)
		return (1);
	return (0);
}

int	ft_isspace(char c)
{
	return ((c == 32 || (c >= 7 && c <= 13)));
}

int	ft_count_token(char *str)
{
	int		count;
	int		i;
	char	quote;

	count = 0;
	i = 0;
	while (str[i])
	{
		while (ft_isspace(str[i]))
			i++;
		if (str[i] == '\0')
			break ;
		if (str[i] == '\'' || str[i] == '\"')
		{
			quote = str[i++];
			while (str[i] && str[i] != quote)
				i++;
			if (str[i] == quote)
				i++;
		}
		else
		{
			while (str[i] && !ft_isspace(str[i]) && str[i] != '\''
				&& str[i] != '\"')
				i++;
		}
		count++;
	}
	// printf("count is %d\n", count);
	return (count);
}

int	ft_is_between(char *str, int index)
{
	int		i;
	char	quote;
	int		open;
	int		close;

	i = 0;
	open = 0;
	close = 0;
	while (str[i])
	{
		if (str[i] == '\"' || str[i] == '\'')
		{
			open = i;
			quote = str[i++];
			while (str[i] && str[i] != quote)
				i++;
			if (str[i] == quote)
				close = i;
			if (index >= open && index <= close)
				return (1);
		}
		i++;
	}
	return (0);
}

char	**ft_split_input(char *str)
{
	char	**arr;
	int		i;

	i = 0;
	if (!str)
		return (NULL);
	printf("\n%d", ft_count_token(str));
	arr = (char **)ft_calloc(sizeof(char *), ft_count_token(str));
	if (!arr)
		return (NULL);
	return (arr);
}

t_cmd	*parse_input(t_shell *shell, char *input)
{
	t_cmd	*cmd_list;
	char	**args;
	int		i;

	cmd_list = NULL;
	if (count_quotes(input))
	{
		panic("Error of quotes");
		return (NULL);
	}
	args = ft_split_input(input);
	// i = 0;
	// while (args[i])
	// {
	// 	printf("{%s}\n", args[i]);
	// 	i++;
	// }
	return (cmd_list);
}

int	main(void)
{
	if (ft_is_between(" \"' '   \"   ' '\" \" s", 17))
	{
		printf("anaty\n");
	}
}

/*int	main(int ac, char **av, char **envp)
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
}*/
