#include "minishell.h"

//クォートを削除
static void	remove_quote(t_token *token)
{
	char	*new_word;
	char	*word;
	char	quote_flag;

	if (token == NULL || token->kind != TK_WORD || token->word == NULL)
		return ;
	word = token->word;
	// new_word = NULL;
	new_word = ft_calloc(1, sizeof(char));// <--- modified
	if (new_word == NULL)
		fatal_error("calloc");
	while (*word && !is_metacharacter(*word))
	{
		if (*word == SINGLE_QUOTE || *word == DOUBLE_QUOTE)
		{
			quote_flag = *word;
			word++;
			while (*word != quote_flag)
			{
				if (*word == '\0')
					todo("Unclosed quote");
				add_char(&new_word, *word++);
			}
			word++;
		}
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
