/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_itoa.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 15:07:27 by abouguri          #+#    #+#             */
/*   Updated: 2023/11/21 21:21:59 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_count(long long int n)
{
	int	count;

	count = 1;
	if (n < 0)
		n = -n;
	while (n >= 10)
	{
		n = n / 10;
		count++;
	}
	return (count);
}

static void	ft_tbe9(long long int n, int i, char *s)
{
	if (n < 0)
	{
		s[0] = '-';
		n = -n;
		while (i > 0)
		{
			s[i] = n % 10 + 48;
			n = n / 10;
			i--;
		}
	}
	else
	{
		while (i >= 0)
		{
			s[i] = n % 10 + 48;
			n = n / 10;
			i--;
		}
	}
}

char	*ft_itoa(int n)
{
	char	*s;
	int		count;
	int		i;

	count = ft_count(n);
	if (n < 0)
		count = count + 1;
	s = (char *)malloc((count + 1) * sizeof(char));
	if (!s)
		return (NULL);
	i = count - 1;
	ft_tbe9(n, i, s);
	s[count] = '\0';
	return (s);
}
