/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalecto <nalecto@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 19:02:31 by nalecto           #+#    #+#             */
/*   Updated: 2020/12/23 18:46:32 by nalecto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void		signal_handler(int signum)
{
	if (signum == SIGINT)
	{
		g_proc_status = 1;
		write(1, "\b\b  \b\b\n", 7);
		write(1, "mimi> ", 6);
	}
	else if (signum == SIGQUIT)
		write(1, "\b\b  \b\b", 6);
}

static void	execwpipe(t_cmd *cmdhead, t_cmd *cmdlist, char ***env, char ***temp)
{
	dup2(1, 2);
	if (cmdlist->readpipe != -1)
	{
		dup2(cmdlist->readpipe, 0);
		close(cmdlist->readpipe);
	}
	if (cmdlist->writepipe != -1)
	{
		dup2(cmdlist->writepipe, 1);
		close(cmdlist->writepipe);
	}
	exec_cmd(cmdhead, cmdlist, env, temp);
}

static void	no_wpipe(t_cmd *cmdhead, t_cmd *cmdlist, char ***env, char ***temp)
{
	int tempfd[3];

	tempfd[0] = dup(0);
	tempfd[1] = dup(1);
	tempfd[2] = dup(2);
	execwpipe(cmdhead, cmdlist, env, temp);
	dup2(tempfd[0], 0);
	dup2(tempfd[1], 1);
	dup2(tempfd[2], 2);
	close(tempfd[0]);
	close(tempfd[1]);
	close(tempfd[2]);
}

static void	execute_func(t_cmd *cmdlist, char ***env, char ***temp)
{
	int		pid;
	int		status;
	t_cmd	*cmdhead;

	cmdhead = cmdlist;
	while (cmdlist)
	{
		if (cmdlist->writepipe == -1)
			no_wpipe(cmdhead, cmdlist, env, temp);
		else if ((pid = fork()) == 0)
		{
			execwpipe(cmdhead, cmdlist, env, temp);
			exit_exec(cmdhead, NULL, *env, *temp);
		}
		else
		{
			waitpid(pid, &status, 0);
			close(cmdlist->readpipe);
			close(cmdlist->writepipe);
		}
		cmdlist = cmdlist->next;
	}
}

int			main(int argc, char **argv, char **envp)
{
	t_cmd	*cmdhead;

	g_proc_status = 0;
	signal(SIGINT, signal_handler);
	signal(SIGQUIT, signal_handler);
	g_temp = NULL;
	g_cmdlist = NULL;
	init_tokens(&g_tokens);
	if ((g_env_dup = array_dup(envp)) == NULL)
		return (MALLOCERR);
	while (write(1, "mimi> ", 6) && read_line(&g_tokens) > 0)
	{
		if (g_tokens.line == NULL)
			continue ;
		parse_to_cmd_list(&g_tokens, &g_cmdlist);
		cmdhead = g_cmdlist;
		execute_func(g_cmdlist, &g_env_dup, &g_temp);
		clear_cmd(&cmdhead);
		g_cmdlist = NULL;
	}
	return (g_proc_status);
}
