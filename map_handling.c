/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 03:44:30 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 03:45:36 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	add_line_to_map(char *line)
{
	t_cub	*data;
	char	*trimmed_line;

	data = get_cub_data();
	trimmed_line = trim_line(line);
	if (!trimmed_line)
		return (1);
	if (strlen(trimmed_line) == 0)
	{
		free(trimmed_line);
		return (0);
	}
	if (!data->map)
	{
		if (initialize_map(data, trimmed_line) == 1)
		{
			free(trimmed_line);
			return (1);
		}
	}
	else
	{
		if (append_line_to_map(data, trimmed_line) == 1)
		{
			free(trimmed_line);
			return (1);
		}
	}
	free(trimmed_line);
	return (0);
}

int	parse_map(int fd)
{
	char	*line;
	int		ret;
	t_cub	*data;

	data = get_cub_data();
	while (1)
	{
		ret = get_next_line(fd, &line);
		if (ret == -1)
			return (1);
		if (strlen(line) == 0 && !data->map)
		{
			free(line);
			continue ;
		}
		if (add_line_to_map(line) == 1)
		{
			free(line);
			return (1);
		}
		free(line);
		if (ret == 0)
			break ;
	}
	return (0);
}
