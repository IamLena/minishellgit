/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cmdlist.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalecto <nalecto@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 17:04:51 by nalecto           #+#    #+#             */
/*   Updated: 2020/12/22 21:44:39 by nalecto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	init_cmd(t_cmd *new)
{
	new->cmdstr = NULL;
	new->args = NULL;
	new->instream = NULL;
	new->outstream = NULL;
	new->next = NULL;
	new->readpipe = -1;
	new->writepipe = -1;
}

t_cmd	*new_cmd(void)
{
	t_cmd	*new;

	new = malloc(sizeof(t_cmd));
	if (new)
		init_cmd(new);
	return (new);
}

void	cmd_push_back(t_cmd **lst, t_cmd *new)
{
	t_cmd	*last;

	if (!lst || !new)
		return ;
	if (*lst == NULL)
		*lst = new;
	else
	{
		last = *lst;
		while (last->next != NULL)
			last = last->next;
		last->next = new;
	}
}

void	print_cmd(t_cmd *list)
{
	t_list	*args;

	while (list)
	{
		ft_putstr("strcmd:");
		ft_putstr(list->cmdstr);
		ft_putstr("\nargs:\n");
		args = list->args;
		while (args != NULL)
		{
			ft_putstr(args->content);
			ft_putstr("\n");
			args = args->next;
		}
		ft_putstr("instream:\n");
		print_list(list->instream);
		ft_putstr("outstream:\n");
		print_list(list->outstream);
		ft_printf("readpipefd: %d\nwritepipefd: %d\n", \
		list->readpipe, list->writepipe);
		list = list->next;
	}
}

void	clear_cmd(t_cmd **cmdlist)
{
	t_cmd *tmp;

	while ((*cmdlist))
	{
		free(((*cmdlist)->cmdstr));
		ft_lstclear(&((*cmdlist)->args), free);
		ft_lstclear(&((*cmdlist)->instream), free);
		ft_lstclear(&((*cmdlist)->outstream), free);
		tmp = (*cmdlist)->next;
		free(*cmdlist);
		(*cmdlist) = tmp;
	}
	*cmdlist = NULL;
}
