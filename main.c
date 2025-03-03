/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:48:57 by abouguri          #+#    #+#             */
/*   Updated: 2025/01/25 21:29:58 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// helpful funcs

int ft_array_length(char **array)
{
    int i = 0;

    if (!array)
        return (0);
    while (array[i])
        i++;
    return (i);
}

int ft_max_length(char **array)
{
    int result = -1;
    int tmp;
    int i = 0;

    if (!array)
        return (0);
    while (array[i])
    {
        tmp = strlen(array[i]);
        if (tmp > result)
            result = tmp;    
        i++;
    }
    return (result);
}
static int	count_words(const char *str, char c)
{
	int	i;
	int	trigger;

	i = 0;
	trigger = 0;
	while (*str)
	{
		if (*str != c && trigger == 0)
		{
			trigger = 1;
			i++;
		}
		else if (*str == c)
			trigger = 0;
		str++;
	}
	return (i);
}

static char	*word_dup(const char *str, int start, int finish)
{
	char	*word;
	int		i;

	i = 0;
	word = malloc((finish - start + 1) * sizeof(char));
	while (start < finish)
		word[i++] = str[start++];
	word[i] = '\0';
	return (word);
}

char	**ft_split(char const *s, char c)
{
	size_t	i;
	size_t	j;
	int		index;
	char	**split;

	split = malloc((count_words(s, c) + 1) * sizeof(char *));
	if (!s || !split)
		return (NULL);
	i = 0;
	j = 0;
	index = -1;
	while (i <= strlen(s))
	{
		if (s[i] != c && index < 0)
			index = i;
		else if ((s[i] == c || i == strlen(s)) && index >= 0)
		{
			split[j++] = word_dup(s, index, i);
			index = -1;
		}
		i++;
	}
	split[j] = 0;
	return (split);
}

// utilities

void error_exit(const char *message)
{
    write(2, message, strlen(message));
    exit(EXIT_FAILURE);
}

void error_exit_fd_cleanup(const char *message, int fd)
{
    if (fd >= 0) close(fd);
    t_cub *data = get_cub_data();
    free_array(&data->textures);
    free_array(&data->colors);
    free_array(&data->map);
    error_exit(message);
}

void error_exit_cleanup(const char *message)
{
    t_cub *data = get_cub_data();
    free_array(&data->textures);
    free_array(&data->colors);
    free_array(&data->map);
    error_exit(message);
}

t_cub *get_cub_data(void)
{
    static t_cub data;
    
    return &data;
}

void free_array(char ***array)
{
    if (!*array)
        return;
    int i = 0;
    while ((*array)[i])
    {
        free((*array)[i]);
        i++;
    }
    free(*array);
    *array = NULL;
}

int validate_file_extension(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    return dot && strcmp(dot + 1, "cub") == 0;
}

// get next line

int append_to_buffer(char **buffer, char *new_content, char **temp)
{
    int i = 0;

    while ((*buffer)[i])
    {
        new_content[i] = (*buffer)[i];
        i++;
    }
    i = 0;
    while ((*temp)[i])
    {
        new_content[i + strlen(*buffer)] = (*temp)[i];
        i++;
    }
    new_content[i + strlen(*buffer)] = '\0';
    free(*temp);
    free(*buffer);
    *buffer = new_content;
    return 1;
}

int read_and_append_to_buffer(char **buffer, int fd, int *bytes_read)
{
    char *temp;
    char *combined_content;

    temp = malloc(BUFFER_SIZE + 1);
    if (!temp)
        return -1;

    *bytes_read = read(fd, temp, BUFFER_SIZE);
    if (*bytes_read < 0)
    {
        free(temp);
        return -1;
    }
    temp[*bytes_read] = '\0';

    combined_content = malloc(strlen(*buffer) + strlen(temp) + 1);
    if (!combined_content)
    {
        free(temp);
        return -1;
    }

    return append_to_buffer(buffer, combined_content, &temp);
}

static char *create_line(const char *buffer, int length)
{
    char *line = malloc(length + 1);
    if (!line)
        return NULL;

    strncpy(line, buffer, length);
    line[length] = '\0';
    return line;
}

static char *get_remaining_content(const char *buffer, int index)
{
    char *remaining = strdup(&buffer[index + 1]);
    return remaining;
}

static void clear_buffer(char **buffer)
{
    free(*buffer);
    *buffer = NULL;
}

int extract_line(char **buffer, char **line)
{
    int i = 0;

    while ((*buffer)[i] != '\n' && (*buffer)[i] != '\0')
        i++;
    *line = create_line(*buffer, i);
    if (!*line)
        return -1;

    if ((*buffer)[i] == '\0') // No remaining content
    {
        clear_buffer(buffer); // Free and NULL the buffer
    }
    else
    {
        char *remaining = get_remaining_content(*buffer, i);
        if (!remaining)
        {
            free(*line); // Free the allocated line in case of failure
            *line = NULL;
            return -1;
        }

        clear_buffer(buffer); // Free old buffer
        *buffer = remaining;  // Set remaining buffer
    }
    return 1;
}

int contains_newline(char *buffer)
{
    int i = 0;

    while (buffer[i] && buffer[i] != '\n')
        i++;

    return buffer[i] == '\n';
}

static int validate_inputs(int fd, char **line)
{
    if (fd < 0 || BUFFER_SIZE <= 0 || fd >= FOPEN_MAX || !line)
        return (-1);
    return (0);
}
static int initialize_buffer(char **buffer)
{
    if (!*buffer)
    {
        *buffer = malloc(1);
        if (!*buffer)
            return (-1);
        (*buffer)[0] = '\0';
    }
    return (0);
}

static int read_to_buffer(char **buffer, int fd, int *bytes_read)
{
    while (!contains_newline(*buffer) && *bytes_read == BUFFER_SIZE)
    {
        if (read_and_append_to_buffer(buffer, fd, bytes_read) == -1)
            return (-1);
    }
    return (0);
}

int get_next_line(int fd, char **line)
{
    static char *buffers[FOPEN_MAX];
    int bytes_read = BUFFER_SIZE;

    if (validate_inputs(fd, line) == -1)
        return (-1);
    if (initialize_buffer(&buffers[fd]) == -1)
        return (-1);
    if (read_to_buffer(&buffers[fd], fd, &bytes_read) == -1)
        return (-1);
    if (contains_newline(buffers[fd]))
        return (extract_line(&buffers[fd], line));
    else
    {
        *line = strdup(buffers[fd]);
        if (!*line)
            return (-1);
        free(buffers[fd]);
        buffers[fd] = NULL;
        return (0);
    }
}

// int get_next_line(int fd, char **line)
// {
//     int bytes_read;
//     static char *buffers[FOPEN_MAX];

//     if (fd < 0 || BUFFER_SIZE <= 0 || fd >= FOPEN_MAX || !line)
//         return (-1);
//     if (!buffers[fd])
//     {
//         buffers[fd] = malloc(1);
//         if (!buffers[fd])
//             return (-1);
//         buffers[fd][0] = '\0';
//     }
//     bytes_read = BUFFER_SIZE;
//     while (!contains_newline(buffers[fd]) && bytes_read == BUFFER_SIZE)
//     {
//         if (read_and_append_to_buffer(&buffers[fd], fd, &bytes_read) == -1)
//             return (-1);
//     }
//     if (contains_newline(buffers[fd]))
//     {
//         int ret = extract_line(&buffers[fd], line);
//         return (ret);
//     }
//     else
//     {
//         *line = strdup(buffers[fd]);
//         if (!*line)
//             return (-1);
//         free(buffers[fd]);
//         buffers[fd] = NULL;
//         return (0);
//     }
// }



// parser

int initialize_resources(t_cub *data)
{
    data->textures = calloc(TEXTURE_COUNT + 1, sizeof(char *));
    data->colors = calloc(RGB_COUNT + 1, sizeof(char *));
    if (!data->textures || !data->colors)
        return (1);
    return (0);
}

int process_line(char *line)
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

int parse_file(int fd)
{
    char *line = NULL;
    int ret;
    t_cub *data = get_cub_data();

    if (initialize_resources(data))
        return (1);
    while (ft_array_length(data->textures) != TEXTURE_COUNT || ft_array_length(data->colors) != RGB_COUNT)
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
            break;
    }
    return (0);
}

int	ft_strncmp(const char *s1, char *s2, size_t n)
{
	while (*s1 != '\0' && *s1 == *s2 && n > 0)
	{
		s1++;
		s2++;
		n--;
	}
	if (n == 0)
		return (0);
	return (*(unsigned char *)s1 - *(unsigned char *)s2);
}

// int parse_textures(char *line)
// {
//     char **tokens = ft_split(line, ' ');
//     t_cub *data = get_cub_data();
    
    
//     if (!tokens)
//         return 1;
//     if (ft_array_length(tokens) != 2)
//         return 1;
//     if (ft_strncmp(tokens[0], "NO", 3) == 0)
//     {
//         data->textures[0] = strdup(tokens[1]);
//     }
//     else if (ft_strncmp(tokens[0], "SO", 3) == 0)
//         data->textures[1] = strdup(tokens[1]);
//     else if (ft_strncmp(tokens[0], "WE", 3) == 0)
//         data->textures[2] = strdup(tokens[1]);
//     else if (ft_strncmp(tokens[0], "EA", 3) == 0)
//         data->textures[3] = strdup(tokens[1]);
//     else if (ft_strncmp(tokens[0], "C", 2) && ft_strncmp(tokens[0], "F", 2))
//         return (1);
//     free_array(&tokens);
//     return 0;
// }

// int parse_colors(char *line)
// {
//     char **tokens = ft_split(line, ' ');
//     t_cub *data = get_cub_data();
//     if (!tokens || ft_array_length(tokens) != 2)
//     {
//         free_array(&tokens);
//         return 1;
//     }
//     if (strncmp(tokens[0], "F", 2) == 0)
//         data->colors[0] = strdup(tokens[1]);
//     else if (strncmp(tokens[0], "C", 2) == 0)
//         data->colors[1] = strdup(tokens[1]);
//     free_array(&tokens);
//     return 0;
// }

void    print_str(char *s)
{
    while(*s)
        write(1, s++, 1);
}
void    map_info_error(char **tokens, char *s)
{
    print_str(s);
    print_str(tokens[0]);
    print_str("\n");
    free_array (&tokens);
}

int parse_texture(t_cub *data, char *identifier, char *path, char **tokens)
{
    int index;

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

int parse_color(t_cub *data, char *identifier, char *value, char **tokens)
{
    int index;

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

int parse_line(char *line)
{
    char **tokens = ft_split(line, ' ');
    t_cub *data = get_cub_data();

    if (!tokens || ft_array_length(tokens) != 2)
    {
        free_array(&tokens);
        return (1);
    }

    if (ft_strncmp(tokens[0], "NO", 3) == 0 || ft_strncmp(tokens[0], "SO", 3) == 0 ||
        ft_strncmp(tokens[0], "WE", 3) == 0 || ft_strncmp(tokens[0], "EA", 3) == 0)
    {
        if (parse_texture(data, tokens[0], tokens[1], tokens) == 1)
            return (1);
    }
    else if (ft_strncmp(tokens[0], "F", 2) == 0 || ft_strncmp(tokens[0], "C", 2) == 0)
    {
        if (parse_color(data, tokens[0], tokens[1], tokens) == 1)
            return (1);
    }
    else
        return (map_info_error(tokens, UNKNOWN_IDENTIFIER), 1);

    free_array(&tokens);
    return (0);
}




char **reallocate_map_memory(char **pointer, int size)
{
    int i = 0;
    char **new_pointer;

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

int initialize_map(t_cub *data, char *line)
{
    data->map = malloc(sizeof(char *) * 2);
    if (!data->map)
        return (1);

    data->map[0] = strdup(line);
    data->map[1] = NULL;

    return (0);
}

int append_line_to_map(t_cub *data, char *line)
{
    char **tmp;
    int current_size = ft_array_length(data->map);

    tmp = reallocate_map_memory(data->map, current_size + 2);
    if (!tmp)
        return (1);

    data->map = tmp;
    data->map[current_size] = strdup(line);
    data->map[current_size + 1] = NULL;

    return (0);
}

int is_whitespace(char c)
{
    return (c == ' ' || c == '\t' || c == '\n' ||
            c == '\v' || c == '\f' || c == '\r');
}

static char *trim_line(const char *line)
{
    const char *start = line;
    const char *end;

    // while (*start && is_whitespace(*start))
    //     start++;
    // if (*start == '\0')
    //     return (strdup(""));
    end = start + strlen(start) - 1;
    while (end > start && is_whitespace(*end))
        end--;
    int length = end - start + 1;
    char *trimmed = malloc(length + 1);
    if (!trimmed)
        return (NULL);
    strncpy(trimmed, start, length);
    trimmed[length] = '\0';
    return (trimmed);
}


static int add_line_to_map(char *line)
{
    t_cub *data = get_cub_data();
    char *trimmed_line = trim_line(line);

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


int parse_map(int fd)
{
    char *line;
    int ret;
    t_cub *data = get_cub_data();

    while (1)
    {
        ret = get_next_line(fd, &line);
        if (ret == -1)
            return (1);

        if (strlen(line) == 0 && !data->map)
        {
            free(line);
            continue;
        }

        if (add_line_to_map(line) == 1)
        {
            free(line);
            return (1);
        }

        free(line);
        if (ret == 0)
            break;
    }

    return (0);
}

static int is_boundary_cell(int i, int j, char **map)
{
    return (i == 0 || j == 0 || 
            i >= ft_array_length(map) - 1 || 
            j >= (int)strlen(map[i]) - 1);
}

static int print_boundary_error(int i, int j)
{
    printf(BOUNDARY_CELL_NOT_ENCLOSED, i, j);
    return (1);
}

static int has_adjacent_space(int i, int j, char **map)
{
    return ((i > 0 && j < (int)strlen(map[i - 1]) && map[i - 1][j] == ' ') ||    // Above
            (i + 1 < ft_array_length(map) && j < (int)strlen(map[i + 1]) && map[i + 1][j] == ' ') || // Below
            (j > 0 && map[i][j - 1] == ' ') || // Left
            (j + 1 < (int)strlen(map[i]) && map[i][j + 1] == ' ')); // Right
}

static int print_adjacent_space_error(int i, int j)
{
    printf(CELL_NOT_ENCLOSED, i, j);
    return (1);
}

static int is_adjacent_to_incomplete_row(int i, int j, char **map)
{
    return ((i > 0 && j >= (int)strlen(map[i - 1])) || // Current cell exceeds previous row length
            (i + 1 < ft_array_length(map) && j >= (int)strlen(map[i + 1]))); // Current cell exceeds next row length
}

static int print_incomplete_row_error(int i, int j)
{
    printf(ERR_ADJ_INCOMPLETE_ROW, i, j);
    return (1);
}


int check_cell_enclosure(int i, int j)
{
    t_cub *data = get_cub_data();
    char **map = data->map;

    if (map[i][j] == '0' || (map[i][j] != '1' && map[i][j] != ' '))
    {
        if (is_boundary_cell(i, j, map))
            return print_boundary_error(i, j);

        if (has_adjacent_space(i, j, map))
            return print_adjacent_space_error(i, j);

        if (is_adjacent_to_incomplete_row(i, j, map))
            return print_incomplete_row_error(i, j);
    }
    return (0);
}

static int	check_trailing_map_lines(int index)
{
	int		i;

    t_cub *data = get_cub_data();
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

static void	init_camera_plane(double direction_x, double direction_y, double pla_x, double pla_y)
{
    t_cub *data = get_cub_data();

	data->var.direction_x = direction_x;
	data->var.direction_y = direction_y;
	data->var.plane_x = pla_x;
	data->var.plane_y = pla_y;
}

static void	init_player(int x, int y)
{
    t_cub *data = get_cub_data();

	data->var.position_x = x * CELL_SIZE + CELL_SIZE / 2;
	data->var.position_y = y * CELL_SIZE + CELL_SIZE / 2;
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
	int	i;
	int	j;
	int	count;
    t_cub *data = get_cub_data();

	i = 0;
	count = 0;
	while (data->map[i])
	{
		j = 0;
		while (data->map[i][j])
		{
			if (!strchr(VALID_MAP_SYMBOLS, data->map[i][j]))
				return (0);
			if (data->map[i][j] == NORTH || data->map[i][j] == SOUTH || data->map[i][j] == EAST || data->map[i][j] == WEST)
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

static int validate_trailing_lines(int i, t_cub *data)
{
    if (strlen(data->map[i]) == 0)
    {
        if (check_trailing_map_lines(i) == 1)
        {
            printf(TRAILING_EMPTY_LINES);
            return 1;
        }
    }
    return (0);
}

static int validate_cells_and_players(t_cub *data)
{
    int i = 0;
    int j;

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

int validate_full_map(void)
{
    t_cub *data = get_cub_data();
    int player_count = validate_characters();

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

int parse(char *file)
{
    if (!validate_file_extension(file))
        error_exit(ERR_INVALID_EXT);

    int fd = open(file, O_RDONLY);
    if (fd < 0)
        error_exit(ERR_FILE_OPEN);

    if (parse_file(fd) == 1)
        error_exit_fd_cleanup(ERR_INVALID_INFO, fd);
    if (parse_map(fd) == 1)
        error_exit_fd_cleanup(ERR_INVALID_MAP, fd);
    
    if (validate_full_map() == 1)
        error_exit_cleanup(ERR_INVALID_MAP);
    
    close(fd);
    return 0;
}



void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}

int	exitbyx(int keycode, t_data *data)
{
    (void)data;
    printf("keycode |%d|\n", keycode);
    if (keycode == 17)
        {
            printf("laaah\n");
            return 1;
            // exit(1);
        }
	return (0);
}

int on_destroy(void*data)
{
    printf("ded\n");
	(void)data;
    // mlx_loop_end(((t_cub*)data)->mlx);
    return 0;
}
static void init_window(t_cub *data, t_data *img, int win_width, int win_height)
{
    data->mlx = mlx_init();
    if (!data->mlx)
        error_exit_cleanup(ERR_INIT_MLX);

    data->win = mlx_new_window(data->mlx, win_width, win_height, "cub3D");
    if (!data->win)
        error_exit_cleanup(ERR_CREATE_WINDOW);

    img->img = mlx_new_image(data->mlx, win_width, win_height);
    if (!img->img)
        error_exit_cleanup(ERR_CREATE_IMAGE);

    img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, &img->line_length, &img->endian);
    if (!img->addr)
        error_exit_cleanup(ERR_GET_IMAGE_ADDR);
}


static int get_cell_color(char cell)
{
    if (cell == '0' || cell == 'N' || cell == 'E' || cell == 'W' || cell == 'S') // Floor or player
        return (WHITE); // White
    if (cell == '1') // Wall
        return (BLUE); // Blue
    return (0); // Default color for invalid cells
}

static void draw_grid(t_cub *data, int map_width, int map_height, int cell_size)
{
    int x;
    int y;
    
    x = 0;
    // Vertical
    while (x <= map_width)
    {
        y = 0;
        while (y < map_height * cell_size)
        {
            my_mlx_pixel_put(&data->img, x * cell_size, y, BLACK);
            y++;
        }
        x++;
    }
    // Horizontal
    y = 0;
    while (y <= map_height)
    {
        x = 0;
        while (x < map_width * cell_size)
        {
            my_mlx_pixel_put(&data->img, x, y * cell_size, BLACK);
            x++;
        }
        y++;
    }
}

static void draw_circle(t_cub *data, int center_x, int center_y, int radius, int color)
{
    int y = -radius;

    while (y <= radius)
    {
        int x = -radius;
        while (x <= radius)
        {
            if (x * x + y * y <= radius * radius) // Check if the pixel is within the circle
            {
                my_mlx_pixel_put(&data->img, center_x + x, center_y + y, color);
            }
            x++;
        }
        y++;
    }
}

static void draw_cell(t_cub *data, int x, int y, int cell_size, int color)
{
    int p_x = 0;

    while (p_x < cell_size)
    {
        int p_y = 0;
        while (p_y < cell_size)
        {
            int pixel_x = p_x + x * cell_size;
            int pixel_y = p_y + y * cell_size;
            my_mlx_pixel_put(&data->img, pixel_x, pixel_y, color);
            p_y++;
        }
        p_x++;
    }
}

static void draw_player(t_cub *data, int cell_size)
{
    // draw_cell(data, cell_size, WHITE);

    int center_x = data->var.position_x;
    int center_y = data->var.position_y;
    draw_circle(data, center_x, center_y, cell_size / 4, GREEN);
}

static void render_map_cells(t_cub *data, char **map, int cell_size)
{
    int y = 0;

    while (y < ft_array_length(map))
    {
        int x = 0;
        while (x < (int)strlen(map[y]))
        {
            int color = get_cell_color(map[y][x]);

            draw_cell(data, x, y, cell_size, color);

            // if (map[y][x] == 'N' || map[y][x] == 'S' || map[y][x] == 'E' || map[y][x] == 'W')
            // {
            //     draw_player(img, x, y, cell_size);
            // }
            x++;
        }
        y++;
    }
	draw_player(data, cell_size);
}

static void render_map_to_image(t_cub *data, char **map, int cell_size)
{
    int map_width = ft_max_length(map);
    int map_height = ft_array_length(map);

    // cells and player
    render_map_cells(data, map, cell_size);

    // Grid
    draw_grid(data, map_width, map_height, cell_size);
}

// Function to move the player
void 	move_player(t_var *player, int keycode, t_cub *data) 
{
	printf("KEY : %d\n",keycode);
	printf("MAP: %s\n", data->map[0]);
	printf("fhjrifherlhf %f %f\n", player->position_x, player->position_y);
    if (keycode == KEY_UP) {
        player->position_y -= 1; // move up
    }
    else if (keycode == KEY_DOWN) {
        player->position_y += 1; // move down
    }
    else if (keycode == KEY_LEFT) {
        player->position_x -= 1; // move left
    }
    else if (keycode == KEY_RIGHT) {
        player->position_x += 1; // move right
    }
}

// Key event handler for movement
int 	handle_key_press(int keycode, t_cub *data)
{
	printf("HANDLE KEY PRESS\n");
	move_player(&data->var, keycode, data);
	render_map_to_image(data, data->map, CELL_SIZE);
	mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);
	return keycode;
}


void init(void)
{
    t_cub *data = get_cub_data();


    int win_height = ft_array_length(data->map) * CELL_SIZE;
    int win_width = ft_max_length(data->map) * CELL_SIZE;

    init_window(data, &data->img, win_width, win_height);
    render_map_to_image(data, data->map, CELL_SIZE);
    mlx_put_image_to_window(data->mlx, data->win, data->img.img, 0, 0);

	//move event
	printf("ANA HNA\n");
	mlx_hook(data->win, 2, (1L<<0), handle_key_press, data);

    // mlx_hook(data->win, 17, 0L, on_destroy, data);
    mlx_loop(data->mlx);
}


// void	init(void)
// {
// 	void	*temporary;
//     t_cub *data = get_cub_data();
//     t_data img;
    
// 	data->mlx = mlx_init(); 
// 	// if (init_textures() == 1)
// 	// 	error_exit_cleanup(ERR_TEXTURE_LOAD);
// 	// if (init_colors() == 1)
// 	// 	error_exit_cleanup(ERR_COLORS_LOAD);
//     int win_height = ft_array_length(data->map) * CELL_SIZE; 
//     int win_width = ft_max_length(data->map) * CELL_SIZE; 
// 	temporary = mlx_new_window(data->mlx, win_width, win_height, "cub3D");
//     img.img = mlx_new_image(data->mlx, win_width, win_height);
//     img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
//     for (int y = 0; y < ft_array_length(data->map); y++)
//     {
//         for (int x = 0; x < (int)strlen(data->map[y]); x++)
//         {
//             int color = 0;
//             if (data->map[y][x] == '0')
//                 color = 0xFFFFFFFF;
//             else if (data->map[y][x] == 'N' || data->map[y][x] == 'W' || data->map[y][x] == 'S' || data->map[y][x] == 'E')
//                 color = 0xFF0000FF;
//             for (int p_x = 0; p_x < CELL_SIZE; p_x++)
//             {
//                 for (int p_y = 0; p_y < CELL_SIZE; p_y++)
//                 {
//                     int pixel_x = p_x + x * CELL_SIZE;
//                     int pixel_y = p_y + y * CELL_SIZE;
//                     my_mlx_pixel_put(&img, pixel_x,  pixel_y,  color);
//                 }
//             }
//         }
//     }
    
//     // my_mlx_pixel_put(&img, 5, 5, 0x00CC66FF);
//     mlx_put_image_to_window(data->mlx, temporary, img.img, 0, 0);
//     // mlx_key_hook(temporary, exitbyx, &data);
//     //mlx_hook(temporary, 3, 1L<<1, exitbyx, &data);
//     mlx_hook(temporary, 17, 0L, on_destroy, &data);
//     mlx_loop(data->mlx);    
// 	// data->win = temporary;
// }
//added



int main(int argc, char **argv)
{
    if (argc < 2)
        error_exit(ERR_TOO_FEW_ARGS);
    if (argc > 2)
        error_exit(ERR_TOO_MANY_ARGS);
    parse(argv[1]);
    for (int i = 0; get_cub_data()->textures[i]; i++)
        printf("textures: %s\n",get_cub_data()->textures[i]);
    for (int i = 0; get_cub_data()->colors[i]; i++)
        printf("colors: %s\n",get_cub_data()->colors[i]);
    for (int i = 0; get_cub_data()->map[i]; i++)
        printf("map: %s\n",get_cub_data()->map[i]);
    init();
    
    return EXIT_SUCCESS; 
}
