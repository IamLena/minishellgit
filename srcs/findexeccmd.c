/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   findexeccmd.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 22:56:29 by nalecto           #+#    #+#             */
/*   Updated: 2020/12/23 06:13:09 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		ft_append_str(char **old, char *add)
{
	char	*tmp;
	int		i;

	if (*old == NULL)
		*old = ft_strdup(add);
	else if ((tmp = malloc(ft_strlen(*old) + ft_strlen(add) + 1)))
	{
		i = 0;
		while ((*old)[i])
		{
			tmp[i] = (*old)[i];
			i++;
		}
		while (*add)
		{
			tmp[i++] = *add;
			add++;
		}
		tmp[i] = '\0';
		free(*old);
		*old = tmp;
	}
}

static char	*tryfullpath(t_cmd *cmdlist, char **env, int *pathindex)
{
	struct stat	buffer;

	*pathindex = -1;
	if ((stat(cmdlist->cmdstr, &buffer)) == 0)
	{
		if (S_ISREG(buffer.st_mode))
			return (ft_strdup(cmdlist->cmdstr));
		if (S_ISDIR(buffer.st_mode))
			errorfd(2, "mini: ", "is a directory: ", cmdlist->cmdstr);
		g_proc_status = 126;
	}
	if (errno == 20)
	{
		errorfd(2, "mini: ", strerror(errno), cmdlist->cmdstr);
		g_proc_status = 126;
	}
	if ((*pathindex = which_env(env, "PATH")) < 0)
	{
		g_proc_status = 127;
		return (NULL);
	}
	return (NULL);
}

static char	*try_relative_path(t_cmd *cmdlist, int i, char *paths)
{
	char		*wholepath;
	char		*tmp;
	struct stat	buffer;

	wholepath = NULL;
	tmp = ft_substr(paths, 0, i);
	ft_append_str(&wholepath, tmp);
	ft_append_str(&wholepath, "/");
	ft_append_str(&wholepath, cmdlist->cmdstr);
	free(tmp);
	if ((stat(wholepath, &buffer)) == 0)
		return (wholepath);
	free(wholepath);
	return (NULL);
}

static char	*find_cmd_in_paths(char *paths, t_cmd *cmdlist, char **wholepath)
{
	int		i;

	i = 0;
	while (paths[i])
	{
		if (paths[i] == ':')
		{
			if ((*wholepath = try_relative_path(cmdlist, i, paths)))
				return (*wholepath);
			paths += i + 1;
			i = 0;
		}
		else
			i++;
	}
	*wholepath = try_relative_path(cmdlist, i, paths);
	return (*wholepath);
}

char		*find_cmd(t_cmd *cmdlist, char **env, char **temp)
{
	char		*wholepath;
	char		*paths;
	int			env_index;

	g_proc_status = 0;
	wholepath = tryfullpath(cmdlist, env, &env_index);
	if (wholepath || env_index == -1)
		return (wholepath);
	paths = ft_strchr(env[env_index], '=') + 1;
	wholepath = find_cmd_in_paths(paths, cmdlist, &wholepath);
	if (wholepath)
	{
		free(cmdlist->cmdstr);
		cmdlist->cmdstr = wholepath;
	}
	return (wholepath);
}
