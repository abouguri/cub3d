/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   texture_parsing.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 03:42:15 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 03:42:54 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	parse_texture(t_cub *data, char *identifier, char *path, char **tokens)
{
	int	index;

	if (ft_strncmp(identifier, "NO", 3) == 0)
		index = 0;
	else if (ft_strncmp(identifier, "SO", 3) == 0)
		index = 1;
	else if (ft_strncmp(identifier, "WE", 3) == 0)
		index = 2;
	else if (ft_strncmp(identifier, "EA", 3) == 0)
		index = 3;
	else
		return (map_info_error(tokens, UNKNOWN_IDENTIFIER), 1);
	if (data->textures[index])
		return (map_info_error(tokens, DUPLICATE_TEXTURE), 1);
	data->textures[index] = strdup(path);
	return (0);
}

int	parse_color(t_cub *data, char *identifier, char *value, char **tokens)
{
	int	index;

	if (ft_strncmp(identifier, "F", 2) == 0)
		index = 0;
	else if (ft_strncmp(identifier, "C", 2) == 0)
		index = 1;
	else
		return (map_info_error(tokens, UNKNOWN_IDENTIFIER), 1);
	if (data->colors[index])
		return (map_info_error(tokens, DUPLICATE_COLOR), 1);
	data->colors[index] = strdup(value);
	return (0);
}

int	process_line(char *line)
{
	if (strlen(line) == 0)
	{
		free(line);
		return (0);
	}
	if (parse_line(line))
	{
		free(line);
		return (1);
	}
	free(line);
	return (0);
}
