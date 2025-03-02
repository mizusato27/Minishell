# Redirect

## 目次
- [open_file.c](#open_filec)
- [stash_fd.c](#stash_fdc)
- [redirect.c](#redirectc)
- [here_document.c](#here_documentc)

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
		```c
		fd = open(node->filename->word, O_CREAT | O_WRONLY | O_TRUNC, 0644);
		```
		- O_CREAT: ファイルが存在しない場合は作成
		- O_WRONLY: 書き込み専用でオープン
		- O_TRUNC: ファイルが存在する場合は内容を切り詰める（上書き）
		- パーミッション 0644: 所有者は読み書き可、その他のユーザーは読み取りのみ

	- 入力リダイレクト'<'(ND_REDIR_IN)
		```c
		fd = open(node->filename->word, O_RDONLY);
		```
		- O_RDONLY: 読み取り専用でオープン

	- 追記モードでの出力リダイレクト">>"(ND_REDIR_APPEND)
		```c
		fd = open(node->filename->word, O_CREAT | O_WRONLY | O_APPEND, 0644);
		```
		- O_APPEND: 既存ファイルの末尾に追記

	- ヒアドキュメント"<<"(ND_REDIR_HEREDOC)
		```c
		fd = read_here_document(node->delimiter->word);
		```
		- read_here_document関数を呼び出してヒアドキュメントを処理

## stash_fd.c

与えられたファイル記述子を「安全な場所」に複製する関数

1. is_valid_fd関数
	- 渡されたファイル記述子が有効かをチェックする
	- 無効な場合、"EBADF"(Bad File Descripter)エラーを設定

2. statsh_fd関数
	- 新しいファイル記述子に、10以降の有効な記述子を設定
	- ft_dup2関数で元のファイル記述子に複製

3. fdに10を加える意図
	- 標準ファイル記述子との衝突回避:
	 UNIXシステムでは、0（標準入力）、1（標準出力）、2（標準エラー出力）は特別な意味を持ちます。また、多くのプログラムは3〜9の番号も使用することがあります。

	- シェルのリダイレクト機能の実装:
	 シェルでリダイレクトを実装する際には、標準入出力のファイル記述子（0, 1, 2）を一時的に変更しますが、元の状態に戻せるように元のファイル記述子を保存しておく必要があります。

	- 安全な番号範囲の確保:
	 10以上の値を使うことで、シェル自身やコマンドが通常使用する低い番号のファイル記述子と重複するリスクを減らします。

## redirect.c

- ファイル記述子
	- filefd : リダイレクト先（または元）のファイルのファイルディスクリプタを保持

	- targetfd : リダイレクトによって変更される標準の入出力ストリームのファイルディスクリプタを示す

	- stashed_targetfd : リダイレクション前の元の状態を保存するためのファイルディスクリプタ。後で元の状態に戻すために使用される。

- setup_redirect関数
	1. 元のファイル記述子(targetfd)を"stashed_targetfd"に複製
	2. dup2関数を使用して、ファイル記述子の向き先を変更(targetfdがfilefdを指すよう変更)
	3. 再帰的に次のリダイレクトノードを処理

- reset_redirect関数
	1. 再帰的に次のリダイレクトノードを先に処理
	2. リダイレクトに使用したファイル記述子を閉じる
	3. リダイレクト前のファイル記述子の状態を復元する(stashed_targetfdに保存しておいた元の記述子をtargetfdに復元)

# here_document.c
