/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_cd.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 14:22:50 by bshang            #+#    #+#             */
/*   Updated: 2020/12/23 06:21:19 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static char	*expand_path(char *arg, char **env, char **temp)
{
	char	*str;
	char	*res;
	char	*env_path;
	char	*tmp;

	str = arg;
	res = NULL;
	if (*str == '~')
	{
		str++;
		while (*str == '/')
			str++;
		if (!(env_path = find_env_path("HOME", env)))
		{
			if (!(env_path = find_env_path("HOME", temp)))
				return (NULL);
		}
		tmp = ft_join_char(env_path, '/');
		res = ft_strjoin(tmp, str);
		free(tmp);
	}
	return (res);
}

static int	prepare_path(char **path, t_cmd *cmdlist, char ***env, char ***temp)
{
	if (cmdlist->args == NULL || strequal(cmdlist->args->content, "~"))
	{
		if (!(*path = find_env_path("HOME", *env)))
		{
			if (!(*path = find_env_path("HOME", *temp)))
				return (-1);
		}
	}
	else if (*(char*)cmdlist->args->content == '~')
	{
		if (!(*path = expand_path(cmdlist->args->content, *env, *temp)))
			return (-1);
		return (0);
	}
	else if (!(*path = parseargument(cmdlist->args->content, *env, *temp)))
		return (-1);
	return (0);
}

static void	update_one_value(char *name, char *value, char ***env)
{
	int		env_index;
	char	*new_env_elem;

	new_env_elem = ft_strjoin(name, value);
	if ((env_index = which_env(*env, name)) >= 0)
		rewrite_value(env, env_index, new_env_elem);
	else
		*env = array_push_back(*env, new_env_elem);
	free(name);
	free(value);
	free(new_env_elem);
}

static int	update_env_values(char ***env, char *oldpwd_value)
{
	char	*value_part;
	char	*name_part;

	if (oldpwd_value)
	{
		value_part = oldpwd_value;
		name_part = ft_strdup("OLDPWD=");
		update_one_value(name_part, value_part, env);
	}
	name_part = ft_strdup("PWD=");
	if (!(value_part = getcwd(NULL, 0)))
		return (0);
	update_one_value(name_part, value_part, env);
	return (0);
}

void		cd_exec(t_cmd *cmdlist, char ***env, char ***temp)
{
	char	*path;
	char	*envarg;
	char	*oldpwd;
	int		i;
	int		fd;

	g_proc_status = 0;
	if ((cmdlist->outstream) && check_redirects_and_pipes(cmdlist) < 0)
		return ;
	if (prepare_path(&path, cmdlist, env, temp) < 0)
	{
		free(path);
		return ;
	}
	if ((oldpwd = getcwd(NULL, 0)) && chdir(path) == -1)
	{
		free(oldpwd);
		errorfd(2, "cd: ", strerror(errno), path);
		g_proc_status = 1;
	}
	else
		g_proc_status = update_env_values(env, oldpwd);
	free(path);
}
