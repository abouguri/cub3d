/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   extra.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 19:33:54 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/12 05:49:31 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_cub	*get_cub_data(void)
{
	static t_cub	data;

	return (&data);
}

char	**reallocate_map_memory(char **pointer, int size)
{
	int		i;
	char	**new_pointer;

	i = 0;
	new_pointer = malloc(sizeof(char *) * size);
	if (!new_pointer)
		return (NULL);
	while (pointer && pointer[i])
	{
		new_pointer[i] = pointer[i];
		i++;
	}
	new_pointer[i] = NULL;
	free(pointer);
	return (new_pointer);
}

int	initialize_map(t_cub *data, char *line)
{
	data->map = malloc(sizeof(char *) * 2);
	if (!data->map)
		return (1);
	data->map[0] = ft_strdup(line);
	data->map[1] = NULL;
	return (0);
}

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

char	*trim_line(const char *line)
{
	const char	*start = line;
	const char	*end;
	int			length;
	char		*trimmed;

	end = start + ft_strlen(start) - 1;
	while (end > start && is_whitespace(*end))
		end--;
	length = end - start + 1;
	trimmed = malloc(length + 1);
	if (!trimmed)
		return (NULL);
	ft_strncpy(trimmed, start, length);
	trimmed[length] = '\0';
	return (trimmed);
}
