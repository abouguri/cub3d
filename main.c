/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:48:57 by abouguri          #+#    #+#             */
/*   Updated: 2025/01/22 14:46:08 by abouguri         ###   ########.fr       */
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

int extract_line(char **buffer, char **line)
{
    int i = 0;
    char *remaining_content;

    while ((*buffer)[i] != '\n' && (*buffer)[i] != '\0')
        i++;

    *line = malloc(i + 1);
    if (!*line)
        return -1;

    strncpy(*line, *buffer, i);
    (*line)[i] = '\0';

    if ((*buffer)[i] == '\0')
    {
        free(*buffer);
        *buffer = NULL;
    }
    else
    {
        remaining_content = strdup(&((*buffer)[i + 1]));
        if (!remaining_content)
        {
            free(*line);
            *line = NULL; // Reset pointer to avoid dangling reference
            return -1;
        }

        free(*buffer);
        *buffer = remaining_content;
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

int get_next_line(int fd, char **line)
{
    int bytes_read;
    static char *buffers[FOPEN_MAX];

    if (fd < 0 || BUFFER_SIZE <= 0 || fd >= FOPEN_MAX || !line)
        return -1;

    if (!buffers[fd])
    {
        buffers[fd] = malloc(1);
        if (!buffers[fd])
            return -1;
        buffers[fd][0] = '\0';
    }

    bytes_read = BUFFER_SIZE;
    while (!contains_newline(buffers[fd]) && bytes_read == BUFFER_SIZE)
    {
        if (read_and_append_to_buffer(&buffers[fd], fd, &bytes_read) == -1)
            return -1;
    }

    if (contains_newline(buffers[fd]))
    {
        int ret = extract_line(&buffers[fd], line);
        return ret;
    }
    else
    {
        *line = strdup(buffers[fd]);
        if (!*line)
            return -1;
        free(buffers[fd]);
        buffers[fd] = NULL;
        return 0;
    }
}



// parser


int parse_file(int fd)
{
    char *line = NULL;
    int ret;

    t_cub *data = get_cub_data();

    data->textures = calloc(TEXTURE_COUNT + 1, sizeof(char *));
    data->colors = calloc(RGB_COUNT + 1, sizeof(char *));
    if (!data->textures || !data->colors)
        return 1;

    while (ft_array_length(data->textures) != TEXTURE_COUNT || ft_array_length(data->colors) != RGB_COUNT)
    {
        ret = get_next_line(fd, &line);

        if (ret == -1)
        {
            free(line);
            line = NULL; // Reset pointer after freeing
            return 1;
        }

        if (strlen(line) == 0)
        {
            free(line);
            line = NULL; // Reset pointer after freeing
            continue;
        }

        int textures_result = parse_textures(line);
        int colors_result = parse_colors(line);

        if (textures_result == 1 && colors_result)
        {
            free(line);
            line = NULL; // Reset pointer after freeing
            return 1;
        }

        free(line);
        line = NULL; // Reset pointer after freeing

        if (ret == 0)
            break;
    }

    if (line)
    {
        free(line);
        line = NULL; // Reset pointer after freeing
    }
    return 0;
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

int parse_textures(char *line)
{
    char **tokens = ft_split(line, ' ');
    t_cub *data = get_cub_data();
    
    
    if (!tokens)
        return 1;
    if (ft_array_length(tokens) != 2)
        return 1;
    if (ft_strncmp(tokens[0], "NO", 3) == 0)
    {
        data->textures[0] = strdup(tokens[1]);
    }
    else if (ft_strncmp(tokens[0], "SO", 3) == 0)
        data->textures[1] = strdup(tokens[1]);
    else if (ft_strncmp(tokens[0], "WE", 3) == 0)
        data->textures[2] = strdup(tokens[1]);
    else if (ft_strncmp(tokens[0], "EA", 3) == 0)
        data->textures[3] = strdup(tokens[1]);
    else if (ft_strncmp(tokens[0], "C", 2) && ft_strncmp(tokens[0], "F", 2))
        return (1);
    free_array(&tokens);
    return 0;
}

int parse_colors(char *line)
{
    char **tokens = ft_split(line, ' ');
    t_cub *data = get_cub_data();
    if (!tokens || ft_array_length(tokens) != 2)
    {
        free_array(&tokens);
        return 1;
    }
    if (strncmp(tokens[0], "F", 2) == 0)
        data->colors[0] = strdup(tokens[1]);
    else if (strncmp(tokens[0], "C", 2) == 0)
        data->colors[1] = strdup(tokens[1]);
    free_array(&tokens);
    return 0;
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




static int add_line_to_map(char *line)
{
    char **tmp;
    int current_size;
    t_cub *data = get_cub_data();

    if (strlen(line) == 0)
        return 0;

    if (!data->map)
    {
        data->map = malloc(sizeof(char *) * 2);
        if (!data->map)
            return (1);
        data->map[0] = strdup(line);
        data->map[1] = NULL;
    }
    else
    {
        current_size = ft_array_length(data->map);

        tmp = reallocate_map_memory(data->map, current_size + 2);
        if (!tmp)
            return (1);
        data->map = tmp;

        data->map[current_size] = strdup(line);
        data->map[current_size + 1] = NULL; 
    }

    // Print current map state
    //printf("Current map state:\n");
    //for (int i = 0; data->map[i]; i++)
        //printf("Map[%d]: %s\n", i, data->map[i]);

    return (0);
}



int	parse_map(int fd)
{
	char	*line;
	int		ret;

    t_cub *data = get_cub_data();
	while (1)
	{
		ret = get_next_line(fd, &line);
		if (ret == -1)
			return (1);
		if (strlen(line) == 0 && !data->map)
            ;
		else if (add_line_to_map(line) == 1)
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

static int	check_cell_enclosure(int i, int j)
{
    t_cub *data = get_cub_data();

	if (data->map[i][j] == '0' || (data->map[i][j] != '1' && data->map[i][j] != ' '))
	{
		if (i == 0 || !data->map[i + 1] || j == 0 || j >= (int)strlen(data->map[i]) || j >= (int)strlen(data->map[i + 1]))
            return (1);
        if (i > 0 && j < (int)strlen(data->map[i - 1]) && data->map[i - 1][j] == ' ')
            return (1);
		if (data->map[i + 1] && j < (int)strlen(data->map[i + 1]) && data->map[i + 1][j] == ' ')
            return (1);
		if (j > 0 && data->map[i][j - 1] == ' ')
            return (1);
		if (j + 1 < (int)strlen(data->map[i]) && data->map[i][j + 1] == ' ')
            return (1);
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

	data->var.position_x = x + 0.5;
	data->var.position_y = y + 0.5;
	if (data->map[y][x] == 'N')
		init_camera_plane(0, -1, 0.66, 0);
	else if (data->map[y][x] == 'S')
		init_camera_plane(0, 1, -0.66, 0);
	else if (data->map[y][x] == 'E')
		init_camera_plane(1, 0, 0, 0.66);
	else if (data->map[y][x] == 'W')
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
			if (!strchr(" 10NSEW", data->map[i][j]))
				return (0);
			if (data->map[i][j] == 'N' || data->map[i][j] == 'S' || data->map[i][j] == 'E' || data->map[i][j] == 'W')
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

int	validate_full_map(void)
{
	int	i;
	int	j;
    int player_count = 0;
    
    t_cub *data = get_cub_data();
	i = 0;
	if (ft_array_length(data->map) < 3 || validate_characters() != 1)
		return (1);
	while (data->map[i])
	{
		if (strlen(data->map[i]) == 0)
		{
			if (check_trailing_map_lines(i) == 1)
				return (1);
			break ;
		}
		j = 0;
		while (data->map[i][j])
		{
			if (check_cell_enclosure(i, j) == 1)
				return (1);
            if (data->map[i][j] == 'S' || data->map[i][j] == 'N' || data->map[i][j] == 'W' || data->map[i][j] == 'E')
                {
                    player_count++;
                }
        	j++;
		}
		i++;
	}
    if (player_count != 1)
    {
        return 1;
    }
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

typedef struct	s_data {
	void	*img;
	char	*addr;
	int		bits_per_pixel;
	int		line_length;
	int		endian;
}				t_data;

void	my_mlx_pixel_put(t_data *data, int x, int y, int color)
{
	char	*dst;

	dst = data->addr + (y * data->line_length + x * (data->bits_per_pixel / 8));
	*(unsigned int*)dst = color;
}
void draw_circle(t_data *img, int xc, int yc, int r, int color)
{
    for (int x = xc - r; x <= xc + r; x++)
    {
        for (int y = yc - r; y <= yc + r; y++)
        {
            if ((x - xc) * (x - xc) + (y - yc) * (y - yc) <= r * r)
            {
                my_mlx_pixel_put(img, x, y, color);
            }
        }
    }
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
void	init(void)
{
	void	*temporary;
    t_cub *data = get_cub_data();
    t_data img;
    
	data->mlx = mlx_init(); 
	// if (init_textures() == 1)
	// 	error_exit_cleanup(ERR_TEXTURE_LOAD);
	// if (init_colors() == 1)
	// 	error_exit_cleanup(ERR_COLORS_LOAD);
	temporary = mlx_new_window(data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D");
    img.img = mlx_new_image(data->mlx, 1920, 1080);
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length, &img.endian);
    draw_circle(&img, 250, 250, 100, 0x0000FF00); // Draw a green circle
    // my_mlx_pixel_put(&img, 5, 5, 0x00CC66FF);
    mlx_put_image_to_window(data->mlx, temporary, img.img, 0, 0);
    // mlx_key_hook(temporary, exitbyx, &data);
    mlx_hook(temporary, 3, 1L<<1, exitbyx, &data);
    mlx_loop(data->mlx);    
	// data->win = temporary;
}
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
