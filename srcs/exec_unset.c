/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_unset.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 00:46:46 by nalecto           #+#    #+#             */
/*   Updated: 2020/12/23 04:59:54 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	unset_one_argument(t_list *arg_list, char ***env, char ***temp)
{
	char	*arg;
	int		env_index;

	if (!(arg = parseargument(arg_list->content, *env, *temp)))
		return ;
	if (!(ft_isalpha((arg[0]))))
	{
		errorfd(2, "mini: unset:", "bad variable name: ", arg);
		free(arg);
		g_proc_status = 2;
		return ;
	}
	if ((env_index = which_env(*env, arg)) >= 0)
		*env = array_delete_item(env, env_index);
	if ((env_index = which_env(*temp, arg)) >= 0)
		*temp = array_delete_item(temp, env_index);
	free(arg);
}

void		unset_exec(t_cmd *cmdlist, char ***env, char ***temp)
{
	t_list	*start;
	int		fd;

	g_proc_status = 0;
	if (cmdlist->args == NULL || cmdlist->readpipe != -1)
	{
		close(cmdlist->readpipe);
		return ;
	}
	fd = check_redirects_and_streams(cmdlist);
	if (fd > 0)
	{
		start = cmdlist->args;
		while (cmdlist->args)
		{
			unset_one_argument(cmdlist->args, env, temp);
			cmdlist->args = cmdlist->args->next;
		}
		cmdlist->args = start;
	}
	if (cmdlist->outstream && fd != -1)
		close(fd);
}
