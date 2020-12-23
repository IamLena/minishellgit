/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_p.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/18 00:00:07 by bshang            #+#    #+#             */
/*   Updated: 2020/07/25 16:49:01 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

static int	p_p(int a, int b, char *s, int minus)
{
	ft_putstr(s);
	free(s);
	if (minus && (a > b))
		pad(' ', a - 2, b);
	return (b > a ? b + 2 : a);
}

static int	print_pointer_prec(int prec, int width, char *s, int minus)
{
	int		len;

	len = (int)ft_strlen(s);
	if (prec < len)
	{
		if (!minus && (len < width - 2))
			pad(' ', width - 2, len);
		if (prec == 0 && *s == '0')
		{
			width > 2 ? ft_putstr(" ") : ft_putstr("");
			ft_putstr("0x");
			free(s);
			return (width > 0 ? width : 2);
		}
		ft_putstr("0x");
		return (p_p(width, len, s, minus));
	}
	else
	{
		if (!minus && (width > prec))
			pad(' ', width - 2, prec);
		ft_putstr("0x");
		pad('0', prec, len);
		return (p_p(width, prec, s, minus));
	}
}

int			proc_p(t_mdf *mdf, va_list arg, long int *res)
{
	long long int	digit;
	int				len;
	int				width;
	int				prec;
	char			*s;

	width = mdf->aster == 1 ? va_arg(arg, int) : mdf->width;
	prec = mdf->asterprec == 1 ? va_arg(arg, int) : mdf->prec_value;
	digit = va_arg(arg, long long int);
	if (!(s = ft_itoa_base(digit, 16)))
		*res = -1;
	len = (int)ft_strlen(s);
	if (mdf->prec)
		return (print_pointer_prec(prec, width, s, mdf->minus));
	else
	{
		if (!mdf->minus && width - 2 > len)
			mdf->zero ? pad('0', width - 2, len) : pad(' ', width - 2, len);
		ft_putstr("0x");
		ft_putstr(s);
		free(s);
		if (mdf->minus && width - 2 > len)
			pad(' ', width - 2, len);
		return (width - 2 > len ? width : len + 2);
	}
}
