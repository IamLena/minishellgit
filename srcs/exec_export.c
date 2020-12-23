/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_export.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 14:10:26 by bshang            #+#    #+#             */
/*   Updated: 2020/12/23 05:14:56 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		rewrite_value(char ***env, int idx, char *str)
{
	char	*res;
	char	*part_one;
	int		len;

	if (!(ft_strchr(str, '=')))
		return ;
	len = 0;
	while ((*env)[idx][len] != '=' && (*env)[idx][len])
		len++;
	part_one = ft_substr((*env)[idx], 0, len);
	while (*str != '=')
		str++;
	res = ft_strjoin(part_one, str);
	free(part_one);
	free((*env)[idx]);
	(*env)[idx] = res;
}

static void	format_output(char *str, int fd)
{
	ft_putstr_fd("declare -x ", fd);
	while (*str != '=' && *str)
		write(fd, str++, 1);
	if (*str == '=')
	{
		ft_putstr_fd("=\"", fd);
		str++;
		ft_putstr_fd(str, fd);
		write(fd, "\"", 1);
	}
	write(fd, "\n", 1);
}

static void	check_and_add_values(t_cmd *cmdlist, char ***env, char ***temp)
{
	t_list	*start;
	char	*arg;
	int		env_index;

	start = cmdlist->args;
	while (cmdlist->args)
	{
		if ((arg = parseargument(cmdlist->args->content, *env, *temp)) == NULL)
			return ;
		if (!(ft_isalpha(arg[0])))
		{
			g_proc_status = 1;
			return (errorfd(2, "mini: export: ", "not a valid identifier: ",
			cmdlist->args->content));
		}
		else if ((env_index = which_env(*env, arg)) >= 0)
			rewrite_value(env, env_index, arg);
		else if ((env_index = which_env(*temp, arg)) >= 0)
			*env = array_push_back(*env, (*temp)[env_index]);
		else
			*env = array_push_back(*env, arg);
		free(arg);
		cmdlist->args = cmdlist->args->next;
	}
	cmdlist->args = start;
}

static void	output_export(char **env, int fd)
{
	int		i;

	i = 0;
	array_sort(env);
	while (env[i])
		format_output(env[i++], fd);
}

void		export_exec(t_cmd *cmdlist, char ***env, char ***temp)
{
	char	**env_sorted;
	int		fd;

	g_proc_status = 0;
	if (cmdlist->readpipe != -1 && cmdlist->args)
	{
		close(cmdlist->readpipe);
		return ;
	}
	fd = check_redirects_and_streams(cmdlist);
	if (fd > 0)
	{
		if (cmdlist->args == NULL || ((char*)cmdlist->args->content)[0] == '#')
			output_export(*env, fd);
		else
			check_and_add_values(cmdlist, env, temp);
	}
	if (cmdlist->outstream && fd != -1)
		close(fd);
}
