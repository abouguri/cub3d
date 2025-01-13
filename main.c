/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: rukia <rukia@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:48:57 by abouguri          #+#    #+#             */
/*   Updated: 2025/01/12 18:30:43 by rukia            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

// helpful funcs

int	ft_array_length(char **array)
{
	int	i;

	i = 0;
	while (array[i])
		i++;
	return (i);
}

static size_t	ft_countword(char const *s, char c)
{
	size_t	count;

	if (!*s)
		return (0);
	count = 0;
	while (*s)
	{
		while (*s == c)
			s++;
		if (*s)
			count++;
		while (*s != c && *s)
			s++;
	}
	return (count);
}

char	*ft_substr(const char *s, unsigned int start, size_t len)
{
	char	*ret;

	if (!s)
		return (0);
	if (strlen(s) < start)
		len = 0;
	if (strlen(s + start) < len)
		len = strlen(s + start);
	ret = malloc(sizeof(char) * (len + 1));
	if (!ret)
		return (0);
	strlcpy(ret, s + start, len + 1);
	return (ret);
}

char	**ft_split(char const *s, char c)
{
	char	**lst;
	size_t	word_len;
	int		i;

	lst = (char **)malloc((ft_countword(s, c) + 1) * sizeof(char *));
	if (!s || !lst)
		return (0);
	i = 0;
	while (*s)
	{
		while (*s == c && *s)
			s++;
		if (*s)
		{
			if (!strchr(s, c))
				word_len = strlen(s);
			else
				word_len = strchr(s, c) - s;
			lst[i++] = ft_substr(s, 0, word_len);
			s += word_len;
		}
	}
	lst[i] = NULL;
	return (lst);
}

// utilities

void error_exit(const char *message)
{
    write(2, message, strlen(message));
    exit(EXIT_FAILURE);
}

void error_exit_with_cleanup(const char *message, int fd)
{
    if (fd >= 0) close(fd);
    t_cub *data = get_cub_data();
    free_array(&data->textures);
    free_array(&data->colors);
    free_array(&data->map);
    error_exit(message);
}

t_cub *get_cub_data(void)
{
    static t_cub data = {0};
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
            return -1;

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
    static char *buffers[OPEN_MAX];

    if (fd < 0 || BUFFER_SIZE <= 0 || fd >= OPEN_MAX || !line)
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
        return extract_line(&buffers[fd], line);
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
    char *line;
    int ret;

    t_cub *data = get_cub_data();
    data->textures = calloc(TEXTURE_COUNT + 1, sizeof(char *));
    data->colors = calloc(RGB_COUNT + 1, sizeof(char *));
    if (!data->textures || !data->colors)
        return 1;

    while (ft_array_length(data->textures) < TEXTURE_COUNT || ft_array_length(data->colors) < RGB_COUNT)
    {
        ret = get_next_line(fd, &line);
        if (ret == -1)
            return 1;
        if (*line)
        {
            if (parse_textures(line) && parse_colors(line))
            {
                free(line);
                return 1;
            }
        }
        free(line);
        if (ret == 0) break;
    }
    return 0;
}

int parse_textures(char *line)
{
    char **tokens = ft_split(line, ' ');
    if (!tokens || ft_array_length(tokens) != 2)
    {
        free_array(&tokens);
        return 1;
    }

    t_cub *data = get_cub_data();
    if (strcmp(tokens[0], "NO") == 0)
        data->textures[0] = strdup(tokens[1]);
    else if (strcmp(tokens[0], "SO") == 0)
        data->textures[1] = strdup(tokens[1]);
    else if (strcmp(tokens[0], "WE") == 0)
        data->textures[2] = strdup(tokens[1]);
    else if (strcmp(tokens[0], "EA") == 0)
        data->textures[3] = strdup(tokens[1]);
    else
    {
        free_array(&tokens);
        return 1;
    }
    free_array(&tokens);
    return 0;
}

int parse_colors(char *line)
{
    char **tokens = ft_split(line, ' ');
    if (!tokens || ft_array_length(tokens) != 2)
    {
        free_array(&tokens);
        return 1;
    }

    t_cub *data = get_cub_data();
    if (strcmp(tokens[0], "F") == 0)
        data->colors[0] = strdup(tokens[1]);
    else if (strcmp(tokens[0], "C") == 0)
        data->colors[1] = strdup(tokens[1]);
    else
    {
        free_array(&tokens);
        return 1;
    }
    free_array(&tokens);
    return 0;
}


char **reallocate_map_memory(char **pointer, int size)
{
    int i;
    char **new_pointer;

    i = 0;
    new_pointer = malloc(sizeof(char *) * size);
    if (!new_pointer)
        return (NULL);

    while (pointer[i])
    {
        new_pointer[i] = pointer[i];
        i++;
    }
    new_pointer[i] = NULL;
    free(pointer);
    return (new_pointer);
}



static int	add_line_to_map(char *line)
{
	char	**tmp;

    t_cub *data = get_cub_data();
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
		tmp = reallocate_map_memory(data->map, ft_array_length(data->map) + 2);
		if (!tmp)
			return (1);
		data->map = tmp;
		data->map[ft_array_length(data->map)] = strdup(line);
	}
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

int parse(char *file)
{
    if (!validate_file_extension(file))
        error_exit(ERR_INVALID_EXT);

    int fd = open(file, O_RDONLY);
    if (fd < 0)
        error_exit(ERR_FILE_OPEN);

    if (parse_file(fd))
        error_exit_with_cleanup(ERR_INVALID_INFO, fd);

    if (parse_map(fd))
        error_exit_with_cleanup(ERR_INVALID_MAP, fd);
    

    close(fd);
    return 0;
}

//added

static void	set_values(double direction_x, double direction_y, double pla_x, double pla_y)
{
    t_cub *data = get_cub_data();

	data->var.direction_x = direction_x;
	data->var.direction_y = direction_y;
	data->var.plane_x = pla_x;
	data->var.plane_y = pla_y;
}

static void	init_vectors(int x, int y)
{
    t_cub *data = get_cub_data();

	data->var.position_x = x + 0.5;
	data->var.position_y = y + 0.5;
	if (data->map[y][x] == 'N')
		set_values(0, -1, 0.66, 0);
	else if (data->map[y][x] == 'S')
		set_values(0, 1, -0.66, 0);
	else if (data->map[y][x] == 'E')
		set_values(1, 0, 0, 0.66);
	else if (data->map[y][x] == 'W')
		set_values(-1, 0, 0, -0.66);
}

int	check_characters(void)
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
			if (!ft_strchr(" 10NSEW", data->map[i][j]))
				return (0);
			if (data->map[i][j] == 'N' || data->map[i][j] == 'S' \
				|| data->map[i][j] == 'E' || data->map[i][j] == 'W')
			{
				init_vectors(j, i);
				count++;
			}
			j++;
		}
		i++;
	}
	return (count);
}

static int	is_closed(int i, int j)
{
    t_cub *data = get_cub_data();

	if (data->map[i][j] == '0' \
		|| (data->map[i][j] != '1' && data->map[i][j] != ' '))
	{
		if (i == 0 || !data->map[i + 1] || j == 0 || !data->map[i][j + 1])
			return (1);
		if (data->map[i - 1] && data->map[i - 1][j]
			&& data->map[i - 1][j] == ' ')
			return (1);
		if (data->map[i + 1] && data->map[i + 1][j]
			&& data->map[i + 1][j] == ' ')
			return (1);
		if (data->map[i] && data->map[i][j - 1]
			&& data->map[i][j - 1] == ' ')
			return (1);
		if (data->map[i] && data->map[i][j + 1]
			&& data->map[i][j + 1] == ' ')
			return (1);
	}
	return (0);
}

static int	is_end(int index)
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

int	validate_map(void)
{
	int	i;
	int	j;
    
    t_cub *data = get_cub_data();
	if (ft_array_length(data->map) < 3 || check_characters() != 1)
		return (1);
	i = 0;
	while (data->map[i])
	{
		if (strlen(data->map[i]) == 0)
		{
			if (is_end(i) == 1)
				return (1);
			break ;
		}
		j = 0;
		while (data->map[i][j])
		{
			if (is_closed(i, j) == 1)
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}

static void	ft_exit_init(char *s)
{
    t_cub *data = get_cub_data();

	write(2, s, ft_strlen(s));
	ft_free(&data->xpm);
	ft_free(&data->rgb);
	ft_free(&data->map);
	exit(1);
}

static void	ft_exit_parse(char *s, int fd)
{
	if (fd < 0)
		close(fd);
    t_cub *data = get_cub_data();
    
	write(2, s, ft_strlen(s));
	ft_free(&data->xpm);
	ft_free(&data->rgb);
	ft_free(&data->map);
	exit(1);
}

int main(int argc, char **argv)
{
    if (argc < 2)
        error_exit(ERR_TOO_FEW_ARGS);
    if (argc > 2)
        error_exit(ERR_TOO_MANY_ARGS);
    parse(argv[1]);
    return EXIT_SUCCESS;
}
