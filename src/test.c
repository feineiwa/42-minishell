#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

// Buffer size for input
#define INPUT_BUFFER 1024

// Structure for command history
typedef struct s_history
{
	char				*command;
	struct s_history	*next;
	struct s_history	*prev;
}						t_history;

// Global variables for terminal settings and history
struct termios			original_termios;
t_history				*history_head = NULL;
t_history				*current_history = NULL;

// Restore terminal settings on exit
void	reset_terminal(void)
{
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &original_termios);
}

// Enable raw mode
void	enable_raw_mode(void)
{
	struct termios	raw;

	tcgetattr(STDIN_FILENO, &original_termios);
	atexit(reset_terminal);
	raw = original_termios;
	raw.c_lflag &= ~(ECHO | ICANON); // Disable echo and canonical mode
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

// Add command to history
void	add_to_history(char *command)
{
	t_history	*new_entry;

	new_entry = malloc(sizeof(t_history));
	new_entry->command = strdup(command);
	new_entry->next = history_head;
	new_entry->prev = NULL;
	if (history_head)
		history_head->prev = new_entry;
	history_head = new_entry;
	current_history = NULL;
}

// Handle special keys
void	handle_special_keys(char *buffer, int *cursor, char key)
{
	if (key == 127)
	{ // Backspace
		if (*cursor > 0)
		{
			memmove(&buffer[*cursor - 1], &buffer[*cursor],
				strlen(&buffer[*cursor]) + 1);
			(*cursor)--;
		}
	}
	// Add more cases for arrow keys, etc.
}

// Read line from user
char	*readline_custom(const char *prompt)
{
	char	*buffer;
	int		cursor;
		char c;
	ssize_t	n;

	buffer = malloc(INPUT_BUFFER);
	cursor = 0;
	if (!buffer)
		return (NULL);
	printf("%s", prompt);
	fflush(stdout);
	while (1)
	{
		n = read(STDIN_FILENO, &c, 1);
		if (n <= 0)
			break ;
		if (c == '\n')
		{ // Enter key
			buffer[cursor] = '\0';
			printf("\n");
			break ;
		}
		else if (c == 127)
		{ // Backspace
			handle_special_keys(buffer, &cursor, c);
		}
		else
		{ // Regular characters
			buffer[cursor++] = c;
			buffer[cursor] = '\0';
		}
		printf("\r%s%s", prompt, buffer);
		fflush(stdout);
	}
	add_to_history(buffer);
	return (buffer);
}

int	main(void)
{
	char	*line;

	enable_raw_mode();
	while (1)
	{
		line = readline_custom("> ");
		if (line == NULL)
			break ;
		if (strcmp(line, "exit") == 0)
		{
			free(line);
			break ;
		}
		printf("You entered: %s\n", line);
		free(line);
	}
	return (0);
}
