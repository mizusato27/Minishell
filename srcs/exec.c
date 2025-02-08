/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:23:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/09 07:54:58 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//いったんコピペ、どうせ後でいらなくなる
char	**tail_recursive(t_token *tok, int nargs, char **argv)
{
	char **new_argv;

	if (tok == NULL || tok->kind == TK_EOF)
		return (argv);

	// 新しいサイズでメモリを再確保
	new_argv = realloc(argv, (nargs + 2) * sizeof(char *));
	if (new_argv == NULL) {
		free(argv); // reallocが失敗した場合に元のメモリを解放
		error("realloc");
	}
	argv = new_argv;

	// トークン文字列をコピーして追加
	argv[nargs] = ft_strdup(tok->word);
	if (argv[nargs] == NULL) {
		free(argv); // strdupが失敗した場合に全体を解放
		error(ER_MALLOC_STRDUP);
	}
	argv[nargs + 1] = NULL;

	// 再帰呼び出し
	return (tail_recursive(tok->next, nargs + 1, argv));
}

//いったんコピペ、どうせ後でいらなくなる
char	**token_list_to_argv(t_token *tok)
{
	char	**argv;

	// 初期状態のargvを確保
	argv = ft_calloc(1, sizeof(char *));
	if (argv == NULL)
		error(ER_MALLOC_CALLOC);

	// 再帰的にトークンリストを変換
	return (tail_recursive(tok, 0, argv));
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

//PATHに指定されたディレクトリを順番に探索して、実行可能なファイルがあればそのパスを返す
static char	*construct_path(char path[PATH_MAX], const char *filename, char *env, char *end)
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

//:はディレクトリの終わりを指す
//PATHに指定されたディレクトリを順番に探索して、実行可能なファイルがあればそのパスを返す
static char	*find_executable(const char *filename)
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
				error(ER_MALLOC_STRDUP);
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
static int	execute(char *args[])
{
	int		pid;
	int		status;
	char	*path;
	char 	*cmd;

	pid = fork();
	cmd = args[0];
	path = cmd;
	if (pid < 0)
		error(ER_FORK);
	else if (pid == CHILD_PROCESS)
	{
		if (ft_strchr(path, '/') == NULL)
			path = find_executable(path);
		if (path == NULL || access(path, F_OK) < 0)
			err_exit(cmd, ER_ACCESS, 127);
		execve(path, args, NULL);
		error(ER_EXECVE);
	}
	else
		wait(&status);
	return (WEXITSTATUS(status));
}

// int	interpret(char *line)
// {
// 	int		status;
// 	char	*arg[2];

// 	if (ft_strncmp(line, "exit", 4) == 0)
// 	{
// 		printf("exit\n");
// 		exit(0);
// 	}
// 	arg[0] = line;
// 	arg[1] = NULL;
// 	execute(arg);
// 	status = 1;
// 	return (status);
// }

//stat_locは終了ステータスを格納する変数
//syntax_errorは構文エラーがあるかどうかを格納する変数
void	interpret(char *line, int *stat_loc)
{
	t_token	*tok;
	char	**argv;
	t_node	*node;

	tok = tokenize(line);
	if (tok->kind == TK_EOF)
		;
	else if (syntax_error)
		*stat_loc = ERROR_TOKENIZE;
	else
	{
		node = parse(tok);
		expand(node);
		argv = token_list_to_argv(node->args);
		*stat_loc = execute(argv);
		free_argv(argv);
		free_node(node);
	}
	free_token(tok);
}

// int main(int argc, char **argv)
// {
// 	interpret(argv[1]);
// }
