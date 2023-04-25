#include "main.h"

/**
 * main - will be the entry point of the shell
 * @argc: arg count
 * @argv: number of arguments
 *
 * Return: 0 on success or other integer
 */

int main(int argc, char *argv[])
{
	size_t buffsize = 0;
	char *line = NULL;
	int i, number_tokens = 0, cmdcount = 1, shell_interaction;

	signal(SIGINT, SIG_IGN); 
	shell_interaction = isatty(STDIN_FILENO);
	if (shell_interaction == 0 && argc == 1) 
	{
		while (getline(&line, &buffsize, stdin) > 0) 
		{
			number_tokens = numcount(line); 
			parse(line, number_tokens, argv, cmdcount);
			line = NULL;
		}
		free(line);
		return (0);
	}
	while (shell_interaction)
	{
		/**prompt */
		write(1, "($) ", 4);
		number_tokens = 0; 
		i = getline(&line, &buffsize, stdin);
		if (i < 0)
		{
			free(line);
			write(1, "\n", 1);
			break;
		}
		number_tokens = numcount(line); 
		parse(line, number_tokens, argv, cmdcount);
		cmdcount++;
		
		line = NULL;}
	return (0);
}