/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:23:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/01/26 17:24:35 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

# ifndef PATH_MAX
# define PATH_MAX 10000
# endif 

void	err_exit(const char *location, const char *msg, int status)
{
	printf("minishell: %s: %s\n", location, msg);
	exit(status);
}

void error(char *msg)
{
    printf("%s\n",msg);
    exit(2);
}

char	*ft_strncpy(char *dest, char *src, size_t n)
{
	size_t	i;

	i = 0;
	while (src[i] && n > i)
	{
		dest[i] = src[i];
		i++;
	}
	dest[i] = '\0';
	return (dest);
}

void	validate_access(const char *path, const char *filename)
{
	if (path == NULL)
		err_exit(filename, "command not found", 127);
	if (access(path, F_OK) < 0)
		err_exit(filename, "command not found", 127);
}

char	*search_path(const char *filename)
{
	char	path[PATH_MAX];
	char	*value;
	char	*end;

	value = getenv("PATH");
	while (*value)
	{
		// /bin:/usr/bin
		//     ^
		//     end
		ft_bzero(path, PATH_MAX);
		end = ft_strchr(value, ':');
		if (end)
			ft_strncpy(path, value, end - value);
		else
			ft_strlcpy(path, value, PATH_MAX);
		ft_strlcat(path, "/", PATH_MAX);
		ft_strlcat(path, filename, PATH_MAX);
		if (access(path, X_OK) == 0)
		{
			char	*dup;

			dup = ft_strdup(path);
			if (dup == NULL)
				error("strdup");
			return (dup);
		}
		if (end == NULL)
			return (NULL);
		value = end + 1;
	}
	return (NULL);
}

int exec(char *arg[])
{
    char    *path;
    char    **env;
    int     pid;
    int     status;

    pid = fork();
        path = arg[0];
    if (pid < 0)
        error("fork_error");
    else if(pid == 0)
    {
        if (ft_strchr(path, '/') == NULL)
            path = search_path(path);
            // path = ft_strjoin("/bin/", arg[0]);
        validate_access(path, arg[0]);
        execve(path, arg, env);
        error("execve error");
    }
    else
    {
        wait(&status);
        return (WEXITSTATUS(status));
    }
    return (1);
}

int interpret(char *line)
{
    int status;
    char *arg[2];

    arg[0] = line;
    arg[1] = NULL;
    exec(arg);
    status = 1;
    return (status);
}
