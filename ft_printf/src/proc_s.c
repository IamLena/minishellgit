/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_s.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/17 23:59:42 by bshang            #+#    #+#             */
/*   Updated: 2020/07/24 19:04:13 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

static void	ft_putstr_part(char *str, int p)
{
	while (p > 0 && *str)
	{
		write(1, *(&str), 1);
		p--;
		str++;
	}
}

static int	put_aligned(char *str, int a, int b, int minus)
{
	if (!minus)
		pad(' ', a, b);
	ft_putstr_part(str, b);
	if (minus)
		pad(' ', a, b);
	return (a);
}

static int	print_str_prec(int prec, int width, char *str, int minus)
{
	int	len;

	len = (int)ft_strlen(str);
	if (width < 0)
	{
		minus = 1;
		width *= -1;
	}
	if (width < prec && width <= len)
	{
		ft_putstr_part(str, prec);
		return (len > prec ? prec : len);
	}
	else if (len <= width && len < prec)
		return (put_aligned(str, width, len, minus));
	else
		return (put_aligned(str, width, prec, minus));
}

static void	check_neg_and_set_field(int *value, int *field, int i)
{
	if (*value < 0)
	{
		*field = i;
		*value *= -1;
	}
}

int			proc_str(t_mdf *mdf, va_list arg)
{
	char	*str;
	int		len;
	int		width;
	int		prec;

	width = mdf->aster == 1 ? va_arg(arg, int) : mdf->width;
	prec = mdf->asterprec ? va_arg(arg, int) : mdf->prec_value;
	prec < 0 ? mdf->prec_minus = 1 : 0;
	check_neg_and_set_field(&width, &mdf->minus, 1);
	if (!(str = va_arg(arg, char *)))
		str = "(null)";
	len = (int)ft_strlen(str);
	if (mdf->prec && !mdf->prec_minus && len)
		return (print_str_prec(prec, width, str, mdf->minus));
	else if (mdf->prec && mdf->prec_minus)
		return (print_str_prec(len, width, str, mdf->minus));
	else
	{
		if (!mdf->minus && width > len)
			pad(' ', width, len);
		ft_putstr(str);
		if (mdf->minus && width > len)
			pad(' ', width, len);
		return (width > len ? width : len);
	}
}
