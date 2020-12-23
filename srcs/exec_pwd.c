/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_pwd.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 14:24:45 by bshang            #+#    #+#             */
/*   Updated: 2020/12/23 01:58:44 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		pwd_exec(t_cmd *cmdlist)
{
	char	*str;
	int		fd;

	g_proc_status = 0;
	fd = check_redirects_and_streams(cmdlist);
	str = NULL;
	if (!(str = getcwd(NULL, 0)))
		return ;
	if (fd > 0)
		ft_putendl_fd(str, fd);
	if (cmdlist->outstream && fd != -1)
		close(fd);
	free(str);
}
