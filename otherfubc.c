#include "holberton.h"
/**
 * checkbltin - checks if the command entered is a built-in
 * @line: input entered by user
 * @ar: array that can be handled by execve
 * @newline: duplicate of line
 * @array: Tokens to check
 * Return: Return depends upon if the built-in was found
 */
int checkbltin(char *line, char **ar, char *newline, char **array)
{
	int i = 0;

	if (_strcmp(ar[0], "exit") == 0) /* compares first token to exit */
	{
		myfree(line, ar, newline, array);
		_exit(errno); /* exits with status 0 or errno */
	}
	if (_strcmp(ar[0], "env") == 0) /* compares first token to env */
	{
		for (i = 0; environ[i] != NULL; i++)
		{
			_puts(environ[i]); /* prints string of environ */
			write(1, "\n", 1);
		}
		return (2);
	}
	if (_strcmp(ar[0], "cd") == 0) /* compares first token to exit */
	{
		changedir(ar);
		return (2); /* go back to shell loop */	}
	return (0);
}
/**
 * changedir - changes directory to ar
 * @ar: the directory to change to or nothing
 */
void changedir(char **ar)
{
	int i = 0;
	char *homeval = NULL, *home = NULL;

	if (ar[1] == NULL)
	{ /* if cd is by itself */
		for (i = 0; environ[i] != NULL; i++) /* loops through environ */
		{
			if (_strncmp("HOME=", environ[i], 5) == 0)
			{ /* find the line matching home */
				home = _strdup(environ[i]);
				strtok(home, "="); /* stores its value */
				homeval = strtok(NULL, "=");
				break;
			}
		}
	}
	else
		homeval = ar[1]; /* homeval is set to 2nd arg */
	chdir(homeval); /* change directory to homeval */
	free(home);
}

/**
 * exe - function to execute new processes
 * @line: input from user
 * @ar: array to be handled by execve
 * @nln: duplicate of line
 * @arry: Tokens to check
 * @argv: arguments
 * @flcnt: number of commands that have failed
 * Return: 0 for success
 */

int exe(char *line, char **ar, char *nln, char **arry, char **argv, int flcnt)
{
	int status, onpath = -1;
	pid_t child;
	struct stat *statbuf;

	statbuf = malloc(sizeof(struct stat)); /* creates status buffer */
	if (stat(ar[0], statbuf) == -1) /* checks if filename or path */
	{
		onpath = findonpath(ar); /* if file name, searchespath for it */
		if (onpath == 0)
		{
			free(statbuf); /* if on path go back */
			return (0);
		}
	}
	child = fork(); /* forks a child process */
	if (child == 0)
	{
		if (execve(ar[0], ar, environ) == -1) /* attempt execute file */
		{
			_printf("%s: %d: %s: not found\n", argv[0], flcnt, ar[0]);
			free(statbuf);
			myfree(line, ar, nln, arry);
			_exit(-1); /* terminates the child */
		}
	}
	else
	{
		while (waitpid(-1, &status, 0) != child) /* waits for child */
			;
	}
	free(statbuf);
	if (status == 0) /* checks if execve failed or succeeded */
		errno = 0;
	if (status == 512)
		errno = 2;
	if (status == 65280)
		errno = 127;
	return (0);
}