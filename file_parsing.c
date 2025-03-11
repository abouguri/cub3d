/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   file_parsing.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 03:43:11 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 03:43:41 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	initialize_resources(t_cub *data)
{
	data->textures = calloc(TEXTURE_COUNT + 1, sizeof(char *));
	data->colors = calloc(RGB_COUNT + 1, sizeof(char *));
	if (!data->textures || !data->colors)
		return (1);
	return (0);
}

int	parse_file(int fd)
{
	char	*line;
	int		ret;
	t_cub	*data;

	line = NULL;
	data = get_cub_data();
	if (initialize_resources(data))
		return (1);
	while (ft_array_length(data->textures) != TEXTURE_COUNT
		|| ft_array_length(data->colors) != RGB_COUNT)
	{
		ret = get_next_line(fd, &line);
		if (ret == -1)
		{
			free(line);
			return (1);
		}
		if (process_line(line))
			return (1);
		if (ret == 0)
			break ;
	}
	return (0);
}

int	append_line_to_map(t_cub *data, char *line)
{
	char	**tmp;
	int		current_size;

	current_size = ft_array_length(data->map);
	tmp = reallocate_map_memory(data->map, current_size + 2);
	if (!tmp)
		return (1);
	data->map = tmp;
	data->map[current_size] = strdup(line);
	data->map[current_size + 1] = NULL;
	return (0);
}
