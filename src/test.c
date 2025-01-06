#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

char	*my_strtok(char *str, const char *delim)
{
	static char	*next_token = NULL;
	char		*token_start;

	token_start = NULL;
	if (str != NULL)
	{
		// If a new string is passed, reset the next_token pointer
		next_token = str;
	}
	if (next_token == NULL)
	{
		// No more tokens
		return (NULL);
	}
	// Skip leading delimiters
	while (*next_token && strchr(delim, *next_token) != NULL)
	{
		next_token++;
	}
	if (*next_token == '\0')
	{
		// No more tokens
		next_token = NULL;
		return (NULL);
	}
	// Mark the start of the token
	token_start = next_token;
	// Find the next delimiter
	while (*next_token && strchr(delim, *next_token) == NULL)
	{
		next_token++;
	}
	if (*next_token != '\0')
	{
		// Null-terminate the current token and advance to the next one
		*next_token = '\0';
		next_token++;
	}
	else
	{
		// End of string reached
		next_token = NULL;
	}
	return (token_start);
}
char	*strtok_prompt(char *str);
int	main(void)
{
	char	str[] = "  echo\"Bonjour  Rolio\" |cat|cat |grep 'Rol'  ";

	printf("str:%s\n", strtok_prompt(str));
	printf("str:%s\n", strtok_prompt(NULL));
	printf("str:%s\n", strtok_prompt(NULL));
	printf("str:%s\n", strtok_prompt(NULL));
	printf("str:%s\n", strtok_prompt(NULL));
	printf("str:%s\n", strtok_prompt(NULL));
	printf("str:%s\n", strtok_prompt(NULL));
	printf("str:%s\n", strtok_prompt(NULL));
	printf("str:%s\n", strtok_prompt(NULL));
	printf("str:%s\n", strtok_prompt(NULL));
	printf("str:%s\n", strtok_prompt(NULL));
	printf("str:%s\n", strtok_prompt(NULL));
	return (0);
}
char	*ft_pad_zero(char *str);
char	*strtok_prompt(char *str)
{
	static char	*buff = NULL;
	static int	size = 0;
	static int	index = 0;
	int			i;

	if (str)
	{
		size = 0;
		index = 0;
		while (str[size])
			size++;
		buff = ft_pad_zero(str);
		i = 0;
		while (i < size)
		{
			if (buff[i])
				printf("%c", buff[i]);
			else
				printf("\033[33m0\033[0m");
			i++;
		}
		printf("\n");
	}
	if (!buff)
		return (NULL);
	while (index < size && !buff[index])
		index++;
	if (index == size)
		return (NULL);
	i = 0;
	while (index + i < size && buff[index + i])
		i++;
	str = malloc(i + 1);
	i = 0;
	while (index + i < size && buff[index + i])
	{
		str[i] = buff[index + i];
		i++;
	}
	str[i] = 0;
	index += i;
	return (str);
}

char	*ft_pad_zero(char *str)
{
	int		i;
	char	delim;

	i = 0;
	delim = 0;
	while (str[i])
	{
		if (delim)
		{
			if (str[i] == delim)
				delim = 0;
			i++;
			continue ;
		}
		if (str[i] == ' ')
			str[i] = 0;
		if (str[i] == '\'' || str[i] == '"')
			delim = str[i];
		i++;
	}
	return (str);
}
