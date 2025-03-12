/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_memset.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/01 02:10:39 by abouguri          #+#    #+#             */
/*   Updated: 2023/11/23 02:56:15 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

void	*ft_memset(void *b, int c, size_t len)
{
	unsigned char	*p;
	unsigned char	val;
	size_t			i;

	val = (unsigned char)c;
	p = b;
	i = 0;
	while (i < len)
	{
		p[i] = val;
		i++;
	}
	return (b);
}
