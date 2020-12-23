/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: nalecto <nalecto@student.21-school.ru>     +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/11/15 16:50:20 by nalecto           #+#    #+#             */
/*   Updated: 2020/12/23 18:22:21 by nalecto          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include <stdlib.h>
# include <unistd.h>
# include <signal.h>
# include <sys/wait.h>
# include <sys/types.h>
# include <ctype.h>
# include <errno.h>
# include <string.h>
# include <sys/stat.h>
# include <fcntl.h>
# include "../libft/libft.h"
# include "../ft_printf/includes/libftprintf.h"
# include "structures.h"

/*
** errors
*/
# define OK 0
# define MALLOCERR -1
# define PARSEERR -2

/*
** tokens.c
*/
void	envparseargument(char **res, char **str, char **env, char **temp);
char	*parseargument(char *str, char **env, char **temp);
char	*parsecmdstr(char *str);

/*
** cd.c
*/
void	cd_exec(t_cmd *cmdlist, char ***env, char ***temp);

/*
** cmdist.c
*/
void	init_cmd(t_cmd *new);
t_cmd	*new_cmd();
void	cmd_push_back(t_cmd **lst, t_cmd *new);
void	print_cmd(t_cmd *list);
void	clear_cmd(t_cmd **cmdlist);

/*
** echo.c
*/
void	echo_exec(t_cmd *cmdlist, char **env, char **temp);

/*
** empty_vars.c
*/
void	empty_exec(t_cmd *cmdlist, char ***env, char ***temp);
int		cmd_out_of_vars(t_cmd *cmdlist, char **env, char **temp);

/*
** env.c
*/
void	env_exec(t_cmd *cmdlist, char **env);

/*
** execcmdloop.c
*/
void	exec_cmd(t_cmd *cmdhead, t_cmd *cmdlist, char ***env, char ***temp);

/*
** execve.c
*/
char	**array_for_execve(t_cmd *cmdlist, char **env, char **temp);
void	calling_execve(t_cmd *cmdlist, char **env, char **temp);

/*
** exit.c
*/
void	exit_exec(t_cmd *headcmd, t_cmd *cmdlist, char **env, char **temp);

/*
** export.c
*/
void	rewrite_value(char ***env, int idx, char *str);
void	export_exec(t_cmd *cmdlist, char ***env, char ***temp);

/*
** findexeccmd.c
*/
void	ft_append_str(char **old, char *add);
char	*find_cmd(t_cmd *cmdlist, char **env, char **temp);

/*
** main.c
*/
void	signal_handler(int signum);

/*
** pwd.c
*/
void	pwd_exec(t_cmd *cmdlist);

/*
** token_to_cmd.c
*/
void	parse_to_cmd_list(t_tokens *tokens, t_cmd **cmdlist);

/*
** tokens.c
*/
void	init_tokens(t_tokens *tokens);
int		read_line(t_tokens *tokens);

/*
** tools1_arrays.c - some of array_tools
*/
int		array_length(char **array);
char	**array_dup(char **array);
void	array_sort(char **array);
char	**array_push_back(char **array, char *str);
char	**array_delete_item(char ***env, int env_index);

/*
** tools2.c
*/
int		reset_if_set(char **temp, int equalsignpos, const char *cmdstr);
int		which_env(char **env, char *arg);
char	*find_env_path(char *env_name, char **env);
void	my_strjoin(char **str, const char *new, int len);

/*
** tools3_str.c - tools for strings
*/
int		add_symbol(char buf, char **line);
char	*ft_join_char(char *str, char buf);
int		strequal(const char *str1, const char *str2);
void	*append_str(char ***temp, const char *new_str);

/*
** tools4_output.c - tools for messages & debug
*/
void	errorfd(int fd, char *cmd, char *msg, char *cmdstr);
void	put_error_message(char *s1, char *s2, char *s3, int fd);
void	print_list(t_list *head);
void	print_array(char **arr);
int		return_print_error(char *str);

/*
** tools5_free.c - memory management
*/
void	free_tokens(t_tokens *tokens);
void	free_tokens_clear_cmd(t_tokens *tokens, t_cmd **cmdlist);
void	array_clear(char **array);

/*
** tools6_redirects.c - redirects and streams
*/
int		check_redirects_and_streams(t_cmd *cmdlist);
void	execve_redirect_out(t_cmd *cmdlist);
void	execve_redirect_in(t_cmd *cmdlist);
int		checkredirect(char *curtoken, char *nexttoken);
int		check_redirects_and_pipes(t_cmd *cmdlist);

/*
** unset.c
*/
void	unset_exec(t_cmd *cmdlist, char ***env, char ***temp);

#endif
