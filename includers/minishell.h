/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:30:52 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/23 17:18:22 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <errno.h> // <--- pipe
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>

# ifndef PATH_MAX
#  define PATH_MAX 10000
# endif

// error_message
# define ER_MALLOC_STRDUP "malloc error"
# define ER_MALLOC_CALLOC "malloc error"
# define ER_SYNTAX_ERROR "syntax error"
# define ER_FORK "fork error"
# define ER_EXECVE "execve error"
# define ER_ACCESS "command not found"
# define ER_MALLOC "malloc error"

# define OPERATORS "|&;()\n"
# define METAS "|&;()<> \t\n"
# define SINGLE_QUOTE '\''
# define DOUBLE_QUOTE '\"'

// externは複数のファイルで使う変数を宣言するときに使う
extern bool					syntax_error;

# define CHILD_PROCESS 0
# define APPEND_CHAR_SIZE 1
# define END_CHAR_SIZE 1
# define ERROR_TOKENIZE 258
# define ERROR_PARSE 258
# define ERROR_OPEN_REDIR 1 // <-----

typedef struct s_token		t_token;
enum						e_token_kind
{
	TK_WORD,
	TK_RESERVED,
	TK_OP,
	TK_EOF,
};
typedef enum e_token_kind	t_token_kind;

// `word` is zero terminated string.
struct						s_token
{
	char					*word;
	t_token_kind			kind;
	t_token					*next;
};

enum						e_node_kind
{
	ND_PIPELINE, // <--- pipe
	ND_SIMPLE_CMD,
	ND_REDIR_OUT, // <--- added
	ND_REDIR_IN,
	ND_REDIR_APPEND,
	ND_REDIR_HEREDOC,
};
typedef enum e_node_kind	t_node_kind;
typedef struct s_node		t_node;
struct						s_node
{
	// t_token		*args;
	t_node_kind				kind;
	t_node					*next;
	// CMD <---------------
	t_token					*args;
	t_node					*redirects;
	// REDIR
	int						targetfd;
	t_token					*filename;
	t_token					*delimiter;
	int						filefd;
	int						stashed_targetfd;
	// PIPELINE <---
	int						inpipe[2];
	int						outpipe[2];
	t_node					*command;
};

typedef struct s_map		t_map;
typedef struct s_item		t_item;
struct						s_item
{ //<-env.c
	char					*name;
	char					*value;
	t_item					*next;
};

// map->item_headにはダミーのアイテムが入っている
//ダミーノードの利点は、リストの先頭と末尾の処理を簡単にすることができる
// map->item_head.nextには最初のアイテムが入っている
struct						s_map
{ //<-env.c
	t_item					item_head;
};

// externは複数のファイルで使う変数を宣言するときに使う
extern bool					syntax_error;
extern int						last_status;
extern t_map *g_envmap;           //<-env.c
extern bool readline_interrupted; //<-signal.c
extern volatile sig_atomic_t sig; //<-signal.c

// error.c
void						malloc_error(void);
void						err_exit(const char *location, const char *msg,
								int status);
void						error(char *msg);
void						tokenize_error(const char *location, int *j,
								char *line);
void						parse_error(const char *location, t_token **rest,
								t_token *tok);
void						fatal_error(const char *msg);
void						assert_error(const char *msg);
void						xperror(const char *location);

// exec.c
char						**token_list_to_argv(t_token *tok);
void						interpret(char *line, int *stat_loc);

// expand.c
void						todo(char *message);
void						expand(t_node *node);

// tokenize.c
t_token						*tokenize(char *arg);
t_token						*new_token(char *word, t_token_kind kind);

// tokenize_helper.c
bool						is_blank(char c);
bool						is_metacharacter(char c);
bool						which_op(const char *s, const char *op);
// bool	is_operator(char c);
bool						is_operator(const char *s);
void						init_operators(char *operators[14]);

// destructor.c
void						free_node(t_node *node);
void						free_token(t_token *tok);
void						free_argv(char **argv);

// parse.c
bool						at_eof(t_token *tok);
t_node						*parse(t_token *tok);

// utils.c
char						*ft_strncpy(char *dest, char *src, size_t n);
int							ft_strcmp(const char *s1, const char *s2);

// redirect.c
// int		redirect(t_node *node);
int							open_redirect_file(t_node *node);
void						do_redirect(t_node *redirects);
void						reset_redirect(t_node *redirects);

// pipe.c
void						create_new_pipe(t_node *node);
void						process_child_pipe(t_node *node);
void						process_parent_pipe(t_node *node);

// map.c
char						*map_get(t_map *map, const char *name);
int							map_put(t_map *map, const char *string,
								bool allow_empty_value);
int							map_unset(t_map *map, const char *name);
size_t						map_len(t_map *map, bool count_null_value);
void						map_printall(t_map *map);

// map_create.c
t_item						*item_new(char *name, char *value);
t_map						*map_new(void);
int							map_set(t_map *map, const char *name,
								const char *value);

// map_helper.c
bool						is_identifier(const char *s);
// char	*item_get_string(t_item *item);

// env.c
char						*xgetenv(const char *name);
void						initenv(void);
char						**get_environ(t_map *map);

// builtin.c
int							exec_builtin(t_node *node);
bool						is_builtin(t_node *node);

//builtin_exit.c
int							builtin_exit(char **argv);

#endif
