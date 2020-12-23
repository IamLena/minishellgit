/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   args.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalecto <nalecto@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/29 04:10:23 by nalecto           #+#    #+#             */
/*   Updated: 2020/12/23 19:03:49 by nalecto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		envparseargument(char **res, char **str, char **env, char **temp)
{
	int		array_index;
	int		var_len;
	char	*value_ptr;

	var_len = 0;
	while ((*str)[var_len] && ft_isalnum((*str)[var_len]))
		var_len++;
	array_index = which_env(env, *str);
	if (array_index != -1)
	{
		value_ptr = env[array_index] + var_len + 1;
		my_strjoin(res, value_ptr, ft_strlen(value_ptr));
		*str += var_len - 1;
		return ;
	}
	array_index = which_env(temp, *str);
	if (array_index != -1)
	{
		value_ptr = temp[array_index] + var_len + 1;
		my_strjoin(res, value_ptr, ft_strlen(value_ptr));
		*str += var_len - 1;
		return ;
	}
	*str += var_len - 1;
}

static int	innerloop1(char **str, unsigned char *outside_single_q, \
unsigned char *outside_double_q, char **result)
{
	char	*statusstr;

	if (**str == '\\')
	{
		if (!*outside_single_q)
			my_strjoin(result, *str, 1);
		my_strjoin(result, *str + 1, 1);
		(*str)++;
	}
	else if (**str == '\"' && *outside_single_q)
		*outside_double_q = (*outside_double_q + 1) % 2;
	else if (**str == '\'' && *outside_double_q)
		*outside_single_q = (*outside_single_q + 1) % 2;
	else if (**str == '$' && *((*str) + 1) == '?' && *outside_single_q)
	{
		statusstr = ft_itoa(g_proc_status);
		my_strjoin(result, statusstr, ft_strlen(statusstr));
		free(statusstr);
		(*str)++;
	}
	else
		return (1);
	return (0);
}

char		*parseargument(char *str, char **env, char **temp)
{
	char			*result;
	unsigned char	outside_single_q;
	unsigned char	outside_double_q;

	result = NULL;
	outside_single_q = 1;
	outside_double_q = 1;
	while (str && *str)
	{
		if (innerloop1(&str, &outside_single_q, &outside_double_q, &result))
		{
			if (*str == '$' && (*(str + 1) == '\0' || !ft_isalnum(*(str + 1))))
				my_strjoin(&result, str, 1);
			else if (*str == '$' && outside_single_q)
			{
				(str)++;
				envparseargument(&result, &str, env, temp);
			}
			else
				my_strjoin(&result, str, 1);
		}
		str++;
	}
	return (result);
}

static int	slashforcmd(char **str, unsigned char *outside_single_q, \
unsigned char *outside_double_q, char **result)
{
	if (**str == '\\')
	{
		if (!*outside_single_q)
			my_strjoin(result, *str, 1);
		my_strjoin(result, *str + 1, 1);
		(*str)++;
	}
	else if (**str == '\"' && *outside_single_q)
		*outside_double_q = (*outside_double_q + 1) % 2;
	else if (**str == '\'' && *outside_double_q)
		*outside_single_q = (*outside_single_q + 1) % 2;
	else
		return (0);
	return (1);
}

char		*parsecmdstr(char *str)
{
	char			*result;
	unsigned char	outside_single_q;
	unsigned char	outside_double_q;
	char			*status;

	result = NULL;
	outside_single_q = 1;
	outside_double_q = 1;
	while (str && *str)
	{
		if (!slashforcmd(&str, &outside_single_q, &outside_double_q,\
		&result))
		{
			if (outside_single_q && *str == '$' && *(str + 1) == '?')
			{
				status = ft_itoa(g_proc_status);
				my_strjoin(&result, status, ft_strlen(status));
				str += 2;
			}
			else
				my_strjoin(&result, str, 1);
		}
		str++;
	}
	return (result);
}
