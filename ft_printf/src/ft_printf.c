/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_printf.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: bshang <bshang@student.21-school.ru>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/07/03 16:25:16 by bshang            #+#    #+#             */
/*   Updated: 2020/07/22 20:58:38 by bshang           ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libftprintf.h"

static char	*check_percent(char *s, long int *res)
{
	if (*s == '%')
	{
		ft_putchar('%');
		*res += 1;
		return (++s);
	}
	else
		return (s);
}

static int	print_types(t_mdf *mdf, va_list ap, long int *res)
{
	if (mdf->type == 'd' || mdf->type == 'i')
		return (proc_digit(mdf, ap, res));
	else if (mdf->type == 'u' || mdf->type == 'x' || mdf->type == 'X')
		return (proc_u_hex(mdf, ap, res));
	else if (mdf->type == 'c')
		return (proc_char(mdf, ap));
	else if (mdf->type == 's')
		return (proc_str(mdf, ap));
	else if (mdf->type == 'p')
		return (proc_p(mdf, ap, res));
	else
		return (proc_digit(mdf, ap, res));
}

static char	*parse_substr(char *s, va_list ap, long int *res)
{
	char	*p;
	t_mdf	*mdf;

	s++;
	if (*s == '%')
		return (check_percent(s, res));
	p = s;
	while (*p && !(ft_strchr("sciduxXp", *p)) && *p != '%')
		p++;
	mdf = &(t_mdf){0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
	mdf->type = *p;
	if (p - s != 0)
		if ((s = read_flags(*(&mdf), s, res)) && *res == -1)
			return (s);
	*res += print_types(mdf, ap, res);
	s = p + 1;
	return (s);
}

static char	*print_till_spec(char *s, long int *res)
{
	if (*res == -1)
		return (s);
	while (*s != '%' && *s)
	{
		write(1, &(*s), 1);
		s++;
		*res += 1;
	}
	return (s);
}

int			ft_printf(const char *s, ...)
{
	va_list		ap;
	long int	res;

	res = 0;
	va_start(ap, s);
	if (!s)
		return (-1);
	while (*s)
	{
		if (res == -1)
			return (-1);
		if (*s == '%')
			s = parse_substr((char*)s, ap, &res);
		s = print_till_spec((char*)s, &res);
	}
	va_end(ap);
	return (res);
}
