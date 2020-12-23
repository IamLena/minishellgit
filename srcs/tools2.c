/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools2.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalecto <nalecto@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 16:41:52 by bshang            #+#    #+#             */
/*   Updated: 2020/12/23 04:31:08 by nalecto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			reset_if_set(char **temp, int equalsignpos, const char *cmdstr)
{
	int		i;

	if (temp)
	{
		i = 0;
		while (temp[i])
		{
			if (ft_strncmp(temp[i], cmdstr, equalsignpos) == 0)
			{
				free(temp[i]);
				temp[i] = ft_strdup(cmdstr);
				return (0);
			}
			i++;
		}
	}
	return (-1);
}

int			which_env(char **env, char *arg)
{
	char	*name;
	int		len;
	int		i;

	if (!arg || !env)
		return (-1);
	i = 0;
	len = 0;
	while (arg[len] != '=' && arg[len] && ft_isalnum(arg[len]))
		len++;
	while (env[i])
	{
		if (ft_strncmp(arg, env[i], len) == 0 && (env[i][len] == '='
		|| env[i][len] == '\0'))
			return (i);
		i++;
	}
	return (-1);
}

char		*find_env_path(char *env_name, char **env)
{
	char	*homepath;
	int		len;
	int		i;

	len = ft_strlen(env_name);
	homepath = NULL;
	i = 0;
	if (env)
	{
		while (env[i])
		{
			if (ft_strncmp(env[i], env_name, len) == 0 && (env[i])[len] &&
			(env[i])[len] == '=')
			{
				if (!(homepath = ft_substr(env[i], ft_strlen(env_name) + 1,
				ft_strlen(env[i]) - len - 1)))
					return (NULL);
			}
			i++;
		}
	}
	return (homepath);
}

void		my_strjoin(char **str, const char *new, int len)
{
	char	*tmp;
	int		index;
	int		newlen;

	if (len > 0 && len <= ft_strlen(new))
	{
		newlen = ft_strlen(*str) + len + 1;
		if ((tmp = malloc(newlen)))
		{
			index = 0;
			while (*str && (*str)[index] != '\0')
			{
				tmp[index] = (*str)[index];
				index++;
			}
			while (index < newlen - 1)
			{
				tmp[index++] = *new;
				new++;
			}
			tmp[index] = 0;
			free(*str);
			*str = tmp;
		}
	}
}
