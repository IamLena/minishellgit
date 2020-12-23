/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_c.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/24 19:28:15 by bshang            #+#    #+#             */
/*   Updated: 2020/07/24 20:05:38 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

int			proc_char(t_mdf *mdf, va_list arg)
{
	int		ch;
	int		width;

	width = mdf->aster == 1 ? va_arg(arg, int) : mdf->width;
	if (width < 0)
	{
		mdf->minus = 1;
		width *= (-1);
	}
	ch = va_arg(arg, int);
	if (!mdf->minus && width > 1)
		pad(' ', width, 1);
	ft_putchar(ch);
	if (mdf->minus && width > 1)
		pad(' ', width, 1);
	return (width > 1 ? width : 1);
}
