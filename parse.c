/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parse.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 20:49:13 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/03 23:26:14 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static int	is_boundary_cell(int i, int j, char **map)
{
	return (i == 0 || j == 0 || i >= ft_array_length(map) - 1
		|| j >= (int)strlen(map[i]) - 1);
}

static int	print_boundary_error(int i, int j)
{
	printf(BOUNDARY_CELL_NOT_ENCLOSED, i, j);
	return (1);
}

static int	has_adjacent_space(int i, int j, char **map)
{
	return ((i > 0 && j < (int)strlen(map[i - 1]) && map[i - 1][j] == ' ')
		||                        // Above
			(i + 1 < ft_array_length(map) && j < (int)strlen(map[i + 1])
				&& map[i + 1][j] == ' ') || // Below
			(j > 0 && map[i][j - 1] == ' ')
				||                                                       // Left
			(j + 1 < (int)strlen(map[i]) && map[i][j
				+ 1] == ' '));                                  // Right
}

static int	print_adjacent_space_error(int i, int j)
{
	printf(CELL_NOT_ENCLOSED, i, j);
	return (1);
}

static int	is_adjacent_to_incomplete_row(int i, int j, char **map)
{
	return ((i > 0 && j >= (int)strlen(map[i - 1])) ||                      
		// Current cell exceeds previous row length
			(i + 1 < ft_array_length(map) && j >= (int)strlen(map[i + 1])));
				// Current cell exceeds next row length
}

static int	print_incomplete_row_error(int i, int j)
{
	printf(ERR_ADJ_INCOMPLETE_ROW, i, j);
	return (1);
}

int	check_cell_enclosure(int i, int j)
{
	t_cub	*data;
	char	**map;

	data = get_cub_data();
	map = data->map;
	if (map[i][j] == '0' || (map[i][j] != '1' && map[i][j] != ' '))
	{
		if (is_boundary_cell(i, j, map))
			return (print_boundary_error(i, j));
		if (has_adjacent_space(i, j, map))
			return (print_adjacent_space_error(i, j));
		if (is_adjacent_to_incomplete_row(i, j, map))
			return (print_incomplete_row_error(i, j));
	}
	return (0);
}

void	print_str(char *s)
{
	while (*s)
		write(1, s++, 1);
}
void	map_info_error(char **tokens, char *s)
{
	print_str(s);
	print_str(tokens[0]);
	print_str("\n");
	free_array(&tokens);
}

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

int	is_whitespace(char c)
{
	return (c == ' ' || c == '\t' || c == '\n' || c == '\v' || c == '\f'
		|| c == '\r');
}

static char	*trim_line(const char *line)
{
	const char	*start = line;
	const char	*end;
	int			length;
	char		*trimmed;

	// while (*start && is_whitespace(*start))
	//     start++;
	// if (*start == '\0')
	//     return (strdup(""));
	end = start + strlen(start) - 1;
	while (end > start && is_whitespace(*end))
		end--;
	length = end - start + 1;
	trimmed = malloc(length + 1);
	if (!trimmed)
		return (NULL);
	strncpy(trimmed, start, length);
	trimmed[length] = '\0';
	return (trimmed);
}

static int	add_line_to_map(char *line)
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

int	parse_line(char *line)
{
	char	**tokens;
	t_cub	*data;

	tokens = ft_split(line, ' ');
	data = get_cub_data();
	if (!tokens || ft_array_length(tokens) != 2)
	{
		free_array(&tokens);
		return (1);
	}
	if (ft_strncmp(tokens[0], "NO", 3) == 0 || ft_strncmp(tokens[0], "SO",
			3) == 0 || ft_strncmp(tokens[0], "WE", 3) == 0
		|| ft_strncmp(tokens[0], "EA", 3) == 0)
	{
		if (parse_texture(data, tokens[0], tokens[1], tokens) == 1)
			return (1);
	}
	else if (ft_strncmp(tokens[0], "F", 2) == 0 || ft_strncmp(tokens[0], "C",
			2) == 0)
	{
		if (parse_color(data, tokens[0], tokens[1], tokens) == 1)
			return (1);
	}
	else
		return (map_info_error(tokens, UNKNOWN_IDENTIFIER), 1);
	free_array(&tokens);
	return (0);
}

static void	init_camera_plane(double direction_x, double direction_y,
		double pla_x, double pla_y)
{
	t_cub	*data;

	data = get_cub_data();
	data->var.direction_x = direction_x;
	data->var.direction_y = direction_y;
	data->var.plane_x = pla_x;
	data->var.plane_y = pla_y;
}

static void	init_player(int x, int y)
{
	t_cub	*data;

	data = get_cub_data();
	data->var.position_x = x + 0.5;
	data->var.position_y = y + 0.5;
	if (data->map[y][x] == NORTH)
		init_camera_plane(0, -1, 0.66, 0);
	else if (data->map[y][x] == SOUTH)
		init_camera_plane(0, 1, -0.66, 0);
	else if (data->map[y][x] == EAST)
		init_camera_plane(1, 0, 0, 0.66);
	else if (data->map[y][x] == WEST)
		init_camera_plane(-1, 0, 0, -0.66);
}

int	validate_characters(void)
{
	int		i;
	int		j;
	int		count;
	t_cub	*data;

	data = get_cub_data();
	i = 0;
	count = 0;
	while (data->map[i])
	{
		j = 0;
		while (data->map[i][j])
		{
			if (!strchr(VALID_MAP_SYMBOLS, data->map[i][j]))
				return (0);
			if (data->map[i][j] == NORTH || data->map[i][j] == SOUTH
				|| data->map[i][j] == EAST || data->map[i][j] == WEST)
			{
				init_player(j, i);
				count++;
			}
			j++;
		}
		i++;
	}
	return (count);
}

static int	check_trailing_map_lines(int index)
{
	int		i;
	t_cub	*data;

	data = get_cub_data();
	i = index;
	i++;
	while (data->map[i])
	{
		if (strlen(data->map[i]) > 0)
			return (1);
		i++;
	}
	data->map[i] = NULL;
	return (0);
}

static int	validate_trailing_lines(int i, t_cub *data)
{
	if (strlen(data->map[i]) == 0)
	{
		if (check_trailing_map_lines(i) == 1)
		{
			printf(TRAILING_EMPTY_LINES);
			return (1);
		}
	}
	return (0);
}

static int	validate_cells_and_players(t_cub *data)
{
	int	i;
	int	j;

	i = 0;
	while (data->map[i])
	{
		if (validate_trailing_lines(i, data) == 1)
			return (1);
		j = 0;
		while (data->map[i][j])
		{
			if (check_cell_enclosure(i, j) == 1)
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

int	validate_full_map(void)
{
	t_cub	*data;
	int		player_count;

	data = get_cub_data();
	player_count = validate_characters();
	if (player_count != 1)
	{
		printf(SINGLE_PLAYER_NEEDED, player_count);
		return (1);
	}
	if (ft_array_length(data->map) < 3)
	{
		printf(SMALL_MAP);
		return (1);
	}
	if (validate_cells_and_players(data) == 1)
		return (1);
	return (0);
}
