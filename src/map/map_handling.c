/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_handling.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 03:44:30 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/12 05:30:29 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	handle_empty_line(char *trimmed_line)
{
	if (ft_strlen(trimmed_line) == 0)
	{
		free(trimmed_line);
		return (0);
	}
	return (-1);
}

static int	handle_map_line(t_cub *data, char *trimmed_line)
{
	int	result;

	if (!data->map)
		result = initialize_map(data, trimmed_line);
	else
		result = append_line_to_map(data, trimmed_line);
	free(trimmed_line);
	return (result);
}

int	add_line_to_map(char *line)
{
	t_cub	*data;
	char	*trimmed_line;
	int		status;

	data = get_cub_data();
	trimmed_line = trim_line(line);
	if (!trimmed_line)
		return (1);
	status = handle_empty_line(trimmed_line);
	if (status != -1)
		return (status);
	return (handle_map_line(data, trimmed_line));
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
		if (ft_strlen(line) == 0 && !data->map)
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
