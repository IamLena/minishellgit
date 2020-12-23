/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execcmdloop.c                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalecto <nalecto@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 19:24:30 by nalecto           #+#    #+#             */
/*   Updated: 2020/12/23 05:53:42 by nalecto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

static int	builtins(t_cmd *cmdhead, t_cmd *cmdlist, char ***env, char ***temp)
{
	int	rc;

	rc = 1;
	if (strequal(cmdlist->cmdstr, "echo"))
		echo_exec(cmdlist, *env, *temp);
	else if (strequal(cmdlist->cmdstr, "cd"))
		cd_exec(cmdlist, env, temp);
	else if (strequal(cmdlist->cmdstr, "pwd"))
		pwd_exec(cmdlist);
	else if (strequal(cmdlist->cmdstr, "export"))
		export_exec(cmdlist, env, temp);
	else if (strequal(cmdlist->cmdstr, "unset"))
		unset_exec(cmdlist, env, temp);
	else if (strequal(cmdlist->cmdstr, "env"))
		env_exec(cmdlist, *env);
	else if (strequal(cmdlist->cmdstr, "exit"))
		exit_exec(cmdhead, cmdlist, *env, *temp);
	else
		rc = 0;
	return (rc);
}

static int	set_cmd(int *doublepass, t_cmd *cmdlist, char ***env, char ***temp)
{
	char	*equalsign;
	char	*parsedarg;
	t_list	*nextarg;

	if (!((equalsign = ft_strchr(cmdlist->cmdstr, '=')) && equalsign > \
	cmdlist->cmdstr && *(equalsign - 1) != '\\' && *(equalsign + 1) != '\0'\
	&& !ft_isdigit(*(cmdlist->cmdstr))))
		return (0);
	if (cmdlist->args)
	{
		free(cmdlist->cmdstr);
		cmdlist->cmdstr = cmdlist->args->content;
		nextarg = cmdlist->args->next;
		free(cmdlist->args);
		cmdlist->args = nextarg;
		*doublepass = 1;
		return (1);
	}
	parsedarg = parseargument(cmdlist->cmdstr, NULL, NULL);
	if (reset_if_set(*temp, equalsign - cmdlist->cmdstr, parsedarg) == -1)
		*temp = array_push_back(*temp, parsedarg);
	reset_if_set(*env, equalsign - cmdlist->cmdstr, parsedarg);
	free(parsedarg);
	return (1);
}

static int	notbuilt(t_cmd *cmdhead, t_cmd *cmdlist, char ***env, char ***temp)
{
	int			doublepass;
	char		*wholepath;
	struct stat	buffer;

	doublepass = 0;
	if ((wholepath = find_cmd(cmdlist, *env, *temp)))
		calling_execve(cmdlist, *env, *temp);
	else if (g_proc_status == 126)
		return (0);
	else if (cmd_out_of_vars(cmdlist, *env, *temp))
		return (1);
	else if (!set_cmd(&doublepass, cmdlist, env, temp))
	{
		if (stat(cmdlist->cmdstr, &buffer) != 0 && \
		(ft_strchr(cmdlist->cmdstr, '/') || g_proc_status == 127))
			errorfd(2, "mini: ", cmdlist->cmdstr, \
			": No such file or directory");
		else
			errorfd(2, "mini: ", "command not found: ", cmdlist->cmdstr);
		g_proc_status = 127;
	}
	return (doublepass);
}

static int	ft_switch(t_cmd *cmdhead, t_cmd *cmdlist, char ***env, char ***temp)
{
	int			doublepass;

	doublepass = 0;
	if (cmdlist->cmdstr == NULL)
	{
		empty_exec(cmdlist, env, temp);
		return (cmdlist->cmdstr != NULL);
	}
	if (!builtins(cmdhead, cmdlist, env, temp))
		doublepass = notbuilt(cmdhead, cmdlist, env, temp);
	return (doublepass);
}

void		exec_cmd(t_cmd *cmdhead, t_cmd *cmdlist, char ***env, char ***temp)
{
	char	doublepass_flag;

	doublepass_flag = 1;
	while (cmdlist && doublepass_flag)
		doublepass_flag = ft_switch(cmdhead, cmdlist, env, temp);
}
