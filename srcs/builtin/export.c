/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   export.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/24 21:11:02 by ynihei            #+#    #+#             */
/*   Updated: 2025/03/06 20:47:40 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	print_allenv(void)
{
	t_item	*cur;
	t_item	*sorted_head = NULL;
	t_item	*node;
	t_item	*prev;
	t_item	*insert_pos;

	// 環境変数が空の場合は何もしない
	if (!g_ctx.g_envmap || !g_ctx.g_envmap->item_head.next)
		return;

	// 環境変数リストをコピーし、辞書順に並び替える
	cur = g_ctx.g_envmap->item_head.next;
	while (cur)
	{
		// 新しいノードを作成
		node = malloc(sizeof(t_item));
		if (!node)
			malloc_error("malloc");
		node->name = cur->name;
		node->value = cur->value;
		node->next = NULL;

		// 挿入位置を探す
		insert_pos = sorted_head;
		prev = NULL;
		while (insert_pos && strcmp(insert_pos->name, node->name) < 0)
		{
			prev = insert_pos;
			insert_pos = insert_pos->next;
		}

		// ソート済みリストにノードを挿入
		if (prev == NULL)
		{
			node->next = sorted_head;
			sorted_head = node;
		}
		else
		{
			node->next = prev->next;
			prev->next = node;
		}

		cur = cur->next;
	}
	// ソート済みリストを表示
	cur = sorted_head;
	while (cur)
	{
		if (cur->value)
			printf("declare -x %s=\"%s\"\n", cur->name, cur->value);
		else
			printf("declare -x %s\n", cur->name);
		cur = cur->next;
	}
	// ソート済みリストを解放
	while (sorted_head)
	{
		node = sorted_head;
		sorted_head = sorted_head->next;
		free(node);
	}
}

int	builtin_export(char **argv)
{
	size_t	i;
	int		status;

	if (argv[1] == NULL)
	{
		print_allenv();
		return (0);
	}
	status = 0;
	i = 1;
	while (argv[i])
	{
		if (map_put(g_ctx.g_envmap, argv[i], true) < 0)
		{
			builtin_error("export", argv[i], "not a valid identifier");
			status = 1;
		}
		i++;
	}
	return (status);
}
