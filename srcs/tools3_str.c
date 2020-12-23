/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools3_str.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalecto <nalecto@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 19:08:24 by nalecto           #+#    #+#             */
/*   Updated: 2020/12/23 18:22:17 by nalecto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int					add_symbol(char buf, char **line)
{
	*line = ft_join_char(*line, '\n');
	*line = ft_join_char(*line, buf);
	*line = ft_join_char(*line, '\n');
	return (1);
}

char				*ft_join_char(char *str, char buf)
{
	int				i;
	int				len;
	char			*res;

	len = ft_strlen(str);
	if (!(res = (char*)malloc(sizeof(char) * (len + 2))))
		return (NULL);
	i = 0;
	while (i < len)
	{
		res[i] = str[i];
		i++;
	}
	res[i] = buf;
	res[i + 1] = '\0';
	free(str);
	str = NULL;
	return (res);
}

int					strequal(const char *str1, const char *str2)
{
	if (str1 && str2)
	{
		while (*str1 && *str2 && *str1 == *str2)
		{
			str1++;
			str2++;
		}
		if (*str1 == '\0' && *str2 == '\0')
			return (1);
	}
	return (0);
}

void				*append_str(char ***temp, const char *new_str)
{
	int				i;
	int				j;
	char			**realloced_temp;

	i = 0;
	while (temp && *temp && (*temp)[i] != NULL)
		i++;
	realloced_temp = malloc(sizeof(char *) * (i + 2));
	if (realloced_temp)
	{
		j = 0;
		while (j < i)
		{
			realloced_temp[j] = (*temp)[j];
			j++;
		}
		realloced_temp[j] = ft_strdup(new_str);
		realloced_temp[j + 1] = NULL;
	}
	free(*temp);
	*temp = realloced_temp;
	return (realloced_temp);
}
