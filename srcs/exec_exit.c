/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_exit.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 00:34:10 by nalecto           #+#    #+#             */
/*   Updated: 2020/12/23 05:06:06 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		exit_exec(t_cmd *headcmd, t_cmd *cmdlist, char **env, char **temp)
{
	int		code;
	char	*st;

	code = 0;
	if (cmdlist && cmdlist->args)
	{
		st = cmdlist->args->content;
		while (*(char*)cmdlist->args->content)
		{
			if (isdigit(*(char*)cmdlist->args->content) == 0)
			{
				errorfd(2, "mini: exit: ", "numeric argument required: ", st);
				g_proc_status = 2;
				cmdlist->args->content = st;
				exit(255);
			}
			cmdlist->args->content++;
		}
		cmdlist->args->content = st;
		code = ft_atoi(cmdlist->args->content);
	}
	clear_cmd(&headcmd);
	array_clear(env);
	array_clear(temp);
	code == 0 ? exit(g_proc_status) : exit(code % 256);
}
