# Pipe

## 目次
	- [pipe.c の概要](#pipec-の概要)
	- [実行過程の例](#実行過程の例)

## pipe.c の概要
1. パイプの実行(exec/exec.c)
	```c
	pid_t execute_pipe(t_node *node)
	{
	    pid_t pid;

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
	```
	- ノードが存在する場合、必要に応じて新しいパイプを作成(create_new_pipe関数)
	- fork()で新しいプロセスを作成
	- 子プロセスの場合(pid == 0)
		- シグナルハンドラをリセット
		- パイプを設定(process_child_pipe関数)
	- 親プロセスの場合
		- パイプを閉じる(process_parent_pipe関数)
		- 次のノードがある場合、再帰的に処理
		- 最後ノードの場合、最後のコマンドのpidを返す

2. パイプの作成(create_new_pipe関数)
	```c
	void create_new_pipe(t_node *node)
	{
	    if (node->next == NULL)
	        return;
	    ft_pipe(node->outpipe);
	    node->next->inpipe[0] = node->outpipe[0];
	    node->next->inpipe[1] = node->outpipe[1];
	}
	```
	- パイプラインの最後のコマンドにはパイプは不要なので、次のノードがない場合は何もせず終了する
	- 次のノードが存在する場合、新しいパイプを作成する。作成したパイプは読み取り側と書き込み側を、次のノードの入力パイプとして設定。これにより、現在のコマンドの出力が次のコマンドの入力に接続される。

3. 子プロセス用のパイプ処理(rocess_child_pipe関数)
	```c
	void process_child_pipe(t_node *node)
	{
	    ft_close(node->outpipe[0]);
	    ft_dup2(node->inpipe[0], STDIN_FILENO);
	    if (node->inpipe[0] != STDIN_FILENO)
	        ft_close(node->inpipe[0]);
	    ft_dup2(node->outpipe[1], STDOUT_FILENO);
	    if (node->outpipe[1] != STDOUT_FILENO)
	        ft_close(node->outpipe[1]);
	}
	```
	- ft_close(node->outpipe[0]): 出力パイプの読み取り側を閉じます。子プロセスは書き込みだけを行うため、読み取り側は不要です。
	- ft_dup2(node->inpipe[0], STDIN_FILENO): 入力パイプの読み取り側を標準入力に複製します。これにより、前のコマンドの出力がこのコマンドの標準入力になります。
	- 入力パイプが標準入力でない場合は、元の入力パイプのファイルディスクリプタを閉じます（複製後は不要）。
	- ft_dup2(node->outpipe[1], STDOUT_FILENO): 出力パイプの書き込み側を標準出力に複製します。これにより、このコマンドの出力が次のコマンドの入力になります。
	- 出力パイプが標準出力でない場合は、元の出力パイプのファイルディスクリプタを閉じます（複製後は不要）。

4. 親プロセス用のパイプ処理 (process_parent_pipe関数)
	```c
	void process_parent_pipe(t_node *node)
	{
	    if (node->inpipe[0] != STDIN_FILENO)
	        ft_close(node->inpipe[0]);
	    if (node->next)
	        ft_close(node->outpipe[1]);
	}
	```
	- 入力パイプが標準入力でない場合は閉じます。親プロセスはこのパイプを使用しないためです。
	- 次のノードが存在する場合、出力パイプの書き込み側を閉じます。これは重要な操作で、親プロセスが書き込み側を開いたままにしていると、子プロセスが終了しても読み取り側がEOF（ファイル終端）を検知できないためです。

## 実行過程の例

```bash
ls | grep "file" | wc -l
```
はパイプ（|）を使って3つのコマンドを連結する典型的な例です。

1. 最初のコマンド（ls）
	- 親プロセスは create_new_pipe() を呼び出し、パイプAを作成します
	- fork() で子プロセスを作成します
	- 子プロセスは:
		- process_child_pipe() を呼び出して、標準出力(STDOUT)をパイプAの書込側[1]に接続します
		- ls コマンドを実行し、結果がパイプAに書き込まれます
	- 親プロセスは:
		- process_parent_pipe() でパイプAの書込側[1]を閉じます（重要）
		- 次のノード(grep)の処理に進みます

2. 中間のコマンド（grep "file"）
	- 親プロセスは create_new_pipe() を呼び出し、パイプBを作成します
	- fork() で子プロセスを作成します
	- 子プロセスは:
		- process_child_pipe() を呼び出し、標準入力(STDIN)をパイプAの読取側[0]に接続します
		- 標準出力(STDOUT)をパイプBの書込側[1]に接続します
		- grep "file" コマンドを実行し、パイプAからデータを読み取り、"file"を含む行だけをパイプBに書き込みます
	- 親プロセスは:
		- process_parent_pipe() でパイプAの読取側[0]とパイプBの書込側[1]を閉じます
		- 次のノード(wc)の処理に進みます

3. 最後のコマンド（wc -l）
	- 親プロセスは create_new_pipe() を呼び出しますが、次のノードがないのでパイプは作成しません
	- fork() で子プロセスを作成します
	- 子プロセスは:
		- process_child_pipe() を呼び出し、標準入力(STDIN)をパイプBの読取側[0]に接続します
		- 標準出力(STDOUT)はそのままターミナルに接続されています
		- wc -l コマンドを実行し、パイプBからデータを読み取り、行数を数えて出力します
	- 親プロセスは:
		- process_parent_pipe() でパイプBの読取側[0]を閉じます
		- 全ての子プロセスの完了を待ちます
