/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools6_redirects.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 21:44:22 by bshang            #+#    #+#             */
/*   Updated: 2020/12/23 05:00:55 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			check_redirects_and_streams(t_cmd *cmdlist)
{
	t_list	*lastfile;
	int		fd;

	fd = 1;
	if (cmdlist->outstream)
	{
		lastfile = ft_lstlast(cmdlist->outstream);
		fd = open(lastfile->content, lastfile->size, S_IRUSR | S_IWUSR
		| S_IRGRP | S_IROTH);
		if (fd == -1)
			ft_printf("mini: %s: %s\n", strerror(errno), lastfile->content);
	}
	return (fd);
}

void		execve_redirect_in(t_cmd *cmdlist)
{
	t_list	*instream;
	int		lastinfd;

	if (cmdlist)
	{
		if (cmdlist->readpipe == -1 && cmdlist->instream != NULL)
		{
			instream = cmdlist->instream;
			while (instream->next)
				instream = instream->next;
			lastinfd = open(instream->content, instream->size, S_IRUSR |
			S_IWUSR | S_IRGRP | S_IROTH);
			dup2(lastinfd, 0);
			close(lastinfd);
		}
	}
}

void		execve_redirect_out(t_cmd *cmdlist)
{
	t_list	*outstream;
	int		lastoutfd;

	if (cmdlist)
	{
		if (cmdlist->writepipe == -1 && cmdlist->outstream != NULL)
		{
			outstream = cmdlist->outstream;
			while (outstream->next)
				outstream = outstream->next;
			lastoutfd = open(outstream->content, outstream->size, S_IRUSR |
			S_IWUSR | S_IRGRP | S_IROTH);
			dup2(lastoutfd, 1);
			close(lastoutfd);
		}
	}
}

int			checkredirect(char *curtoken, char *nexttoken)
{
	if (strequal(curtoken, ">") || \
		strequal(curtoken, ">>") || \
		strequal(curtoken, "<"))
	{
		if (nexttoken == NULL)
			ft_putstr("mini: parse error near `\\n\'\n");
		else if (strequal(nexttoken, ";"))
			ft_putstr("mini: parse error near `;\'\n");
		else if (strequal(nexttoken, "|"))
			ft_putstr("mini: parse error near `|\'\n");
		else if (strequal(nexttoken, ">"))
			ft_putstr("mini: parse error near `>\'\n");
		else if (strequal(nexttoken, ">>"))
			ft_putstr("mini: parse error near `>>\'\n");
		else if (strequal(nexttoken, "<"))
			ft_putstr("mini: parse error near `<\'\n");
		else
			return (0);
		return (1);
	}
	return (0);
}

int			check_redirects_and_pipes(t_cmd *cmdlist)
{
	t_list	*lastfile;
	int		fd;

	if (cmdlist->writepipe != -1 || cmdlist->readpipe != -1)
		return (-1);
	if (cmdlist->outstream)
	{
		lastfile = ft_lstlast(cmdlist->outstream);
		fd = open(lastfile->content, lastfile->size);
		if (fd == -1)
			errorfd(2, "mini: ", strerror(errno), lastfile->content);
	}
	return (fd);
}
