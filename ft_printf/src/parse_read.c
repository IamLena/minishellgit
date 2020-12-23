/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse_read.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/17 23:59:59 by bshang            #+#    #+#             */
/*   Updated: 2020/07/24 19:47:50 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

void		pad(char ch, int a, int b)
{
	int		c;

	c = (a - b) > 0 ? a - b : (a - b) * (-1);
	while (c > 0)
	{
		write(1, &ch, 1);
		c--;
	}
}

static char	*read_num(t_mdf *mdf, char *s, long int *res)
{
	char	*p;
	char	*temp;

	p = s;
	while (*p != mdf->type && *p != '.')
		p++;
	if (!(temp = ft_substr(s, 0, p - s)))
	{
		*res = -1;
		return (p);
	}
	if (!(mdf->prec))
		mdf->width = ft_atoi(temp);
	else
		mdf->prec_value = ft_atoi(temp);
	free(temp);
	p--;
	return (p);
}

char		*read_flags(t_mdf *mdf, char *s, long int *res)
{
	while (*s != mdf->type && *s)
	{
		if (*s > 48 && *s < 58)
			s = read_num(*(&mdf), s, res);
		else if (*s == '-' && mdf->prec)
			mdf->prec_minus = 1;
		else if (*s == '-' && !mdf->prec)
			mdf->minus = 1;
		else if (*s == '0' && mdf->prec)
			mdf->prec_zero = 1;
		else if (*s == '0' && !mdf->prec)
			mdf->zero = 1;
		else if (*s == '*' && mdf->prec)
			mdf->asterprec = 1;
		else if (*s == '*' && !mdf->prec)
			mdf->aster = 1;
		else if (*s == '.')
			mdf->prec = 1;
		else
			*res = -1;
		s++;
	}
	return (s);
}

int			check_if_zero_edgecase(int width)
{
	pad(' ', width, 0);
	return (width);
}
