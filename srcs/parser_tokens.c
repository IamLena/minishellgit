/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser_tokens.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalecto <nalecto@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 17:19:42 by nalecto           #+#    #+#             */
/*   Updated: 2020/12/23 18:20:32 by nalecto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		init_tokens(t_tokens *tokens)
{
	tokens->line = NULL;
	tokens->cur_token = 0;
	tokens->tokens = NULL;
	tokens->separator_count = 0;
	tokens->quotes = 0;
	tokens->dquotes = 0;
	tokens->shield = 0;
	tokens->pipe_count = 0;
	tokens->redirect = 0;
}

static int	parse1(char buf, t_tokens *tokens)
{
	if (buf != '>' && tokens->redirect == 1)
		tokens->redirect = 0;
	if (buf == '\\' && tokens->shield == 0)
		tokens->shield = 1;
	else if (tokens->shield == 1)
	{
		tokens->line = ft_join_char(tokens->line, '\\');
		tokens->line = ft_join_char(tokens->line, buf);
		tokens->shield = 0;
	}
	else if (buf == ';' && !tokens->dquotes && !tokens->quotes)
		tokens->separator_count += add_symbol(buf, &(tokens->line));
	else if (buf == '|' && !tokens->dquotes && !tokens->quotes)
		tokens->pipe_count += add_symbol(buf, &(tokens->line));
	else if (buf == '<' && !tokens->dquotes && !tokens->quotes)
		add_symbol(buf, &(tokens->line));
	else if (buf == ' ' && !tokens->dquotes && !tokens->quotes)
		tokens->line = ft_join_char(tokens->line, '\n');
	else
		return (1);
	return (0);
}

static void	parse2(char buf, t_tokens *tokens)
{
	int	curlinelen;

	if (buf == '>' && tokens->redirect && \
	!tokens->dquotes && !tokens->quotes)
	{
		curlinelen = ft_strlen(tokens->line);
		(tokens->line)[curlinelen - 1] = buf;
		tokens->line = ft_join_char(tokens->line, '\n');
		tokens->redirect = 0;
	}
	else if (buf == '>' && !tokens->redirect && \
	!tokens->dquotes && !tokens->quotes)
		tokens->redirect = add_symbol(buf, &(tokens->line));
	else
	{
		if (buf == '\'')
			tokens->quotes = tokens->quotes == 0 && \
			tokens->dquotes == 0 ? 1 : 0;
		if (buf == '\"')
			tokens->dquotes = tokens->dquotes == 0 && \
			tokens->quotes == 0 ? 1 : 0;
		tokens->line = ft_join_char(tokens->line, buf);
	}
}

static void	innerloop(char *buf, t_tokens *tokens)
{
	int	read_res;
	int	len;

	len = 0;
	while ((read_res = read(0, buf, 1)) > 0 && *buf != '\n')
	{
		if (parse1(*buf, tokens))
			parse2(*buf, tokens);
		len++;
	}
	if (read_res == 0 && len == 0)
	{
		write(1, "\nexit\n", 6);
		free(tokens->line);
		exit_exec(NULL, NULL, NULL, NULL);
	}
	write(1, "  ", 2);
	write(1, "\b\b", 2);
}

int			read_line(t_tokens *tokens)
{
	char	buf;

	if ((tokens->line = ft_strdup("")) == NULL)
		return (MALLOCERR);
	buf = -1;
	while (buf != '\n')
		innerloop(&buf, tokens);
	if (tokens->dquotes || tokens->quotes)
	{
		errorfd(2, "mini: ", "syntax error: unexpected end of file", "");
		free_tokens(tokens);
		init_tokens(tokens);
	}
	return (1);
}
