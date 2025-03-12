/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_strtrim.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 21:29:06 by abouguri          #+#    #+#             */
/*   Updated: 2023/11/23 03:31:09 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h"

static int	ft_check1(char c, char const *set)
{
	while (*set)
	{
		if (*set == c)
			return (1);
		set++;
	}
	return (0);
}

static char	*ft_substr1(char const *s1, int i, int len)
{
	char	*sub;
	int		j;

	j = 0;
	sub = (char *) malloc (sizeof(char) * (len + 1));
	if (!sub)
		return (NULL);
	while (j < len)
	{
		sub[j] = s1[i];
		j++;
		i++;
	}
	sub[j] = '\0';
	return (sub);
}

char	*ft_strtrim(char const *s1, char const *set)
{
	int	i;
	int	j;

	if (!s1)
		return (NULL);
	i = 0;
	j = ft_strlen(s1) - 1;
	if (!set)
		return (ft_strdup((char *)s1));
	while (s1[i] && ft_check1(s1[i], set))
		i++;
	if (s1[i] == '\0')
		return (ft_strdup(""));
	while (s1[j] && ft_check1(s1[j], set))
		j--;
	return (ft_substr1(s1, i, j - i + 1));
}
