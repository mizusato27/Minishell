/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   builtin.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/22 21:00:56 by ynihei            #+#    #+#             */
/*   Updated: 2025/02/25 09:44:37 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

//いったん置いておいて、この後にbuiltin_exitを実装する
// int builtin_exit(char **argv)
// {
// 	int		status;

// 	status = 0;
// 	printf("argv[0]: %s\n", argv[0]);
// 	exit(status);
// }

int	exec_builtin(t_node *node)
{
	int		status;
	char	**argv;

	status = 0;// <--- 追加しないとコンパイルエラーが出ました。
	do_redirect(node->command->redirects);
	argv = token_list_to_argv(node->command->args);
	if (ft_strcmp(argv[0], "exit") == 0)
		status = builtin_exit(argv);
	else if (ft_strcmp(argv[0], "export") == 0)
		status = builtin_export(argv);
	else if (ft_strcmp(argv[0], "pwd") == 0)
		status = builtin_pwd();
	else
		todo("exec_builtin");
	free_argv(argv);
	reset_redirect(node->command->redirects);
	return (status);
}

void	init_builtin_commands(char *commands[8])
{
	commands[0] = "exit";
	commands[1] = "export";
	commands[2] = "pwd";
	// commands[3] = "cd";
	// commands[4] = "echo";
	// commands[5] = "env";
	// commands[6] = "unset";
	commands[7] = NULL;
}

bool	is_builtin(t_node *node)
{
	const char		*cmd_name;
	char			*builtin_commands[8];
	unsigned int	i;

	init_builtin_commands(builtin_commands);
	if (node == NULL || node->command == NULL || node->command->args == NULL
		|| node->command->args->word == NULL)
		return (false);
	cmd_name = node->command->args->word;
	i = 0;
	while (i < 3)
	{
		if (ft_strcmp(cmd_name, builtin_commands[i]) == 0)
			return (true);
		i++;
	}
	return (false);
}
