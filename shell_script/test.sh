#!/bin/bash
RED="\033[31m"
GREEN="\033[32m"
BLUE="\033[96m"
RESET="\033[0m"
OK=$GREEN"OK"$RESET
NG=$RED"NG"$RESET

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

cleanup() {
	rm -f cmp out a.out print_args
}

assert() {
	COMMAND="$1"
	shift
	printf '%-50s:' "[$COMMAND]"
	# exit status
	echo -n -e "$COMMAND" | bash >cmp 2>&-
	expected=$?
	for arg in "$@"
	do
		mv "$arg" "$arg"".cmp"
	done
	echo -n -e "$COMMAND" | ./minishell >out 2>&-
	actual=$?
	for arg in "$@"
	do
		mv "$arg" "$arg"".out"
	done

	diff cmp out >/dev/null && echo -e -n "  diff $OK" || echo -e -n "  diff $NG"

	if [ "$actual" = "$expected" ]; then
		echo -e -n "  status $OK"
	else
		echo -e -n "  status $NG, expected $expected but got $actual"
	fi
	for arg in "$@"
	do
		echo -n "  [$arg] "
		diff "$arg"".cmp" "$arg"".out" >/dev/null && echo -e -n "$OK" || echo -e -n "$NG"
		rm -f "$arg"".cmp" "$arg"".out"
	done
	echo
}

# Empty line (EOF)
echo
echo -e "${BLUE}Empty line (EOF)${RESET}"
assert ''
echo 

# Absolute path commands without args 
echo -e "${BLUE}Absolute path commands without args${RESET}"
assert '/bin/pwd'
assert '/bin/echo'
assert '/bin/ls'
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
echo

## double quote
echo -e "${BLUE}double quote${RESET}"
assert './print_args "hello   world" "42Tokyo"'
assert 'echo "hello   world" "42Tokyo"'
assert "echo \"'hello   world'\" \"42Tokyo\""
echo

## combination
echo -e "${BLUE}combination${RESET}"
assert "echo hello'      world'"
assert "echo hello'  world  '\"  42Tokyo  \""
echo

# Redirect
## Redirecting output
echo -e "${BLUE}Redirecting output${RESET}"
assert 'echo hello >hello.txt' 'hello.txt'
assert 'echo hello >f1>f2>f3' 'f1' 'f2' 'f3'
echo

## Redirecting input
echo -e "${BLUE}Redirecting input${RESET}"
assert 'cat <Makefile'
echo hello >f1
echo world >f2
echo 42Tokyo >f3
assert 'cat <f1<f2<f3'
rm -f f1 f2 f3
assert 'cat <hoge'
echo

## Appending Redirected output
echo -e "${BLUE}Appending Redirected output${RESET}"
assert 'pwd >>pwd.txt' 'pwd.txt'
assert 'pwd >>pwd.txt \n pwd >>pwd.txt' 'pwd.txt'
echo

## Here Document
echo -e "${BLUE}Here Document${RESET}"
assert 'cat <<EOF\nhello\nworld\nEOF\nNOPRINT'
assert 'cat <<EOF<<eof\nhello\nworld\nEOF\neof\nNOPRINT'
assert 'cat <<EOF\nhello\nworld'
assert 'cat <<E"O"F\nhello\nworld\nEOF\nNOPRINT'
echo


cleanup