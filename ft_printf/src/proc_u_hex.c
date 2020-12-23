/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_u_hex.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/17 23:59:52 by bshang            #+#    #+#             */
/*   Updated: 2020/07/24 18:00:38 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

static void	pc(char *str, int flag, int length)
{
	int		a;

	while (*str && length > 0)
	{
		if (flag == 0)
			ft_putchar(*str);
		else
		{
			a = ft_toupper(*str);
			ft_putchar(a);
		}
		str++;
		length--;
	}
}

static void	p_u_h(int a, int b, char *s, t_mdf *mdf)
{
	if (mdf->type == 'u')
		ft_putstr(s);
	else
		mdf->type == 'X' ? pc(s, 1, b) : pc(s, 0, b);
	if (mdf->minus && (b < a))
		pad(' ', a, b);
	free(s);
}

static int	print_u_h(int p, int w, char *s, t_mdf *mdf)
{
	int		l;

	l = (int)ft_strlen(s);
	if (p == 0 && *s == '0')
	{
		pad(' ', w, 0);
		free(s);
		return (w < 0 ? w * (-1) : w);
	}
	if (p < l)
	{
		if (!mdf->minus && (l < w))
			p < 0 && mdf->zero ? pad('0', w - l + 1, p * (-1)) : pad(' ', w, l);
		p_u_h(w, l, s, mdf);
		return (l > w ? l : w);
	}
	else
	{
		if (!mdf->minus && (p < w))
			pad(' ', w, p);
		pad('0', p, l);
		p_u_h(w, p, s, mdf);
		return (p > w ? p : w);
	}
}

int			proc_u_hex(t_mdf *mdf, va_list arg, long int *res)
{
	unsigned int	dig;
	int				len;
	int				width;
	int				prec;
	char			*s;

	width = mdf->aster == 1 ? va_arg(arg, int) : mdf->width;
	prec = mdf->asterprec == 1 ? va_arg(arg, int) : mdf->prec_value;
	dig = va_arg(arg, unsigned int);
	if (!(s = mdf->type == 'u' ? ft_itoa_base(dig, 10) : ft_itoa_base(dig, 16)))
		*res = -1;
	len = (int)ft_strlen(s);
	if (mdf->prec)
		return (print_u_h(prec, width, s, mdf));
	else
	{
		if (!mdf->minus && width > len)
			mdf->zero ? pad('0', width, len) : pad(' ', width, len);
		p_u_h(width, len, s, mdf);
		return (width > len ? width : len);
	}
}
