/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memmove.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 18:00:15 by abouguri          #+#    #+#             */
/*   Updated: 2023/11/23 03:00:47 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static void	ft_dir(unsigned char *d, const unsigned char *s, int i, size_t n)
{
	if (d < s)
	{
		while (n--)
		{
			d[i] = s[i];
			i++;
		}
	}
	else
	{
		i = 0;
		d = d + n;
		s = s + n;
		while (n--)
			*(--d) = *(--s);
	}
}

void	*ft_memmove(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	int					i;

	s = (const unsigned char *)src;
	d = (unsigned char *)dest;
	i = 0;
	if (d == s)
		return (dest);
	ft_dir(d, s, i, n);
	return (dest);
}
