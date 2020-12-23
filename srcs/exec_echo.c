/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   exec_echo.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/17 00:45:16 by nalecto           #+#    #+#             */
/*   Updated: 2020/12/23 02:32:35 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void			parse_all_args(t_list *args, char **env, char **temp)
{
	char			*tmpstr;

	while (args)
	{
		tmpstr = parseargument(args->content, env, temp);
		free(args->content);
		args->content = tmpstr;
		args = args->next;
	}
}

static void			putargstofd(t_list *args, int fd, char **env, char **temp)
{
	unsigned char	nflag;

	nflag = 1;
	if (args && strequal(args->content, "-n"))
	{
		args = args->next;
		nflag = 0;
	}
	parse_all_args(args, env, temp);
	while (args)
	{
		if (args->content && *((char *)args->content) != '\0')
		{
			ft_putstr_fd(args->content, fd);
			if (args->next && args->next->content && \
			*((char *)args->next->content) != '\0')
				write(fd, " ", 1);
		}
		args = args->next;
	}
	if (nflag)
		write(fd, "\n", 1);
}

void				echo_exec(t_cmd *cmdlist, char **env, char **temp)
{
	int				lastfd;
	t_list			*lastfile;

	if ((!cmdlist->outstream && cmdlist->writepipe != -1) \
	|| (!cmdlist->outstream))
		putargstofd(cmdlist->args, 1, env, temp);
	else
	{
		lastfile = ft_lstlast(cmdlist->outstream);
		lastfd = open(lastfile->content, lastfile->size);
		if (lastfd != -1)
		{
			putargstofd(cmdlist->args, lastfd, env, temp);
			close(lastfd);
		}
	}
	g_proc_status = 0;
}
