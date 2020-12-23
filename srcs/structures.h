/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structures.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/22 20:11:22 by nalecto           #+#    #+#             */
/*   Updated: 2020/12/23 00:49:10 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTURES_H
# define STRUCTURES_H

# include "../libft/libft.h"

typedef struct		s_cmd
{
	char			*cmdstr;
	t_list			*args;
	t_list			*instream;
	t_list			*outstream;
	int				readpipe;
	int				writepipe;
	struct s_cmd	*next;
}					t_cmd;

typedef struct		s_tokens
{
	char			*line;
	int				cur_token;
	char			**tokens;
	int				separator_count;
	int				quotes;
	int				dquotes;
	int				shield;
	int				pipe_count;
	int				redirect;
}					t_tokens;

int					g_proc_status;

t_cmd				*g_cmdlist;
t_tokens			g_tokens;
char				**g_env_dup;
char				**g_temp;

#endif
