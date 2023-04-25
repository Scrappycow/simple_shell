#include "main.h"

/**
 * _strcpy - function copies str from src to dest
 * @src: source string
 * @dest: destination string
 * Return: pointer to destination string
 */
char *_strcpy(char *dest, char *src)
{
	int i;
	int lens = 0;

	while (*(src + lens) != '\0')
	{
		lens++;
	}

	for (i = 0; i <= lens; i++)
	{
		*(dest + i) = *(src + i);
	}
	return (dest);
}
/**
 * executepath - function used to access PATH
 * @p: directory in PATH to access
 * @tokens: array of tokens to check
 */
void executepath(char *p, char **tokens)
{
	int status, len, len2;
	pid_t child;
	char *newp = NULL;

	for (len = 0; *(p + len) != '\0'; len++)
		;
	for (len2 = 0; tokens[0][len2] != '\0'; len2++)
		;
	newp = malloc(sizeof(char) * (len + len2 + 2));
	_strcpy(newp, p);
	_strcat(newp, "/"); 
	_strcat(newp, tokens[0]);
	newp[(len + len2 + 1)] = '\0';

	child = fork(); 

	if (child == 0)
	{
		if (access(newp, X_OK) == 0) 
		{
			execve(newp, tokens, environ);
		}
	}
	else
	{
		while (waitpid(-1, &status, 0) != child) 
			;
	}
	if (status == 0)
		errno = 0;
	free(newp);
}

/**
 * searchpath - will search the PATH
 * @p: part of PATH to search
 * @tokens: array of tokens
 * Return: 0 for success
 */

int searchpath(char *p, char **tokens)
{
	struct dirent *de;
	int reached = 0, onepath = -1;

	p = strtok(p, ":"); 
	while (p != NULL && reached != 1)
	{
		DIR *dr = opendir(p); 
		if (dr == NULL)
		{
			return (0);
		}
		while ((de = readdir(dr)) != NULL)
			
		{
			if (_strcmp(de->d_name, tokens[0]) == 0)
			{ 
				executepath(p, tokens);
				
				onepath = 0;
				
				reached = 1;
				break;
			}
		}
		closedir(dr); 
		if (reached == 0)
			p = strtok(NULL, ":");
		}
	return (onepath);
}

/**
 * findonpath - find PATH in the environ
 * @tokens: token to pass along to PATH
 * Return: 0 for success
 */

int findonpath(char **tokens)
{
	int i = 0, onepath;
	char *p = NULL;
	char *path = NULL;

	for (i = 0; environ[i] != NULL; i++)
	{
		if (_strncmp("PATH=", environ[i], 5) == 0)
			
		{
			path = _strdup(environ[i]);
			strtok(path, "=");
			p = strtok(NULL, "="); 
			onepath = searchpath(p, tokens); 
			break;
		}
	}
	free(path);
	return (onepath); 
}