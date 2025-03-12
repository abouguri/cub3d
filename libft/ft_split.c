/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_split.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/11/19 15:13:21 by abouguri          #+#    #+#             */
/*   Updated: 2023/11/22 22:03:41 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "libft.h" 

static int	ft_count(char const *s, char c)
{
	int	count;
	int	i;

	count = 0;
	i = 0;
	if (s == NULL)
		return (0);
	while (s[i] != '\0')
	{
		if (s[i] != c && (s[i + 1] == c || s[i + 1] == '\0'))
			count++;
		i++;
	}
	return (count);
}

static char	**ft_free(int i, char **split)
{
	while (i >= 0)
	{
		free(split[i]);
		i--;
	}
	free (split);
	return (NULL);
}

static char	*ft_fill(char const *s, char c)
{
	int		i;
	char	*sub;

	i = 0;
	while (s[i] != '\0' && s[i] != c)
		i++;
	sub = (char *)malloc((i + 1) * sizeof(char));
	if (!sub)
		return (NULL);
	i = 0;
	while (s[i] != '\0' && s[i] != c)
	{
		sub[i] = s[i];
		i++;
	}
	sub[i] = '\0';
	return (sub);
}

char	**ft_split(char const *s, char c)
{
	int		i;
	char	**split;

	if (!s)
		return (NULL);
	i = 0;
	split = malloc (sizeof (char *) * (ft_count(s, c) + 1));
	if (split == NULL)
		return (NULL);
	while (*s)
	{
		while (*s && *s == c)
			s++;
		if (*s == '\0')
			break ;
		split[i] = ft_fill(s, c);
		if (!split[i])
			return (ft_free(i, split));
		s = s + ft_strlen(split[i]);
		i++;
	}
	split[i] = NULL;
	return (split);
}
