#include "minishell.h"

// static void	remove_quote(t_token *token)
// {
// 	char	*new_word;
// 	char	*word;
// 	char	quote_flag;

// 	if (token == NULL || token->kind != TK_WORD || token->word == NULL)
// 		return ;
// 	word = token->word;
// 	// new_word = NULL;
// 	new_word = ft_calloc(1, sizeof(char));// <--- modified
// 	if (new_word == NULL)
// 		fatal_error("calloc");
// 	while (*word && !is_metacharacter(*word))
// 	{
// 		if (*word == SINGLE_QUOTE || *word == DOUBLE_QUOTE)
// 		{
// 			quote_flag = *word;
// 			word++;
// 			while (*word != quote_flag)
// 			{
// 				if (*word == '\0')
// 					todo("Unclosed quote");
// 				add_char(&new_word, *word++);
// 			}
// 			word++;
// 		}
// 		else
// 			add_char(&new_word, *word++);
// 	}
// 	free(token->word);
// 	if (!new_word)
// 		new_word = ft_strdup("");
// 	token->word = new_word;
// 	remove_quote(token->next);
// }

static void	process_removal(char **dst, char **rest, char *ptr)
{
	char	quote_flag;

	quote_flag = *ptr;
	if (quote_flag != SINGLE_QUOTE && quote_flag != DOUBLE_QUOTE)
		assert_error("Expected quote");
	ptr++;
	while (*ptr != quote_flag)
	{
		if (*ptr == '\0')
			assert_error("Unclosed double quote");
		add_char(dst, *ptr++);
	}
	ptr++;
	*rest = ptr;
}

static void	remove_quote(t_token *token)
{
	char	*new_word;
	char	*word;

	if (token == NULL || token->kind != TK_WORD || token->word == NULL)
		return ;
	word = token->word;
	new_word = ft_calloc(1, sizeof(char));
	if (new_word == NULL)
		fatal_error("calloc");
	while (*word && !is_metacharacter(*word))
	{
		if (*word == SINGLE_QUOTE || *word == DOUBLE_QUOTE)
			process_removal(&new_word, &word, word);
		else
			add_char(&new_word, *word++);
	}
	free(token->word);
	if (!new_word)
		new_word = ft_strdup("");
	token->word = new_word;
	remove_quote(token->next);
}

void	expand_quote_removal(t_node *node)
{
	if (node == NULL)
		return ;
	remove_quote(node->args);
	remove_quote(node->filename);
	remove_quote(node->delimiter);
	expand_quote_removal(node->redirects);
	expand_quote_removal(node->command);// <--- pipe
	expand_quote_removal(node->next);
}
