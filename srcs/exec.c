/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:23:43 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/04 23:10:43 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"
#include <string.h> // <--- 一旦使用

//いったんコピペ、どうせ後でいらなくなる
// reallocは後で変更必須
char	**tail_recursive(t_token *tok, int nargs, char **argv)
{
	char	**new_argv;

	if (tok == NULL || tok->kind == TK_EOF)
		return (argv);
	// 新しいサイズでメモリを再確保
	new_argv = realloc(argv, (nargs + 2) * sizeof(char *));
	if (new_argv == NULL)
	{
		free(argv); // reallocが失敗した場合に元のメモリを解放
		error("realloc");
	}
	argv = new_argv;
	// トークン文字列をコピーして追加
	argv[nargs] = ft_strdup(tok->word);
	if (argv[nargs] == NULL)
	{
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
char	*find_executable(const char *filename)
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
				error(ER_MALLOC_STRDUP);
			return (result);
		}
		if (path_end == NULL)
			break ;
		env = path_end + 1;
	}
	return (NULL);
}

int	exec_nonbuiltin(t_node *node) __attribute__((noreturn));
int	exec_nonbuiltin(t_node *node)
{
	char	*path;
	char	**argv;

	setup_redirect(node->command->redirects);
	argv = token_list_to_argv(node->command->args);
	path = argv[0];
	if (ft_strchr(path, '/') == NULL)
		path = find_executable(path);
	if (path == NULL)
		err_exit(argv[0], "command not found", 127);
	if (access(path, F_OK) < 0)
		err_exit(argv[0], "command not found", 127);
	execve(path, argv, get_environ(g_ctx.g_envmap));
	free(argv);
	reset_redirect(node->command->redirects);
	write(2, "execve failed\n", 14);
	exit(127);
	// fatal_error("execve");
}

pid_t	execute_pipe(t_node *node)
{
	pid_t	pid;

	if (node == NULL)
		return (-1);
	create_new_pipe(node);
	pid = fork();
	if (pid < 0)
		fatal_error("fork");
	else if (pid == 0)
	{
		reset_signal();
		process_child_pipe(node);
		if (is_builtin(node))
			exit(exec_builtin(node));
		else
			exec_nonbuiltin(node);
	}
	process_parent_pipe(node);
	if (node->next)
		return (execute_pipe(node->next));
	return (pid);
}

int	wait_pipe(pid_t pid)
{
	pid_t	result;
	int		status;
	int		wstatus;

	while (1)
	{
		result = wait(&wstatus);
		if (result == pid)
		{
			if (WIFSIGNALED(wstatus))
				status = 128 + WTERMSIG(wstatus);
			else
				status = WEXITSTATUS(wstatus);
		}
		else if (result < 0)
		{
			if (errno == ECHILD)
				break ;
			else if (errno == EINTR)
				continue ;
			else
				fatal_error("wait");
		}
	}
	return (status);
}

int	execute_cmd(t_node *node)
{
	pid_t	pid;
	int		cmd_status;

	if (open_redirect_file(node) < 0)
		return (ERROR_OPEN_REDIR);
	if (node->next == NULL && is_builtin(node))
		cmd_status = exec_builtin(node);
	else
	{
		pid = execute_pipe(node);
		cmd_status = wait_pipe(pid);
	}
	return (cmd_status);
}

// stat_locは終了ステータスを格納する変数
// syntax_errorは構文エラーがあるかどうかを格納する変数
void	interpret(char *line, int *stat_loc)
{
	t_token	*tok;
	t_node	*node;

	tok = tokenize(line);
	if (at_eof(tok))
		;
	else if (g_ctx.g_syntax_error)
		*stat_loc = ERROR_TOKENIZE;
	else
	{
		node = parse(tok);
		if (g_ctx.g_syntax_error)
			*stat_loc = ERROR_PARSE;
		else
		{
			expand(node);
			*stat_loc = execute_cmd(node);
		}
		free_node(node);
	}
	free_token(tok);
}
