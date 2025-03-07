#!/bin/bash
RED="\033[31m"
GREEN="\033[32m"
BLUE="\033[96m"
YELLOW="\033[33m"
RESET="\033[0m"
OK=$GREEN"OK"$RESET
NG=$RED"NG"$RESET

# 引数処理
RUN_TEST="$1"  # 最初の引数を取得

cat <<EOF | gcc -xc -o a.out -
#include <stdio.h>
int main() { printf("hello from a.out\n"); }
EOF

cat <<EOF | gcc -xc -o print_args -
#include <stdio.h>
int main(int argc, char *argv[]) {
	for (int i = 0; argv[i]; i++)
		printf("argv[%d] = %s\n", i, argv[i]);
}
EOF

cat <<EOF | gcc -xc -o exit42 -
int main() { return 42; }
EOF

# cleanup() {
# 	rm -f cmp out a.out print_args exit42
# print_desc() {
# 	echo -e $YELLOW"$1"$RESET
# }

print_desc() {
	echo -e $YELLOW"$1"$RESET
}

cleanup() {
	rm -f cmp out a.out print_args exit42 infinite_loop
}

NG_LIST=()  # NG の項目を保存する配列

assert() {
	COMMAND="$1"
	shift
	printf '%-50s:' "[$COMMAND]"
	# exit status
	echo -n -e "$COMMAND" | bash >cmp 2>&-
	EXPECTED=$YELLOW$?$RESET
	for arg in "$@"
	do
		mv "$arg" "$arg"".cmp"
	done
	echo -n -e "$COMMAND" | ./minishell >out 2>&-
	ACTUAL=$YELLOW$?$RESET
	for arg in "$@"
	do
		mv "$arg" "$arg"".out"
	done

	NG_FLAG=0  # 1ならNGがあったと判断

	# diffの結果をチェック
	if diff cmp out >/dev/null; then
		echo -e -n "  diff $OK"
	else
		echo -e -n "  diff $NG"
		NG_LIST+=("[$COMMAND] diff NG")  # NGの情報を配列に追加
		NG_FLAG=1
	fi

	# ステータスコードの比較
	if [ "$ACTUAL" = "$EXPECTED" ]; then
		echo -e -n "  status $OK"
	else
		echo -e -n "  status $NG"
		NG_LIST+=("[$COMMAND] status NG (Expected: $EXPECTED, Actual: $ACTUAL)")
		NG_FLAG=1
	fi
	echo -e ", bash status $EXPECTED, my_shell status $ACTUAL"

	# ファイルのdiffチェック
	for arg in "$@"
	do
		# ファイルの内容を表示する
		# if [ -f "$arg"".cmp" ]; then
		# 	echo "===== Bash output for $arg ====="
		# 	cat "$arg"".cmp"
		# 	echo "===== End of bash output ====="
		# fi

		# if [ -f "$arg"".out" ]; then
		# 	echo "===== Minishell output for $arg ====="
		# 	cat "$arg"".out"
		# 	echo "===== End of minishell output ====="
		# fi

		echo -n "  [$arg] "
		if diff "$arg"".cmp" "$arg"".out" >/dev/null; then
			echo -e -n "$OK"
		else
			echo -e -n "$NG"
			NG_LIST+=("[$COMMAND] File diff NG for $arg")
			NG_FLAG=1
		fi
		rm -f "$arg"".cmp" "$arg"".out"
	done
	echo

	# NGがあれば配列に記録
	if [ $NG_FLAG -eq 1 ]; then
		NG_LIST+=("------------------------------------------------")
	fi
}

