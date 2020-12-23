/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_token_to_cmd.c                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalecto <nalecto@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 19:13:15 by nalecto           #+#    #+#             */
/*   Updated: 2020/12/23 18:30:27 by nalecto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	addtostream(char *filename, int flags, t_cmd *new)
{
	int		fd;
	mode_t	mode;

	mode = S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH;
	if (flags == O_RDONLY)
		fd = open(filename, flags);
	else
		fd = open(filename, flags, mode);
	if (fd == -1)
		return (return_print_error(filename));
	close(fd);
	if (flags == O_RDONLY)
		ft_lstadd_back(&(new->instream), \
			ft_lstnew(ft_strdup(filename), flags));
	else
		ft_lstadd_back(&(new->outstream), \
			ft_lstnew(ft_strdup(filename), flags));
	return (0);
}

static int	parse_token(t_cmd **new, char **tokens, int *i)
{
	int		flags;

	*new = *new == NULL ? new_cmd() : *new;
	if (checkredirect(tokens[*i], tokens[*i + 1]))
		return (0);
	if (strequal(tokens[*i], ">"))
		flags = O_CREAT | O_WRONLY | O_TRUNC;
	else if (strequal(tokens[*i], ">>"))
		flags = O_CREAT | O_APPEND | O_WRONLY;
	else if (strequal(tokens[*i], "<"))
		flags = O_RDONLY;
	else
	{
		if (*i == 0 || strequal(tokens[*i - 1], ";") || \
		strequal(tokens[*i - 1], "|"))
			(*new)->cmdstr = parsecmdstr(tokens[*i]);
		else
			ft_lstadd_back(&(*new)->args, \
			ft_lstnew(ft_strdup(tokens[*i]), 0));
		return (0);
	}
	if (addtostream(tokens[*i + 1], flags, *new))
		return (1);
	(*i)++;
	return (0);
}

static int	check_unexpected(t_cmd **cmdlist, t_cmd **new, char *cur_token)
{
	if (*cmdlist == NULL && *new == NULL)
	{
		g_proc_status = 258;
		ft_printf("mini: syntax error near unexpected token `%s'\n", cur_token);
		return (1);
	}
	if (*new && (*new)->cmdstr == NULL && (*new)->args == NULL && \
	(*new)->instream == NULL && (*new)->outstream == NULL)
	{
		if ((*new)->readpipe != -1)
			close((*new)->readpipe);
		if ((*new)->writepipe != -1)
			close((*new)->writepipe);
		g_proc_status = 258;
		ft_printf("mini: syntax error near unexpected token `%s'\n", cur_token);
		return (1);
	}
	return (0);
}

static int	add_cmd(t_cmd **cmdlist, t_cmd **new, char *cur_token)
{
	int		pipefds[2];

	if (check_unexpected(cmdlist, new, cur_token))
		return (1);
	if (strequal(cur_token, "|"))
	{
		if (pipe(pipefds) == 0)
			(*new)->writepipe = pipefds[1];
	}
	cmd_push_back(cmdlist, *new);
	*new = NULL;
	if (strequal(cur_token, "|"))
	{
		*new = new_cmd();
		(*new)->readpipe = pipefds[0];
	}
	return (0);
}

void		parse_to_cmd_list(t_tokens *tokens, t_cmd **cmdlist)
{
	int		i;
	t_cmd	*new;

	i = 0;
	new = NULL;
	if ((tokens->tokens = ft_split(tokens->line, '\n')))
	{
		while (tokens->tokens[i] != NULL)
		{
			if (strequal(tokens->tokens[i], ";") || \
			strequal(tokens->tokens[i], "|"))
			{
				if (add_cmd(cmdlist, &new, tokens->tokens[i]) == 1)
					return (free_tokens_clear_cmd(tokens, cmdlist));
			}
			else if (parse_token(&new, tokens->tokens, &i) == 1)
				return (free_tokens_clear_cmd(tokens, cmdlist));
			i++;
		}
		if (i > 0)
			add_cmd(cmdlist, &new, tokens->tokens[i]);
	}
	free_tokens(tokens);
}
