/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memcpy.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 17:44:21 by abouguri          #+#    #+#             */
/*   Updated: 2023/11/22 22:03:11 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memcpy(void *dest, const void *src, size_t n)
{
	unsigned char		*d;
	const unsigned char	*s;
	int					i;

	d = dest;
	s = src;
	i = 0;
	if (!dest && !src)
		return (NULL);
	while (n > 0 && dest != src)
	{
		d[i] = s[i];
		i++;
		n--;
	}
	return (dest);
}