# 全テスト終了後に NG の一覧を出力
summary() {
	if [ ${#NG_LIST[@]} -ne 0 ]; then
		echo -e "\n${RED}====== NG Summary ======${RESET}"
		for item in "${NG_LIST[@]}"; do
			echo -e "$RED$item$RESET"
		done
	else
		echo -e "\n${GREEN}All tests passed!${RESET}"
	fi
}

# スクリプトの最後に summary を実行
trap summary EXIT


# Empty line (EOF)
echo
echo -e "${BLUE}Empty line (EOF)${RESET}"
assert ''
assert ' '
echo

# Absolute path commands without args
echo -e "${BLUE}Absolute path commands without args${RESET}"
assert '/bin/pwd'
assert '/bin/echo'
assert '/bin/ls'
assert '/bin/ll'
echo

# Search command path without args
echo -e "${BLUE}Search command path without args${RESET}"
assert 'pwd'
assert 'echo'
assert 'ls'
assert './a.out'
echo

## no such command
echo -e "${BLUE}no such command${RESET}"
assert 'a.out'
assert 'nosuchfile'
echo

# Tokenize
## unquoted word
echo -e "${BLUE}unquoted word${RESET}"
assert 'ls /'
assert 'echo hello    world     '
assert 'nosuchfile\n\n'
echo

## single quote
echo -e "${BLUE}single quote${RESET}"
assert "./print_args 'hello   world' '42Tokyo'"
assert "echo 'hello   world' '42Tokyo'"
assert "echo '\"hello   world\"' '42Tokyo'"
# assert "\'"
echo

## double quote
echo -e "${BLUE}double quote${RESET}"
assert './print_args "hello   world" "42Tokyo"'
assert 'echo "hello   world" "42Tokyo"'
assert "echo \"'hello   world'\" \"42Tokyo\""
assert "echo hello'      world'"
assert "echo hello'  world  '\"  42Tokyo  \""
echo

## meta chara
echo -e "${BLUE}meta chara${RESET}"
assert ">"
assert ">>"
assert "|"
assert "||"
echo

# Redirect
# Redirecting output
echo -e "${BLUE}Redirecting output${RESET}"
assert 'echo hello >hello.txt' 'hello.txt'
assert 'echo world> hello.txt' 'hello.txt'
assert 'echo hello >f1>f2>f3' 'f1' 'f2' 'f3'
assert 'echo "" > f4' 'f4'
assert 'echo '\"\"' > f5' 'f5'
assert 'echo >f6' 'f6'
echo 42 > f7
chmod 400 f7
echo -e "-- chmod 400 --"
assert 'echo Tokyo > f7'
chmod 777 f7
echo -e "-- chmod 777 --"
assert 'echo Tokyo > f7' 'f7'
rm -f f4 f5 f6 f7
echo

## Redirecting input
echo -e "${BLUE}Redirecting input${RESET}"
assert 'cat <Makefile'
assert 'cat <'
echo hello >f1
echo world >f2
echo 42Tokyo >f3
echo "" >f4
assert 'cat <f1<f2<f3<f4'
assert 'cat< f1 <   f2  <       f3      '
assert 'cat <f1<f10<f2<f3'
assert 'cat <f1<'
assert 'cat <hoge'
echo SUCCESS > f5
chmod 000 f5
echo -e "-- chmod 000 --"
assert 'cat < f5'
chmod 777 f5
echo -e "-- chmod 777 --"
assert 'cat < f5'
rm -f f1 f2 f3 f4 f5
assert 'cat</etc/passwd'
assert 'cat < /etc/passwd < /etc/hosts'
echo

## Appending Redirected output
echo -e "${BLUE}Appending Redirected output${RESET}"
assert 'pwd >>pwd.txt' 'pwd.txt'
assert 'pwd >>pwd.txt \n pwd >>pwd.txt' 'pwd.txt'
# テストではうまくいかないので、実際に動作で確認
# echo hello >f1
# assert 'echo world >>f1' 'f1'
# assert 'echo hello >>f2' 'f2'
# assert 'echo hello>>f3' 'f3'
# assert 'echo "" >>f2' 'f2'
# assert 'echo "" >>f4' 'f4'
# assert 'echo >>f2' 'f2'
# echo 42 >f7
# chmod 400 f7
# echo -e "-- chmod 400 --"
# assert 'echo Tokyo >> f7'
# assert 'cat <f7'
# chmod 777 f7
# echo -e "-- chmod 777 --"
# assert 'echo Tokyo >> f7' 'f7'
# rm -f f1 f2 f3 f4 f7

echo

## Here Document
echo -e "${BLUE}Here Document${RESET}"
assert 'cat <<EOF\nhello\nworld\nEOF\nNOPRINT'
assert 'cat <<EOF<<eof\nhello\nworld\nEOF\neof\nNOPRINT'
assert 'cat <<EOF<<eof\nhello\nworld\n  EOF\nNOPRINT'
assert 'cat <<EOF<<eof\nhello\nworld\nEOF  \nNOPRINT'
assert 'cat <<EOF\nEOF\nNOPRINT'
assert 'cat <<EOF\nhello\nworld'
assert 'cat <<E"O"F\nhello\nworld\nEOF\nNOPRINT'
assert 'cat <<EOF\n$USER\nEOF\nNOPRINT'
assert 'cat <<'\''EOF'\''\n$USER\nEOF\nNOPRINT'
assert 'cat <<"EOF"\n$USER\nEOF\nNOPRINT'
assert 'cat <<E"O"F\n$USER\nEOF\nNOPRINT'
assert 'cat <<"E"'\''O'\''F\n$USER\nEOF\nNOPRINT'
assert 'cat <<EOF | grep "hello"\nhello world\ngoodbye world\nEOF\nNOPRINT'
assert 'cat <<EOF\n'\\'\nEOF\nNOPRINT'
assert 'cat <<EOF\n'\$'USER\n$USER\nEOF\nNOPRINT'
assert 'cat <<A\nsingle char delim\nA\nNOPRINT'
assert 'cat <<E@Fspecial chars delim\nE@F\nNOPRINT'
assert 'cat <<EOF1 <<EOF2\nabc\ndef\nEOF1\nEOF2\nNOPRINT'
echo

## Pipe
echo -e "${BLUE}Pipe${RESET}"
assert 'cat Makefile | grep minishell'
assert 'cat | cat | ls\n\n'
assert 'ls |'
assert 'echo hello | cat'
assert 'echo hello | cat | cat | cat | cat'
assert 'ls -la | grep "a" | grep "b" | grep "c"'
assert 'ls | sort | head -n 3'
assert '| ls'
assert 'ls ||'
assert 'ls | | grep a'
assert './exit42 | echo $?'
assert 'echo hello | invalid_command'
assert 'invalid_command | echo hello'
assert 'cat /etc/passwd | grep root | wc -l'
assert 'cd .. | pwd'  # ビルトインとパイプの組み合わせ
assert 'export TEST=hello | echo $TEST'  # 環境変数とパイプ
assert 'echo hello > file.txt | cat file.txt' 'file.txt'
assert 'cat < Makefile | grep include'
assert 'cat <<EOF | wc -l\nhello\nworld\nEOF\nNOPRINT'
assert 'echo hello | cat > pipe_output.txt' 'pipe_output.txt'
assert './print_args "hello | world" | cat'  # クォート内のパイプ記号
echo

## Expand Variable
echo -e "${BLUE}Expand Variable${RESET}"
assert 'echo $USER'
assert 'echo $USER$PATH$TERM'
assert 'echo "$USER  $PATH   $TERM"'
assert 'echo $UNDEFINED_VARIABLE'
assert 'echo "$UNDEFINED_VARIABLE"'
assert 'echo "$USER$UNDEFINED_VARIABLE"'
assert 'echo "$USER $"'
# assert 'echo "$USER$$"'  # 本家:PIDの取得 -> 未実装
assert 'echo "$USER$?"'
assert 'echo "Hello $USER, your PATH is $PATH"'
assert 'echo '\''$USER'\''' 
assert 'echo "$USER"'\''$PATH'\'''
# assert 'echo "$USER""\$PATH"'  # ダブルクォート内のエスケープ
assert 'export TEST=value\necho $TEST\nunset TEST\necho $TEST'
assert 'export TEST="$USER"\necho $TEST'
assert 'export TEST=$UNDEFINED\necho $TEST'
assert 'export TEST="hello world"\necho "$TEST"'
assert 'export TEST1=hello\nexport TEST2=$TEST1\necho $TEST2'
assert 'export TEST="$USER   $PATH"\necho $TEST'
assert 'echo $TEST$?$USER'  # 変数と特殊パラメータの組み合わせ
assert '$ECHO hello'  # 存在しないコマンド
assert '$USER'  # 存在しないコマンド（環境変数のみ）
assert 'export PATH=/bin\nls\nexport PATH=\nls'  # PATHの変更
# assert 'cat $FILE'  # 存在しないファイル
# assert 'echo hello > $OUTFILE' 'variable_output.txt'  # リダイレクトでの変数展開
echo

## Special Parameter $?
echo -e "${BLUE}Special Parameter${RESET}"
assert 'echo $?'
assert 'invalid\necho $?\necho $?'
assert 'exit 42\necho $?\necho $?'
assert 'exit 42\n\necho $?\necho $?'
assert 'true\necho $?'  # 成功した場合のステータスコード
assert 'false\necho $?'  # 失敗した場合のステータスコード
assert 'exit100\necho $?'  # 終了ステータスが100
assert '/bin/true\necho $?'  # 絶対パスでのコマンド
assert 'cd /nonexistent\necho $?'  # 存在しないディレクトリへのcd
assert 'echo hello > /dev/full\necho $?'  # 書き込み失敗
echo SUCCESS > f5
chmod 000 f5
echo -e "-- chmod 000 --"
assert 'cat < f5\necho $?'
chmod 777 f5
echo -e "-- chmod 777 --"
assert 'cat < f5\necho $?'
rm -f f5
assert 'echo $?\n/bin/false\necho $?'  # 連続したエラーコードの確認
assert 'echo "$?"'  # ダブルクォート内での$?
assert 'echo '\''$?'\'''  # シングルクォート内での$?
assert 'echo hello | grep world\necho $?'  # パイプラインでのエラーコード
assert 'echo hello | grep hello\necho $?'  # 成功するパイプライン
assert 'echo "Status: $?"'  # 文字列内での$?
assert 'export STATUS=$?\necho $STATUS'  # 変数への$?の代入
assert 'invalid_command 2>/dev/null\necho $?'  # エラー出力をリダイレクト
assert 'echo foo > bar 2>/dev/null\nrm bar\necho foo > bar 2>/dev/null\nchmod 000 bar\necho foo > bar 2>/dev/null\necho $?\nchmod 777 bar\nrm bar' # パーミッションエラー
assert './print_args $?\n./print_args "$?"'  # コマンドライン引数としての$?
echo

# Signal handling
echo "int main() { while (1) ; }" | gcc -xc -o infinite_loop -

## Signal to shell processes
print_desc "SIGTERM to SHELL"
(sleep 0.01; pkill -SIGTERM bash;
 sleep 0.01; pkill -SIGTERM minishell) &
assert './infinite_loop' 2>/dev/null # Redirect stderr to suppress signal terminated message

print_desc "SIGQUIT to SHELL"
(sleep 0.01; pkill -SIGQUIT bash; # SIGQUIT should not kill the shell
 sleep 0.01; pkill -SIGTERM bash;
 sleep 0.01; pkill -SIGQUIT minishell; # SIGQUIT should not kill the shell
 sleep 0.01; pkill -SIGTERM minishell) &
assert './infinite_loop' 2>/dev/null # Redirect stderr to suppress signal terminated message

print_desc "SIGINT to SHELL"
(sleep 0.01; pkill -SIGINT bash; # SIGINT should not kill the shell
 sleep 0.01; pkill -SIGTERM bash;
 sleep 0.01; pkill -SIGINT minishell; # SIGINT should not kill the shell
 sleep 0.01; pkill -SIGTERM minishell) &
assert './infinite_loop' 2>/dev/null # Redirect stderr to suppress signal terminated message

## Signal to child processes
print_desc "SIGTERM to child process"
(sleep 0.01; pkill -SIGTERM infinite_loop;
 sleep 0.01; pkill -SIGTERM infinite_loop) &
assert './infinite_loop'

print_desc "SIGINT to child process"
(sleep 0.01; pkill -SIGINT infinite_loop;
 sleep 0.01; pkill -SIGINT infinite_loop) &
assert './infinite_loop'

print_desc "SIGQUIT to child process"
(sleep 0.01; pkill -SIGQUIT infinite_loop;
 sleep 0.01; pkill -SIGQUIT infinite_loop) &
assert './infinite_loop'

print_desc "SIGUSR1 to child process"
(sleep 0.01; pkill -SIGUSR1 infinite_loop;
 sleep 0.01; pkill -SIGUSR1 infinite_loop) &
assert './infinite_loop'

# Manual Debug
# $ ./minishell
# $
# 1. Ctrl-\
# 2. Ctrl-C
# 3. Ctrl-D
#
# $ ./minishell
# $ hogehoge
# 1. Ctrl-\
# 2. Ctrl-C
# 3. Ctrl-D
#
# $ ./minishell
# $ cat <<EOF
# >
# 1. Ctrl-\
# 2. Ctrl-C
# 3. Ctrl-D
#
# $ ./minishell
# $ cat <<EOF
# > hoge
# > fuga
# 1. Ctrl-\
# 2. Ctrl-C
# 3. Ctrl-D



# Builtin
echo -e "${BLUE}Builtin${RESET}"
echo -e "${BLUE}exit${RESET}"
## exit
echo -e "${BLUE}exit${RESET}"
assert 'exit'
assert 'exit  '
assert 'exit 42'
assert 'exit 2147483647'
assert 'exit 2147483648'
assert 'exit 4294967295'
assert 'exit 4294967296'
assert 'exit 9223372036854775807'
assert 'exit 9223372036854775808'
assert 'exit 0'
assert 'exit +1'
assert 'exit -1'
assert 'exit 111111111111111111111111111111111111111111'
assert 'exit ""'
assert 'exit hello'
assert 'exit 42Tokyo'
assert 'exit 1 2'
assert 'exit 1 2 3'
assert 'ls | exit'
assert 'exit | ls'
echo

## export
echo -e "${BLUE}export${RESET}"
print_desc "Output of 'export' differs, but it's ok."
echo -e "${BLUE}昇順にはした\nminishellのexportは環境変数に実行ファイルが入るため、これでok${RESET}"
assert 'export' # order of variables, default variables differs...
assert 'export | grep nosuch | sort'
assert 'export nosuch\n export | grep nosuch | sort'
assert 'export nosuch=fuga\n export | grep nosuch | sort'
assert 'export nosuch=fuga hoge=nosuch\n export | grep nosuch | sort'
assert 'export [invalid]'
assert 'export [invalid_nosuch]\n export | grep nosuch | sort'
assert 'export [invalid]=nosuch\n export | grep nosuch | sort'
assert 'export [invalid] nosuch hoge=nosuch\n export | grep nosuch | sort'
assert 'export nosuch [invalid] hoge=nosuch\n export | grep nosuch | sort'
assert 'export nosuch hoge=nosuch [invalid]\n export | grep nosuch | sort'
assert 'export nosuch="nosuch2=hoge"\nexport $nosuch\n export | grep nosuch | sort'
echo

## env
echo -e "${BLUE}env${RESET}"
print_desc "Output of 'env' differs, but it's ok."
assert 'env' # order of variables, default variables differs...
assert 'env | grep hoge | sort'
echo

## cd
echo -e "${BLUE}cd${RESET}"
assert 'cd'
assert 'cd .'
assert 'cd ..'
assert 'cd ///'
assert 'cd /tmp'
assert 'cd /tmp/'
assert 'cd /tmp///'
assert 'cd /../../../././.././'
assert 'cd src'
# assert 'cd |'

assert 'cd \n echo $PWD'
unset HOME
assert 'cd \n echo $PWD'
assert 'cd .\n echo $PWD'
assert 'cd ..\n echo $PWD'
assert 'cd ///\n echo $PWD'
assert 'cd /tmp\n echo $PWD'
assert 'cd /tmp/\n echo $PWD'
assert 'cd /tmp///\n echo $PWD'
assert 'cd /../../../././.././\n echo $PWD'
assert 'cd src\n echo $PWD'
echo

## echo
echo -e "${BLUE}echo${RESET}"
assert 'echo'
assert 'echo hello'
assert 'echo hello "    " world'
assert 'echo -n'
assert 'echo -n hello'
assert 'echo -n hello world'
assert 'echo hello -n'
echo

## pwd
echo -e "${BLUE}pwd${RESET}"
assert 'pwd'
assert 'cd\npwd'
assert 'cd src\npwd'
assert 'cd /etc\npwd'
assert 'cd . \n pwd \n echo $PWD $OLDPWD'
assert 'cd .. \n pwd \n echo $PWD $OLDPWD'
assert 'cd /// \n pwd \n echo $PWD $OLDPWD'
assert 'cd /tmp/// \n pwd \n echo $PWD $OLDPWD'
assert 'unset PWD\npwd\ncd /etc\npwd'
echo

## unset
echo -e "${BLUE}unset${RESET}"
export hoge fuga=fuga
assert 'unset'
assert 'unset hoge'
assert 'unset ='
assert 'unset >'
assert 'unset []'
assert 'unset unset'
assert 'unset fuga'
assert 'unset nosuch'
assert 'unset [invalid]'
assert 'unset 123abc'    # 数字で始まる変数名（無効）
assert 'unset !invalid'  # `!` を含む変数名（無効）
assert 'unset foo;bar'   # `;` を含む（無効）
assert 'unset hoge=val'  # `=` を含む変数名（無効）
assert 'unset hoge fuga'
assert 'unset hoge nosuch fuga'
assert 'unset fuga \n export | echo $fuga'
assert 'unset [invalid] fuga \n echo $fuga'

cleanup
