/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:30:52 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/15 15:37:37 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/stat.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef PATH_MAX
#  define PATH_MAX 10000
# endif

// error_message
# define ER_ACCESS "command not found"
# define ER_EXEC "execve failed\n"
# define ER_ADD_CMD "add_cmd_elm error"
# define ER_CLOSE "close error"
# define ER_DUP2 "dup2 error"
# define ER_EXECVE "execve error"
# define ER_FILE "invalid file"
# define ER_FORK "fork error"
# define ER_CALLOC "calloc"
# define ER_REALLOC "realloc"
# define ER_STRDUP "strdup"
# define ER_MALLOC "malloc error"
# define ER_OVERFLOW "overflow"
# define ER_PIPE "pipe error"
# define ER_RESET_REDIR "reset_redirect error"
# define ER_SETUP_REDIR "setup_redirect error"
# define ER_SYNTAX_ERROR "syntax error"
# define ERROR_PREFIX "minishell: "
# define ERROR_BAD_CHAR \
	"Variable must starts with alphabetic \
character or underscore."

# define OPERATORS "|\n"
# define METAS "|<> \t\n"
# define SINGLE_QUOTE '\''
# define DOUBLE_QUOTE '\"'

# define APPEND_CHAR_SIZE 1
# define CHILD_PROCESS 0
# define END_CHAR_SIZE 1
# define ERROR_TOKENIZE 258
# define ERROR_PARSE 258
# define ERROR_OPEN_REDIR 1
# define ERROR_UNSET 1
# define ERROR_CMD 127

typedef struct s_token			t_token;
enum							e_token_kind
{
	TK_WORD,
	TK_RESERVED,
	TK_OP,
	TK_EOF,
};
typedef enum e_token_kind		t_token_kind;

// `word` is zero terminated string.
struct							s_token
{
	char						*word;
	t_token_kind				kind;
	t_token						*next;
};

enum							e_node_kind
{
	ND_PIPELINE,
	ND_SIMPLE_CMD,
	ND_REDIR_OUT,
	ND_REDIR_IN,
	ND_REDIR_APPEND,
	ND_REDIR_HEREDOC,
};
typedef enum e_node_kind		t_node_kind;
typedef struct s_node			t_node;
struct							s_node
{
	t_node_kind					kind;
	t_node						*next;
	t_token						*args;
	t_node						*redirects;
	int							targetfd;
	t_token						*filename;
	t_token						*delimiter;
	bool						is_delim_quoted;
	int							filefd;
	int							stashed_targetfd;
	int							inpipe[2];
	int							outpipe[2];
	t_node						*command;
};

typedef struct s_map			t_map;
typedef struct s_item			t_item;
struct							s_item
{
	char						*name;
	char						*value;
	t_item						*next;
};

// map->item_headにはダミーのアイテムが入っている
//ダミーノードの利点は、リストの先頭と末尾の処理を簡単にすることができる
// map->item_head.nextには最初のアイテムが入っている
struct							s_map
{
	t_item						item_head;
};

// g_rl_intr: readlineの割り込みフラグ
// syntax_error: 構文エラーが発生したかどうかのフラグ
// g_status: 最後に実行したコマンドの終了ステータス
// g_envmap: 環境変数を格納するマップ
// g_sig: シグナルの種類

// extern int						g_status;
extern bool						g_rl_intr;
extern bool						g_syntax_error;
extern volatile sig_atomic_t	g_sig;
// extern t_map					*g_envmap;

// -------------------- BUILTIN --------------------
// builtin_utils.c
int								chdir_ex(char *path);
int								map_set_value_ex(t_map *envmap,
									const char *name, const char *pwd);
int								process_minus_option(t_map *envmap,
									char *old_path, char *current_pwd);
int								cpy_home_path(t_map *envmap, char *path,
									char *arg);
// builtin.c
int								exec_builtin(t_map *envmap, t_node *node,
									int *status);
bool							is_builtin(t_node *node);
// cd.c
int								builtin_cd(t_map *envmap, char **argv);
// echo.c
int								builtin_echo(char **argv);
// env.c
int								builtin_env(t_map *envmap);
// exit.c
int								builtin_exit(char **argv, int *status);
// export.c
int								builtin_export(t_map *envmap, char **argv);
// pwd.c
int								builtin_pwd(t_map *envmap);
// unset.c
int								builtin_unset(t_map *envmap, char **argv);

// -------------------- ENV --------------------
// env.c
t_map							*initenv(void);
char							**get_environ(t_map *envmap);
// map_get_value.c
char							*map_get_value(t_map *envmap, const char *name);
// map_set_value.c
int								map_set_value(t_map *envmap, const char *name,
									const char *value);
