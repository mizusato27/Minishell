#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include "minishell.h"

#include <string.h>

#define ERROR_PREFIX "minishell: "

static void	perror_prefix(void)
{
	dprintf(STDERR_FILENO, "%s", ERROR_PREFIX);
}

void	xperror(const char *location)
{
	perror_prefix();
	perror(location);
}

int	stash_fd(int fd)
{
	int	stash;

	stash = fcntl(fd, F_DUPFD, 10);
	if (stash < 0)
		// fatal_error("");
		error("");
	if (close(fd) < 0)
		// fatal_error("");
		error("");
	return (stash);
}

int	read_here_document(const char *delimiter)// delimiter:区切り文字
{
	char	*line;
	int		pipe_fd[2];// パイプのファイルディスクリプタを格納する配列

	// パイプを作成
	if (pipe(pipe_fd) < 0)
		// fatal_error("pipe");
		error("");
	// 区切り文字が入力されるまでループ
	while (1)
	{
		// プロンプト">"を表示して入力を受け取る
		line = readline("> ");
		if (line == NULL)// EOF（Ctrl+D）が入力された場合
			break;
		// 区切り文字と一致するか確認
		if (strcmp(line, delimiter) == 0)
		{
			free(line);
			break ;
		}
		// パイプの書き込み側に入力行を書き込む
		dprintf(pipe_fd[1], "%s\n", line);
		free(line);
	}
	// 書き込み側のファイルディスクリプタを閉じる
	close(pipe_fd[1]);
	// 読み取り側のファイルディスクリプタを返す
	return (pipe_fd[0]);
}

// リダイレクトのためのファイルを開く関数
int	open_redirect_file(t_node *redirects)
{
	if (redirects == NULL)
		return (0);
	if (redirects->kind == ND_REDIR_OUT)
		redirects->filefd = open(redirects->filename->word, O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 	- ND_REDIR_OUT（出力リダイレクト）の場合：
//   - O_CREAT: ファイルが存在しない場合は作成
//   - O_WRONLY: 書き込み専用でオープン
//   - O_TRUNC: ファイルが存在する場合は内容を削除
//   - パーミッション 0644 (rw-r--r--)
	else if (redirects->kind == ND_REDIR_IN)
		redirects->filefd = open(redirects->filename->word, O_RDONLY);
// 	入力リダイレクション（<）の場合は、ファイルを読み込み専用（O_RDONLY）で開きます
// 	出力と異なり、新規作成（O_CREAT）や書き込み権限（O_WRONLY）は不要です
	else if (redirects->kind == ND_REDIR_APPEND)
		redirects->filefd = open(redirects->filename->word, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (redirects->kind == ND_REDIR_HEREDOC)
		redirects->filefd = read_here_document(redirects->delimiter->word);
	else
		// assert_error("open_redir_file");
		error("");
	if (redirects->filefd < 0)
	{
		xperror(redirects->filename->word);
		return (-1);
	}
	redirects->filefd = stash_fd(redirects->filefd);
	return (open_redirect_file(redirects->next));
}

// 実際のリダイレクト処理を行う関数
void	do_redirect(t_node *redirects)
{
	if (redirects == NULL)
		return ;
	if (redirects->kind == ND_REDIR_OUT || redirects->kind == ND_REDIR_IN
		|| redirects->kind == ND_REDIR_APPEND || redirects->kind == ND_REDIR_HEREDOC)
	{
		redirects->stashed_targetfd = stash_fd(redirects->targetfd);
		dup2(redirects->filefd, redirects->targetfd);
	}
// 	- ND_REDIR_OUT の場合：
//   - 元のファイルディスクリプタ（targetfd）を stashfd で保存
//   - dup2 で filefd を targetfd にコピー
//     （例：標準出力をファイルにリダイレクト）
	else
		// assert_error("do_redirect");
		error("");
	do_redirect(redirects->next);
}

int	execute_cmd(t_node *node)
{
	extern char	**environ;
	const char	*path;
	pid_t		pid;
	int			wstatus;
	char		**argv;

	pid = fork();
	if (pid < 0)
		// fatal_error("fork");
		error("");
// 	子プロセス（pid == 0）の場合：
// - トークンリストを argv 配列に変換
// - パスにスラッシュが含まれていない場合は PATH を検索
// - 実行権限をチェック（validate_access）
// - execve でコマンドを実行
// - execve が失敗した場合はエラー処理
	else if (pid == CHILD_PROCESS)
	{
		// child process
		argv = token_list_to_argv(node->args);
		path = argv[0];
		if (strchr(path, '/') == NULL)
			// path = search_path(path);
			path = find_executable(path);
		if (path == NULL || access(path, F_OK) < 0)
			// err_exit(path, ER_ACCESS, 127);
			error("");
		execve(path, argv, environ);
		// fatal_error("execve");
		error("");
	}
// 	親プロセスの場合：
// - 子プロセスの終了を待機（wait）
// - 終了ステータスを返す
	else// parent process
		wait(&wstatus);
	return (WEXITSTATUS(wstatus));
}

// リダイレクトを元に戻す関数
void	reset_redirect(t_node *redirects)
{
	if (redirects == NULL)
		return ;
	reset_redirect(redirects->next);// 末尾から先頭に向かって処理（再帰呼び出しを先に行う）
	if (redirects->kind == ND_REDIR_OUT || redirects->kind == ND_REDIR_IN
		|| redirects->kind == ND_REDIR_APPEND || redirects->kind == ND_REDIR_HEREDOC)
	{
		close(redirects->filefd);
		close(redirects->targetfd);
		dup2(redirects->stashed_targetfd, redirects->targetfd);
	}
// 	- ND_REDIR_OUT の場合：
//   - 開いたファイル（filefd）を閉じる
//   - リダイレクト先（targetfd）を閉じる
//   - 保存しておいた元のファイルディスクリプタを復元
	else
		// assert_error("reset_redirect");
		error("");
}

int	redirect(t_node *node)
{
	int	cmd_status;

	if (open_redirect_file(node->redirects) < 0)
		return (ERROR_OPEN_REDIR);
	do_redirect(node->redirects);
	cmd_status = execute_cmd(node);
	reset_redirect(node->redirects);
	return (cmd_status);
}

// 以下は"minishellの作り方"の関数
// void	interpret(char *line, int *stat_loc)
// {
// 	t_token	*tok;
// 	t_node	*node;

// 	tok = tokenize(line);
// 	if (at_eof(tok))
// 		;
// 	else if (syntax_error)
// 		*stat_loc = ERROR_TOKENIZE;
// 	else
// 	{
// 		node = parse(tok);
// 		// expand(node);
// 		// argv = token_list_to_argv(node->args);
// 		// *stat_loc = exec(argv);
// 		// free_argv(argv);
// 		if (syntax_error)
// 			*stat_loc = ERROR_PARSE;
// 		else
// 		{
// 			expand(node);
// 			*stat_loc = ft_redirect(node);// <- step9
// 		}
// 		free_node(node);
// 	}
// 	free_tok(tok);
// }
