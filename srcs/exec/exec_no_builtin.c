/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_no_builtin.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/06 19:21:56 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/08 18:25:55 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

// PATHに指定されたディレクトリを順番に探索して、実行可能なファイルがあればそのパスを返す
// lsが来たときの例: /bin:/usr/binの中から最初の「:」までを取り出す
//そこにlsをくっつけて、そのパスが実行可能かどうかを確認する
static void	construct_path(char path[PATH_MAX], const char *filename, char *env,
		char *end)
{
	int	cpy_len;

	if (end)
		cpy_len = end - env;
	else
		cpy_len = ft_strlen(env);
	ft_bzero(path, PATH_MAX);
	ft_strncpy(path, env, cpy_len);
	ft_strlcat(path, "/", PATH_MAX);
	ft_strlcat(path, filename, PATH_MAX);
}

//:はディレクトリの終わりを指す
// PATHに指定されたディレクトリを順番に探索して、実行可能なファイルがあればそのパスを返す
static char	*find_executable(const char *filename)
{
	char	path[PATH_MAX];
	char	*env;
	char	*path_end;
	char	*result;

	env = xgetenv("PATH");
	while (*env)
	{
		path_end = ft_strchr(env, ':');
		construct_path(path, filename, env, path_end);
		if (access(path, X_OK) == 0)
		{
			result = ft_strdup(path);
			if (result == NULL)
				malloc_error(ER_STRDUP);
			return (result);
		}
		if (path_end == NULL)
			break ;
		env = path_end + 1;
	}
	return (NULL);
}

// int			exec_nonbuiltin(t_node *node) __attribute__((noreturn));
int	exec_nonbuiltin(t_node *node)
{
	char	*path;
	char	**argv;

	setup_redirect(node->command->redirects);
	argv = token_list_to_argv(node->command->args);
	path = argv[0];
	if (ft_strchr(path, '/') == NULL)
		path = find_executable(path);
	if (path == NULL || access(path, F_OK) < 0)
		not_found_cmd(argv[0], ER_ACCESS, ERROR_CMD);
	execve(path, argv, get_environ(g_ctx.g_envmap));
	free(argv);
	reset_redirect(node->command->redirects);
	write(STDERR_FILENO, ER_EXEC, 14);
	exit(ERROR_CMD);
}
