/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   libftprintf.h                                      :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/03 16:29:36 by bshang            #+#    #+#             */
/*   Updated: 2020/11/13 21:09:36 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef LIBFTPRINTF_H
# define LIBFTPRINTF_H

# include <stdarg.h>
# include <stdlib.h>
# include "../../libft/libft.h"

typedef struct	s_strct_mdf
{
	int			minus;
	int			zero;
	int			aster;
	char		type;
	int			prec;
	int			prec_value;
	int			prec_minus;
	int			prec_zero;
	int			asterprec;
	int			width;
}				t_mdf;

int				ft_printf(const char *s, ...);
void			pad(char ch, int a, int b);
char			*read_flags(t_mdf *mdf, char *s, long int *res);
int				check_if_zero_edgecase(int width);
int				proc_str(t_mdf *mdf, va_list arg);
int				proc_char(t_mdf *mdf, va_list arg);
int				proc_digit(t_mdf *mdf, va_list arg, long int *res);
int				proc_p(t_mdf *mdf, va_list arg, long int *res);
int				proc_u_hex(t_mdf *mdf, va_list arg, long int *res);

#endif
