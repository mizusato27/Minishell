/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ynihei <ynihei@student.42.fr>              +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/02/27 15:36:50 by mizusato          #+#    #+#             */
/*   Updated: 2025/03/15 15:13:28 by ynihei           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	consume_dot_path(char *new_pwd, char **rest, int elm_len)
{
	char	*current_ptr;
	char	*start_ptr;
	char	*last_slash_ptr;

	current_ptr = new_pwd;
	start_ptr = new_pwd;
	last_slash_ptr = NULL;
	if ((*rest)[elm_len] == '\0' || (*rest)[elm_len] == '/')
	{
		*rest += elm_len;
		if (elm_len == 2)
		{
			while (*current_ptr)
			{
				if (*current_ptr == '/')
					last_slash_ptr = current_ptr;
				current_ptr++;
			}
			if (last_slash_ptr != start_ptr)
				*last_slash_ptr = '\0';
		}
	}
}

static void	add_path_elm(char *new_pwd, char **rest, char *src)
{
	char	buf[PATH_MAX];
	size_t	dst_len;
	size_t	elm_len;

	elm_len = 0;
	dst_len = ft_strlen(new_pwd);
	while (src[elm_len] && src[elm_len] != '/')
		elm_len++;
	if (ft_strncpy_ex(buf, src, elm_len, PATH_MAX) >= PATH_MAX)
		assert_error(ER_OVERFLOW);
	if (new_pwd[dst_len - 1] != '/')
		if (ft_strlcat(new_pwd, "/", PATH_MAX) >= PATH_MAX)
			assert_error(ER_OVERFLOW);
	if (ft_strlcat(new_pwd, buf, PATH_MAX) >= PATH_MAX)
		assert_error(ER_OVERFLOW);
	*rest = src + elm_len;
}

static char	*get_new_pwd(char *old_pwd, char *path)
{
	char	new_pwd[PATH_MAX];
	char	*result;

	if (*path == '/' || old_pwd == NULL)
		ft_strlcpy(new_pwd, "/", PATH_MAX);
	else
		ft_strlcpy(new_pwd, old_pwd, PATH_MAX);
	while (*path)
	{
		if (*path == '/')
			path++;
		else if (ft_strncmp(path, "..", 2) == 0)
			consume_dot_path(new_pwd, &path, 2);
		else if (*path == '.')
			consume_dot_path(new_pwd, &path, 1);
		else
			add_path_elm(new_pwd, &path, path);
	}
	result = ft_strdup(new_pwd);
	if (result == NULL)
		malloc_error(ER_STRDUP);
	return (result);
}

int	builtin_cd(t_map *envmap, char **argv)
{
	char	*current_pwd;
	char	*old_pwd;
	char	*new_pwd;
	char	path[PATH_MAX];

	current_pwd = map_get_value(envmap, "PWD");
	old_pwd = map_get_value(envmap, "OLDPWD");
	if (argv[1] && ft_strcmp(argv[1], "-") == 0)
		return (process_minus_option(envmap, old_pwd, current_pwd));
	if (cpy_home_path(envmap, path, argv[1]) < 0)
		return (1);
	if (chdir_ex(path) < 0)
		return (1);
	if (map_set_value_ex(envmap, "OLDPWD", current_pwd) < 0)
		return (1);
	new_pwd = get_new_pwd(current_pwd, path);
	if (map_set_value_ex(envmap, "PWD", new_pwd) < 0)
		return (1);
	free(new_pwd);
	return (0);
}
