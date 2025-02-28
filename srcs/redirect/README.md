# Redirect

## 目次
- [open_file.c](#open_filec)

## open_file.c

1. パイプライン(ND_PIPELINE)
	- パイプの左側のリダイレクトの処理(node->command)
	- パイプの右側のリダイレクトの処理(node->next)

2. シンプルコマンド(ND_SIMPLE_CMD)
	- "node->redirects"を再起的に処理する
	　リダイレクトノードが"NULL"でなければ、ファイルを開く処理が行われる。

3. リダイレクトノードの処理
	- 該当するリダイレクトのファイルを開く(process_open_file関数)
	- ファイル記述子の保存
	- 次のリダイレクトがあれば、処理される

4. process_open_file関数
	- 出力リダイレクト'>'(ND_REDIR_OUT)
		```
		fd = open(node->filename->word, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		```
		- O_CREAT: ファイルが存在しない場合は作成
		- O_WRONLY: 書き込み専用でオープン
		- O_TRUNC: ファイルが存在する場合は内容を切り詰める（上書き）
		- パーミッション 0644: 所有者は読み書き可、その他のユーザーは読み取りのみ
	- 入力リダイレクト'<'(ND_REDIR_IN)
		```
		fd = open(node->filename->word, O_RDONLY);
		```
		- O_RDONLY: 読み取り専用でオープン
	- 追記モードでの出力リダイレクト">>"(ND_REDIR_APPEND)
		```
		fd = open(node->filename->word, O_CREAT | O_WRONLY | O_APPEND, 0644);
		```
		- O_APPEND: 既存ファイルの末尾に追記
	- ヒアドキュメント"<<"(ND_REDIR_HEREDOC)
		```
		fd = read_here_document(node->delimiter->word);
		```
		- read_here_document関数を呼び出してヒアドキュメントを処理
