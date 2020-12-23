/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools4_output.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 21:21:15 by bshang            #+#    #+#             */
/*   Updated: 2020/12/23 04:58:43 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		errorfd(int fd, char *cmd, char *msg, char *cmdstr)
{
	ft_putstr_fd(cmd, fd);
	ft_putstr_fd(msg, fd);
	ft_putstr_fd(cmdstr, fd);
	ft_putstr_fd("\n", fd);
}

void		put_error_message(char *s1, char *s2, char *s3, int fd)
{
	ft_putstr_fd(s1, fd);
	ft_putstr_fd(strerror(errno), fd);
	ft_putstr_fd(": ", fd);
	ft_putstr_fd(s2, fd);
	if (s3)
	{
		ft_putstr_fd(": ", fd);
		ft_putstr_fd(s3, fd);
	}
	ft_putstr_fd("\n", fd);
	g_proc_status = 2;
}

int			return_print_error(char *str)
{
	ft_putstr("mini: ");
	ft_putstr(strerror(errno));
	ft_putstr(": ");
	ft_putendl_fd(str, 1);
	g_proc_status = 1;
	return (1);
}

void		print_list(t_list *head)
{
	while (head)
	{
		ft_putstr(head->content);
		ft_putstr("\t");
		ft_putnbr_fd(head->size, 1);
		ft_putstr("\n");
		head = head->next;
	}
}

void		print_array(char **arr)
{
	int		i;

	i = 0;
	while (arr && arr[i] != NULL)
	{
		ft_putstr(arr[i]);
		ft_putstr("\n");
		i++;
	}
}