// map_helper.c
char							*xgetenv(t_map *envmap, const char *name);
bool							is_alpha_under(char c);
bool							is_alpha_num_under(char c);
bool							is_identifier(const char *s);
size_t							map_len(t_map *envmap, bool count_null_value);
// map.c
int								map_set_from_string(t_map *envmap,
									const char *string, bool allow_empty_value);
// void						map_printall(t_map *map);

// -------------------- EXEC --------------------
// exec_no_builtin.c
int								exec_nonbuiltin(t_map *envmap, t_node *node);
// exec.c
void							interpret_cmd(t_map *envmap, char *line,
									int *stat_loc);
// token_to_arg.c
char							**token_list_to_argv(t_token *tok);

// -------------------- EXPAND --------------------
// expand.c
void							add_char(char **s, char c);
void							expand(t_map *envmap, t_node *node,
									int *status);
// quote_removal.c
void							expand_quote_removal(t_node *node);
// special_param.c
int								is_special_param(char *str);
void							expand_special_param_str(char **dst,
									char **rest, char *ptr, int *status);
// variable.c
int								is_variable(char *str);
void							expand_var_str(t_map *envmap, char **dst,
									char **rest, char *ptr);
void							add_quote(t_map *envmap, char **dst,
									char **rest, char *ptr, int *status);
void							expand_variable(t_map *envmap, t_node *node,
									int *status);

// -------------------- FINISH --------------------
// destructor.c
void							free_node(t_node *node);
void							free_token(t_token *tok);
void							free_argv(char **argv);
// error.c
void							perror_prefix(void);
void							not_found_cmd(const char *location,
									const char *msg, int status);
void							malloc_error(char *msg);
void							tokenize_error(const char *location, int *j,
									char *line);
void							parse_error(const char *location,
									t_token **rest, t_token *tok);
// error2.c
void							fatal_error(const char *msg);
void							assert_error(const char *msg);
void							xperror(const char *location);
void							builtin_error(const char *func,
									const char *name, const char *err);

// -------------------- PARSE --------------------
// parse_redir.c
t_node							*parse_redir_out(t_token **rest, t_token *tok);
t_node							*parse_redir_in(t_token **rest, t_token *tok);
t_node							*parse_redir_append(t_token **rest,
									t_token *tok);
t_node							*parse_redir_heredoc(t_token **rest,
									t_token *tok);
// parse_utils.c
bool							is_ctrl_operator(t_token *tok);
t_node							*new_node(t_node_kind kind);
void							add_node(t_node **node, t_node *elm);
// parse.c
t_node							*parse(t_token *tok);

// -------------------- PIPE --------------------
// pipe.c
void							create_new_pipe(t_node *node);
void							process_child_pipe(t_node *node);
void							process_parent_pipe(t_node *node);

// -------------------- REDIRECT --------------------
// here_document.c
int								read_here_document(t_map *envmap,
									const char *delimiter, bool is_quoted,
									int *status);
// open_file.c
int								open_redirect_file(t_map *envmap, t_node *node,
									int *status);
// redirect.c
void							setup_redirect(t_node *redirects);
void							reset_redirect(t_node *redirects);
// stash_fd.c
int								stash_fd(int fd);

// -------------------- SIGNAL --------------------
// signal.c
void							reset_signal(void);
void							setup_signal(int *status);

// -------------------- TOKENIZE --------------------
// token.c
void							add_token(t_token **tokens, t_token *tok);
bool							at_eof(t_token *tok);
t_token							*token_dup(t_token *tok);
bool							equal_operators(t_token *tok, char *op);
// tokenize.c
t_token							*tokenize(char *arg);
t_token							*new_token(char *word, t_token_kind kind);
// tokenize_helper.c
bool							is_metacharacter(char c);
bool							which_op(const char *s, const char *op);
bool							is_operator(const char *s);
void							init_operators(char *operators[7]);

// -------------------- UTILS --------------------
// ft_close.c
int								ft_close(int fildes);
// ft_dup2.c
int								ft_dup2(int fildes, int fildes2);
// ft_isspace.c
bool							ft_isspace(char c);
// ft_pipe.c
int								ft_pipe(int pipefd[2]);
// ft_strncpy_ex.c
size_t							ft_strncpy_ex(char *dst, char *src, size_t n,
									size_t dstsize);
// utils.c
char							*ft_strncpy(char *dest, char *src, size_t n);
int								ft_strcmp(const char *s1, const char *s2);
bool							is_blank(char c);

#endif
