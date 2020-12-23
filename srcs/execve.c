/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   execve.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 23:16:36 by nalecto           #+#    #+#             */
/*   Updated: 2020/12/23 06:13:13 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

char		**array_for_execve(t_cmd *cmdlist, char **env, char **temp)
{
	char	**arr;
	t_list	*start;
	int		len;
	int		i;

	len = ft_lstsize(cmdlist->args);
	if (!(arr = (char**)malloc(sizeof(char*) * (len + 2))))
		return (NULL);
	i = 0;
	arr[i++] = ft_strdup(cmdlist->cmdstr);
	len++;
	start = cmdlist->args;
	while (i < len)
	{
		arr[i] = parseargument(cmdlist->args->content, env, temp);
		cmdlist->args = cmdlist->args->next;
		i++;
	}
	cmdlist->args = start;
	arr[i] = NULL;
	return (arr);
}

static void	childcode(t_cmd *cmdlist, char **argv_s, char **env)
{
	execve_redirect_in(cmdlist);
	execve_redirect_out(cmdlist);
	signal(SIGINT, SIG_DFL);
	signal(SIGQUIT, SIG_DFL);
	execve(cmdlist->cmdstr, argv_s, env);
	errorfd(2, "mini: ", strerror(errno), cmdlist->cmdstr);
	array_clear(argv_s);
	exit(errno);
}

static void	signal_and_wait(int pid, int *status)
{
	signal(SIGINT, SIG_IGN);
	signal(SIGQUIT, SIG_IGN);
	waitpid(pid, status, 0);
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
}

void		calling_execve(t_cmd *cmdlist, char **env, char **temp)
{
	int		pid;
	char	**argv_s;
	int		status;

	if (!(argv_s = array_for_execve(cmdlist, env, temp)))
		return ;
	pid = fork();
	if (pid == 0)
		childcode(cmdlist, argv_s, env);
	signal_and_wait(pid, &status);
	array_clear(argv_s);
	if (WIFSIGNALED(status) && WTERMSIG(status) == 2)
		g_proc_status = 130;
	if (WIFSIGNALED(status) && WTERMSIG(status) == 3)
	{
		ft_printf("Quit:\t3\n");
		g_proc_status = 131;
	}
	if (WIFEXITED(status) && WEXITSTATUS(status) == 13)
		g_proc_status = 126;
	else if (WIFEXITED(status))
		g_proc_status = WEXITSTATUS(status);
}
