#include <fcntl.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <readline/readline.h>
#include "minishell.h"

#include <string.h>

int	stash_fd(int fd)
{
	int	stash;

	stash = fcntl(fd, F_DUPFD, 10);
	if (stash < 0)
		fatal_error("");
	if (close(fd) < 0)
		fatal_error("");
	return (stash);
}

int	read_here_document(const char *delimiter)// delimiter:区切り文字
{
	char	*line;
	int		pipe_fd[2];// パイプのファイルディスクリプタを格納する配列

	// パイプを作成
	if (pipe(pipe_fd) < 0)
		fatal_error("pipe");
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
int	open_redirect_file(t_node *node)
{
	if (node == NULL)
		return (0);
	if (node->kind == ND_PIPELINE)
	{
		if (open_redirect_file(node->command) < 0)
			return (-1);
		if (open_redirect_file(node->next) < 0)
			return (-1);
		return (0);
	}
	else if (node->kind == ND_SIMPLE_CMD)
		return (open_redirect_file(node->redirects));
	if (node->kind == ND_REDIR_OUT)
		node->filefd = open(node->filename->word, O_CREAT | O_WRONLY | O_TRUNC, 0644);
// 	- ND_REDIR_OUT（出力リダイレクト）の場合：
//   - O_CREAT: ファイルが存在しない場合は作成
//   - O_WRONLY: 書き込み専用でオープン
//   - O_TRUNC: ファイルが存在する場合は内容を削除
//   - パーミッション 0644 (rw-r--r--)
	else if (node->kind == ND_REDIR_IN)
		node->filefd = open(node->filename->word, O_RDONLY);
// 	入力リダイレクション（<）の場合は、ファイルを読み込み専用（O_RDONLY）で開きます
// 	出力と異なり、新規作成（O_CREAT）や書き込み権限（O_WRONLY）は不要です
	else if (node->kind == ND_REDIR_APPEND)
		node->filefd = open(node->filename->word, O_CREAT | O_WRONLY | O_APPEND, 0644);
	else if (node->kind == ND_REDIR_HEREDOC)
		node->filefd = read_here_document(node->delimiter->word);
	else
		assert_error("open_redir_file");
	if (node->filefd < 0)
	{
		xperror(node->filename->word);
		return (-1);
	}
	node->filefd = stash_fd(node->filefd);
	return (open_redirect_file(node->next));
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
		assert_error("do_redirect");
	do_redirect(redirects->next);
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
		assert_error("reset_redirect");
}
