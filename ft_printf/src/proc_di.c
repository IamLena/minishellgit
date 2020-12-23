/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   proc_di.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/17 23:59:29 by bshang            #+#    #+#             */
/*   Updated: 2020/07/25 12:49:56 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

static int	p_d(int a, int b, int digit, t_mdf *mdf)
{
	char	*temp;

	temp = ft_itoa(digit);
	a < 0 ? a = a * (-1) : a;
	mdf->type == '%' ? ft_putchar('%') : ft_putstr(temp);
	free(temp);
	if (mdf->minus && (a > b))
		pad(' ', a, b);
	return (b > a ? b : a);
}

static int	calc_len_and_free(int digit)
{
	int		len;
	char	*temp;

	temp = ft_itoa(digit);
	len = (int)ft_strlen(temp);
	free(temp);
	return (len);
}

static int	print_digit_prec(int prec, int width, int digit, t_mdf *mdf)
{
	int		len;

	len = mdf->type == '%' ? 1 : calc_len_and_free(digit);
	width < 0 ? width = width * (-1) : width;
	if (prec < len)
	{
		if (prec == 0 && digit == 0)
			return (check_if_zero_edgecase(width));
		if (!mdf->minus && (len < width))
			pad(' ', width, len);
		return (p_d(width, len, digit, mdf));
	}
	else
	{
		if (!mdf->minus && (width > prec))
			digit < 0 ? pad(' ', width, prec + 1) : pad(' ', width, prec);
		if (digit < 0)
		{
			ft_putchar('-');
			digit *= (-1);
			prec++;
		}
		pad('0', prec, len);
		return (p_d(width, prec, digit, mdf));
	}
}

static int	p_d_neg(int width, int len, int *digit, t_mdf *mdf)
{
	if (*digit < 0 && mdf->zero)
	{
		ft_putchar('-');
		*digit *= (-1);
	}
	mdf->zero ? pad('0', width, len) : pad(' ', width, len);
	return (*digit);
}

int			proc_digit(t_mdf *mdf, va_list arg, long int *res)
{
	int		digit;
	int		len;
	int		width;
	int		prec;
	char	*str;

	width = mdf->aster == 1 ? va_arg(arg, int) : mdf->width;
	prec = mdf->asterprec == 1 ? va_arg(arg, int) : mdf->prec_value;
	digit = va_arg(arg, long long);
	width < 0 ? mdf->minus = 1 : mdf->minus;
	prec < 0 ? mdf->prec = 0 : 1;
	if (!(str = ft_itoa(digit)))
		*res = -1;
	len = mdf->type == '%' ? 1 : (int)ft_strlen(str);
	mdf->prec = mdf->type == '%' ? 0 : mdf->prec;
	free(str);
	if (mdf->prec)
		return (print_digit_prec(prec, width, digit, mdf));
	else
	{
		if (!mdf->minus && width > len)
			digit = p_d_neg(width, len, &digit, mdf);
		return (p_d(width, len, digit, mdf));
	}
}
