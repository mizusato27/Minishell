/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/26 15:30:52 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/09 07:54:31 by ynihei           ###   ########.fr       */
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

extern bool	syntax_error;
# define CHILD_PROCESS 0
# define APPEND_CHAR_SIZE 1
# define END_CHAR_SIZE 1
# define ERROR_TOKENIZE 258

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
};
typedef enum e_node_kind	t_node_kind;
typedef struct s_node	t_node;
struct s_node {
	t_token		*args;
	t_node_kind	kind;
	t_node		*next;
};

//error.c
void    malloc_error();
void	err_exit(const char *location, const char *msg, int status);
void	error(char *msg);
void	tokenize_error(const char *location, int *j, char *line);

//exec.c
// int		interpret(char *line);
void	interpret(char *line, int *stat_loc);
char	*ft_strncpy(char *dest, char *src, size_t n);

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
t_node	*parse(t_token *tok);

#endif