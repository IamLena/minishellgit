/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   empty_vars.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalecto <nalecto@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 22:37:59 by nalecto           #+#    #+#             */
/*   Updated: 2020/12/23 19:36:01 by nalecto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static void	resetargascmd(t_cmd *cmdlist)
{
	t_list	*nextarg;

	if (cmdlist->args)
	{
		free(cmdlist->cmdstr);
		cmdlist->cmdstr = cmdlist->args->content;
		nextarg = cmdlist->args->next;
		free(cmdlist->args);
		cmdlist->args = nextarg;
	}
}

void		empty_exec(t_cmd *cmdlist, char ***env, char ***temp)
{
	t_list	*outstream;
	int		fd;

	outstream = cmdlist->outstream;
	while (outstream)
	{
		fd = open(outstream->content, outstream->size, \
		S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
		if (fd == -1)
		{
			errorfd(2, "echo: ", strerror(errno), outstream->content);
			g_proc_status = 127;
		}
		else
			close(fd);
		outstream = outstream->next;
	}
	resetargascmd(cmdlist);
}

static int	replacevar(char *run, char **env, char **temp, char **new)
{
	int		var_len;
	int		array_index;
	char	*tmp;

	if (*run == '$')
	{
		run++;
		var_len = 0;
		while (run[var_len] && ft_isalnum(run[var_len]))
			var_len++;
		array_index = which_env(env, run);
		if (array_index != -1)
			ft_append_str(new, env[array_index] + var_len + 1);
		else if ((array_index = which_env(temp, run)) != -1)
			ft_append_str(new, temp[array_index] + var_len + 1);
		return (var_len + 1);
	}
	if ((tmp = malloc(2)))
	{
		*tmp = *run;
		*(tmp + 1) = '\0';
		ft_append_str(new, tmp);
		free(tmp);
	}
	return (1);
}

static void	getsplit(char *newcmd, t_cmd *cmdlist)
{
	char	**split;
	int		i;
	t_list	*newlistnode;

	split = ft_split(newcmd, ' ');
	free(cmdlist->cmdstr);
	cmdlist->cmdstr = split[0];
	if (split[0])
	{
		i = 1;
		while (split[i] != NULL)
		{
			newlistnode = ft_lstnew(split[i], 0);
			ft_lstadd_front(&cmdlist->args, newlistnode);
			i++;
		}
	}
	free(split);
}

int			cmd_out_of_vars(t_cmd *cmdlist, char **env, char **temp)
{
	char	*new;
	char	*run;
	int		i;

	new = NULL;
	if (!cmdlist->cmdstr)
		return (0);
	run = cmdlist->cmdstr;
	if (!ft_strchr(run, '$'))
		return (0);
	while (*run)
	{
		run += replacevar(run, env, temp, &new);
		if (*run == '\0' && new == NULL)
			new = ft_strdup("");
	}
	if (!new)
		return (0);
	getsplit(new, cmdlist);
	return (1);
}
