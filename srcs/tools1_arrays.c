/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   tools1_arrays.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/20 16:41:52 by bshang            #+#    #+#             */
/*   Updated: 2020/12/22 22:11:14 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

int			array_length(char **array)
{
	int		i;

	i = 0;
	while (array && array[i])
		i++;
	return (i);
}

char		**array_push_back(char **array, char *str)
{
	char	**new_array;
	int		i;

	i = array_length(array);
	if (!(new_array = (char**)malloc(sizeof(char*) * (i + 2))))
		return (NULL);
	i = 0;
	while (array && array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		free(array[i]);
		i++;
	}
	free(array);
	new_array[i] = ft_strdup(str);
	new_array[i + 1] = NULL;
	return (new_array);
}

char		**array_delete_item(char ***env, int env_index)
{
	char	**new_array;
	int		i;

	if (!(new_array = (char**)malloc(sizeof(char*) * array_length(*env))))
		return (NULL);
	i = 0;
	while (i < env_index)
	{
		new_array[i] = (*env)[i];
		i++;
	}
	free((*env)[i]);
	while ((*env)[i + 1])
	{
		new_array[i] = (*env)[i + 1];
		i++;
	}
	new_array[i] = NULL;
	free(*env);
	return (new_array);
}

char		**array_dup(char **array)
{
	int		i;
	char	**new_array;

	if (!array)
		return (NULL);
	i = array_length(array);
	if (!(new_array = (char**)malloc(sizeof(char*) * (i + 1))))
		return (NULL);
	i = 0;
	while (array[i])
	{
		new_array[i] = ft_strdup(array[i]);
		i++;
	}
	new_array[i] = NULL;
	return (new_array);
}

void		array_sort(char **array)
{
	char	*temp;
	int		len;
	int		i;
	int		j;

	temp = NULL;
	j = 0;
	len = array_length(array);
	while (j < len)
	{
		i = 0;
		while (i < len - j - 1)
		{
			if (ft_strcmp(array[i], array[i + 1]) > 0)
			{
				temp = array[i];
				array[i] = array[i + 1];
				array[i + 1] = temp;
			}
			i++;
		}
		j++;
	}
	temp = NULL;
}
