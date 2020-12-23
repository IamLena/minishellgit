/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_env.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 14:24:07 by bshang            #+#    #+#             */
/*   Updated: 2020/12/23 01:58:49 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	env_exec(t_cmd *cmdlist, char **env)
{
	int		fd;
	int		i;

	g_proc_status = 0;
	fd = check_redirects_and_streams(cmdlist);
	if (fd > 0)
	{
		i = 0;
		while (env[i])
			ft_putendl_fd(env[i++], fd);
	}
	if (cmdlist->outstream && fd != -1)
		close(fd);
}
