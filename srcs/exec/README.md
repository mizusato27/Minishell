# Exec

## 目次
- [全体の実行フロー](#全体の実行フロー)
- [interpret_cmd](#interpret_cmd)
- [execute_cmd](#execute_cmd)
- [wait_pipe](#wait_pipe)
- [詳細](#詳細)

## 全体の実行フロー
Minishellの実行過程は、大まかに以下のステップで進みます：
1. ユーザーからの入力（コマンドライン）を受け取る
2. トークン化（tokenize）して字句解析を行う
3. 構文解析（parse）してコマンド構造を構築する
4. 変数展開（expand）を行う
5. コマンドを実行する（execute_cmd）
6. 実行結果（終了ステータス）を保存する

## interpret_cmd
1. tokenize(line) - 入力文字列をトークン（t_token構造体）に分解します
	- トークンは、単語（TK_WORD）、予約語（TK_RESERVED）、演算子（TK_OP）などに分類されます

2. at_eof(tok) - EOFトークンかどうかをチェックします（入力が空でないか確認）

3. トークン化でのエラーチェック：
	- g_ctx.g_syntax_errorフラグが立っていれば、トークン化エラーとしてステータスを設定

4. parse(tok) - トークンを構文解析して抽象構文木（t_node構造体）を構築します
	- パイプライン（ND_PIPELINE）、単純コマンド（ND_SIMPLE_CMD）、リダイレクト（ND_REDIR_*）などのノードが作成されます

5. 構文解析でのエラーチェック：
	- g_ctx.g_syntax_errorフラグが立っていれば、構文解析エラーとしてステータスを設定

6. expand(node) - 変数展開などを行います（環境変数の$記号や引用符の処理）

7. execute_cmd(node) - コマンドを実行します
	- 実行結果はステータスコードとして返され、last_statusに保存されます

## execute_cmd
1. open_redirect_file(node) - リダイレクト用のファイルを開きます
	- エラーが発生した場合、リダイレクトエラーを返します

2. ビルトインコマンドのチェックと実行：
	- パイプがなく（node->next == NULL）、ビルトインコマンド（is_builtin(node)）の場合、exec_builtin(node) を呼び出して直接実行します

3. 外部コマンドまたはパイプライン処理：
	- execute_pipe(node) を呼び出してコマンドを実行し、最後のプロセスIDを取得
	- wait_pipe(pid) を呼び出して、すべての子プロセスの終了を待ち、最後のプロセスの終了ステータスを取得

## execute_pipe
1. create_new_pipe(node) - 新しいパイプを作成し、node->outpipe に設定します

2. fork() で子プロセスを作成します：

3. 子プロセス処理（pid == 0の場合）：
	- reset_signal() - シグナルハンドラをリセット
	- process_child_pipe(node) - 標準入出力をパイプにリダイレクト
	- コマンド実行：
		- ビルトインコマンドの場合: exec_builtin(node) を実行して終了
		- 外部コマンドの場合: exec_nonbuiltin(node) を実行（execveなどを呼び出す）

4. 親プロセス処理：
	- process_parent_pipe(node) - 使用済みのパイプをクローズ

5. 再帰的な処理：
	- node->next がある場合、次のコマンドのために execute_pipe(node->next) を呼び出す
	- 最後には最終コマンドの pid を返す

パイプライン処理の際には、以下の構造体のフィールドが重要な役割を果たします：
- node->inpipe - 入力パイプ（前のコマンドからの出力を受け取る）
- node->outpipe - 出力パイプ（次のコマンドへの入力として使われる）

## wait_pipe
1. wait(&wstatus) (wait.hから) - 子プロセスが終了するのを待ち、そのプロセスIDと終了情報を取得します
	- wstatus には子プロセスの終了情報が格納されます

2. 終了したプロセスが最後のプロセス（pid）の場合：
	- WIFSIGNALED(wstatus) - プロセスがシグナルで終了したかをチェック
	- WTERMSIG(wstatus) - 終了の原因となったシグナル番号を取得
	- WEXITSTATUS(wstatus) - 通常終了の場合の終了コードを取得
	シグナルで終了した場合は 128 + シグナル番号 を返す（シェルの慣習）

3. wait がエラーを返した場合（result < 0）:
	- errno == ECHILD - 待つべき子プロセスがない場合、処理を終了
	- errno == EINTR - シグナルによって中断された場合、再度waitを呼び出す
	- その他のエラーの場合、致命的エラーとして処理

4. すべての子プロセスが終了したら、最後のプロセスの終了ステータスを返します

## 詳細
1. wait.h 関連
	- wait(&wstatus) - 任意の子プロセスが終了するまで待機し、終了したプロセスのIDを返す
	- WIFSIGNALED(wstatus) - 子プロセスがシグナルにより異常終了した場合にtrueを返すマクロ
	- WTERMSIG(wstatus) - 子プロセスを終了させたシグナル番号を返すマクロ
	- WEXITSTATUS(wstatus) - 子プロセスの終了ステータスを返すマクロ（0-255の値）

2. error.h 関連
	- errno - システムコールがエラーを返した場合にセットされるエラー番号
	- ECHILD - 待つべき子プロセスが存在しないエラー（すべての子プロセスが既に終了）
	- EINTR - システムコールがシグナルにより中断されたエラー
