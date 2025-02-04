/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:23:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/04 18:44:52 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

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

char	*construct_path(char path[PATH_MAX], const char *filename, char *env, char *end)
{
	int		cpy_len;

	if (end)
		cpy_len = end - env;
	else
		cpy_len = ft_strlen(env);
	ft_bzero(path, PATH_MAX);
	ft_strncpy(path, env, cpy_len);
	ft_strlcat(path, "/", PATH_MAX);
	ft_strlcat(path, filename, PATH_MAX);
	return (ft_strdup(path));
}

char	*find_executable(const char *filename)
{
	char	path[PATH_MAX];
	char	*env;
	char	*path_end;
	char	*result;

	env = getenv("PATH");
	while (*env)
	{
		path_end = ft_strchr(env, ':');
		construct_path(path, filename, env, path_end);
		if (access(path, X_OK) == 0)
		{
			result = ft_strdup(path);
			if (result == NULL)
				malloc_error("strdup: malloc error");
			return (result);
		}
		if (path_end == NULL)
			break ;
		env = path_end + 1;
	}
	return (NULL);
}

// F_OK: ファイルが存在するか
//execveは成功したら戻ってこない
int	execute(char *args[])
{
	int		pid;
	int		status;
	char	*path;
	char 	*cmd;

	pid = fork();
	cmd = args[0];
	path = cmd;
	if (pid < 0)
		error("fork_error");
	else if (pid == CHILD_PROCESS)
	{
		if (ft_strchr(path, '/') == NULL)
			path = find_executable(path);
		if (path == NULL || access(path, F_OK) < 0)
			err_exit(cmd, "command not found", 127);
		execve(path, args, NULL);
		error("execve error");
	}
	else
		wait(&status);
	return (WEXITSTATUS(status));
}

int	interpret(char *line)
{
	int		status;
	char	*arg[2];

	if (ft_strncmp(line, "exit", 4) == 0)
	{
		printf("exit\n");
		exit(0);
	}
	arg[0] = line;
	arg[1] = NULL;
	execute(arg);
	status = 1;
	return (status);
}

// int main(int argc, char **argv)
// {
// 	interpret(argv[1]);
// }
