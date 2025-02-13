/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mizusato <mizusato@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:30:52 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/13 15:43:14 by mizusato         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../libft/libft.h"
# include <fcntl.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <stdio.h>
# include <stdlib.h>
# include <sys/wait.h>
# include <unistd.h>
# include <stdbool.h>
# include <signal.h>

#ifndef PATH_MAX
# define PATH_MAX 10000
#endif

//error_message
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

//externは複数のファイルで使う変数を宣言するときに使う
extern bool	syntax_error;
# define CHILD_PROCESS 0
# define APPEND_CHAR_SIZE 1
# define END_CHAR_SIZE 1
# define ERROR_TOKENIZE 258
# define ERROR_PARSE 258
# define ERROR_OPEN_REDIR 1// <-----

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

enum e_node_kind {
	ND_SIMPLE_CMD,
	ND_REDIR_OUT,// <-----
	ND_REDIR_IN,
	ND_REDIR_APPEND,
	ND_REDIR_HEREDOC,
};
typedef enum e_node_kind	t_node_kind;
typedef struct s_node	t_node;
struct s_node {
	// t_token		*args;
	t_node_kind	kind;
	t_node		*next;
	// CMD <---------------
	t_token		*args;
	t_node		*redirects;
	// REDIR
	int			targetfd;
	t_token		*filename;
	t_token		*delimiter;
	int			filefd;
	int			stashed_targetfd;
};

//error.c
void    malloc_error();
void	err_exit(const char *location, const char *msg, int status);
void	error(char *msg);
void	tokenize_error(const char *location, int *j, char *line);
void	parse_error(const char *location, t_token **rest, t_token *tok);

//exec.c
// int		interpret(char *line);
void	interpret(char *line, int *stat_loc);

//expand.c
void	expand(t_node *node);

//tokenize.c
t_token	*tokenize(char *arg);
t_token	*new_token(char *word, t_token_kind kind);

//tokenize_helper.c
bool	is_blank(char c);
bool	is_metacharacter(char c);
bool	which_op(const char *s, const char *op);
bool	is_operator(char c);
void	init_operators(char *operators[14]);

//destructor.c
void	free_node(t_node *node);
void	free_token(t_token *tok);
void	free_argv(char **argv);

//parse.c
bool	at_eof(t_token *tok);
t_node	*parse(t_token *tok);

//utils.c
char	*ft_strncpy(char *dest, char *src, size_t n);

// 修正
int		redirect(t_node *node);
char	**token_list_to_argv(t_token *tok);
char	*find_executable(const char *filename);
void	todo(char *message);

#endif
