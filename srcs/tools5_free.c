/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools5_free.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 21:31:41 by bshang            #+#    #+#             */
/*   Updated: 2020/12/22 22:45:10 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		free_tokens(t_tokens *tokens)
{
	char	**arr;
	int		i;

	arr = tokens->tokens;
	i = 0;
	while (arr && arr[i])
	{
		free(arr[i]);
		i++;
	}
	free(tokens->tokens);
	tokens->tokens = NULL;
	free(tokens->line);
	tokens->line = NULL;
}

void		free_tokens_clear_cmd(t_tokens *tokens, t_cmd **cmdlist)
{
	free_tokens(tokens);
	clear_cmd(cmdlist);
	return ;
}

void		array_clear(char **array)
{
	int		i;

	if (!array)
		return ;
	i = 0;
	while (array[i])
		free(array[i++]);
	free(array);
}
