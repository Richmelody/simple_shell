#include "shell.h"
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/stat.h>

/**
 * is_cmd - determines if a file is an executable command
 * @info: the info struct
 * @path: path to the file
 *
 * Return: 1 if true, 0 otherwise
 */
int is_cmd(info_t *info, char *path)
{
	struct stat st;

	if (!path || stat(path, &st) == -1)
		return (0);

	return (S_ISREG(st.st_mode) && (st.st_mode & S_IXUSR));
}

/**
 * dup_chars - duplicates characters
 * @pathstr: the PATH string
 * @start: starting index
 * @stop: stopping index
 *
 * Return: pointer to new buffer
 */
char *dup_chars(char *pathstr, int start, int stop)
{
	int len = stop - start;
	char *buf = malloc(len + 1);

	if (!buf)
		return (NULL);

	strncpy(buf, pathstr + start, len);
	buf[len] = '\0';
	return (buf);
}

/**
 * find_path - finds this cmd in the PATH string
 * @info: the info struct
 * @pathstr: the PATH string
 * @cmd: the cmd to find
 *
 * Return: full path of cmd if found or NULL
 */
char *find_path(info_t *info, char *pathstr, char *cmd)
{
	int i = 0, curr_pos = 0;
	char *path;

	if (!pathstr)
		return (NULL);
	if (cmd[0] == '/')
	{
		if (is_cmd(info, cmd))
			return (cmd);
		return (NULL);
	}
	while (1)
	{
		if (!pathstr[i] || pathstr[i] == ':')
		{
			path = dup_chars(pathstr, curr_pos, i);
			if (!*path)
				path = strdup(cmd);
			else
				path = strcat(strcat(path, "/"), cmd);
			if (is_cmd(info, path))
				return (path);
			free(path);
			if (!pathstr[i])
				break;
			curr_pos = i + 1;
		}
		i++;
	}
	return (NULL);
}

