# Parse

1. パイプライン解析:コマンドのパイプライン（'|'で接続されたコマンド群）を解析
	- 新しいパイプラインノードを作成
	- パイプの入出力ファイル記述子を初期化
	- (左側の)シンプルコマンドを解析
	- 次のトークンが'|'の場合、再帰的に次のパイプラインを解析

	処理例
	```bash
	入力: ls -l | grep ".c" | wc -l
	```
	```s
	parse_pipeline()
	├── ND_PIPELINE
	│   ├── command: simple_command("ls -l")
	│   ├── next:
	│       ├── ND_PIPELINE
	│       │   ├── command: simple_command("grep \".	c\"")
	│       │   ├── next:
	│       │       ├── ND_PIPELINE
	│       │       │   ├── command: simple_command("wc -l")
	```


2. シンプルコマンド(引数とリダイレクションを含む単一のコマンド)の解析
	- 新しいシンプルコマンドノードを作成
	- コマンド要素（引数、リダイレクション）を追加
		- 通常の引数（TK_WORD）の追加
		- 各リダイレクションの追加
	- 制御演算子('|', '\n', "EOF")に遭遇するまで、コマンド要素の追加を継続

3. コマンド要素の追加
	- 単語(TK_WORD)ならコマンドの引数リストに追加
	- '>', '<', ">>", "<<" なら、対応するリダイレクトノードを作成して追加
	- 無効なトークンの場合、エラー処理

	処理例
	```bash
	入力: echo hello > output.txt
	```
	```s
	add_cmd_elm()
	├── args: ["echo", "hello"]
	├── redirects: ND_REDIR_OUT ("output.txt")
	```
