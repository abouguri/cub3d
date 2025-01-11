
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
# include <limits.h>

#define BUFFER_SIZE 42

// Error Messages
#define ERR_TOO_FEW_ARGS "Error: Too few arguments. Provide a .cub file.\n"
#define ERR_TOO_MANY_ARGS "Error: Too many arguments. Only the .cub file is required.\n"
#define ERR_INVALID_EXT "Error: Invalid file extension. The file must have a .cub extension.\n"
#define ERR_FILE_OPEN "Error: Cannot open the map file.\n"
#define ERR_INVALID_INFO "Error: Invalid map information in the file.\n"
#define ERR_INVALID_MAP "Error: Invalid map structure.\n"

// Constants
#define TEXTURE_COUNT 4
#define RGB_COUNT 2

int	ft_start(char **buf, int fd, int *ret)
{
	buf[fd] = malloc(BUFFER_SIZE + 1);
	if (!buf[fd])
		return (-1);
	*ret = read(fd, buf[fd], BUFFER_SIZE);
	if (*ret < 0)
	{
		free(buf[fd]);
		return (-1);
	}
	buf[fd][*ret] = 0;
	return (0);
}

int	ft_in_while_a(char **buf, char *cat, char **tmp)
{
	int	i;

	i = 0;
	while ((*buf)[i])
	{
		cat[i] = (*buf)[i];
		i++;
	}
	i = 0;
	while ((*tmp)[i])
	{
		cat[i + strlen(*buf)] = (*tmp)[i];
		i++;
	}
	cat[i + strlen(*buf)] = 0;
	free(*tmp);
	free(*buf);
	*buf = cat;
	return (1);
}

int	ft_in_while(char **buf, int fd, int *ret)
{
	char	*tmp;
	char	*cat;

	tmp = malloc(BUFFER_SIZE + 1);
	if (!tmp)
		return (-1);
	*ret = read(fd, tmp, BUFFER_SIZE);
	if (*ret < 0)
	{
		free(tmp);
		return (-1);
	}
	tmp[*ret] = 0;
	cat = malloc(strlen(buf[fd]) + strlen(tmp) + 1);
	if (!cat)
	{
		free(tmp);
		return (-1);
	}
	return ((ft_in_while_a(&(buf[fd]), cat, &tmp)));
}

int	ft_if_new_line(char **buf, char **line)
{
	int		i;
	char	*tmp;

	i = 0;
	while ((*buf)[i] != 10)
		i++;
	*line = strdup(*buf);
	if (!*line)
		return (-1);
	(*line)[i] = 0;
	tmp = strdup(&((*buf)[i + 1]));
	if (!tmp)
		return (-1);
	free((*buf));
	*buf = tmp;
	return (1);
}

int	ft_new_line(char *buf)
{
	int	i;

	i = 0;
	while (buf[i] && buf[i] != 10)
		i++;
	if (buf[i] == 10)
		return (1);
	return (0);
}

int	get_next_line(int fd, char **line)
{
	int			ret;
	static char	*buf[OPEN_MAX];

	ret = BUFFER_SIZE;
	if (fd >= 0 && BUFFER_SIZE > 0 && fd < OPEN_MAX)
	{
		if (!buf[fd] && ft_start(buf, fd, &ret))
			return (-1);
		while (!ft_new_line(buf[fd]) && BUFFER_SIZE == ret)
			if (ft_in_while(buf, fd, &ret) == -1)
				return (-1);
		if (ft_new_line(buf[fd]))
			return (ft_if_new_line(&(buf[fd]), line));
		else
		{
			*line = strdup(buf[fd]);
			if (!*line)
				return (-1);
			free(buf[fd]);
			buf[fd] = NULL;
			return (0);
		}
	}
	return (-1);
}

// Utility Functions
void free_array(char ***array);
void handle_error(const char *message, int fd);

// Cub3D Data Structure
typedef struct s_cub {
    char **textures;
    char **colors;
    char **map;
} t_cub;

t_cub *get_cub_data(void) {
    static t_cub data = {0};
    return &data;
}

void free_array(char ***array)
{
    if (!*array) return;
    for (int i = 0; (*array)[i]; i++) {
        free((*array)[i]);
    }
    free(*array);
    *array = NULL;
}

void handle_error(const char *message, int fd)
{
    if (fd >= 0) close(fd);
    write(2, message, strlen(message));
    t_cub *data = get_cub_data();
    free_array(&data->textures);
    free_array(&data->colors);
    free_array(&data->map);
    exit(EXIT_FAILURE);
}

// Parsing Functions
int validate_file_extension(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    return dot && strcmp(dot + 1, "cub") == 0;
}

int	ft_array_length(char **array)
{
	int	i;

	i = 0;
	while (array[i])
	{
		i++;
	}
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

int parse_textures(char *line)
{
    char **tokens = ft_split(line, ' ');
    if (!tokens || ft_array_length(tokens) != 2) {
        free_array(&tokens);
        return 1;
    }

    t_cub *data = get_cub_data();
    if (strcmp(tokens[0], "NO") == 0) data->textures[0] = strdup(tokens[1]);
    else if (strcmp(tokens[0], "SO") == 0) data->textures[1] = strdup(tokens[1]);
    else if (strcmp(tokens[0], "WE") == 0) data->textures[2] = strdup(tokens[1]);
    else if (strcmp(tokens[0], "EA") == 0) data->textures[3] = strdup(tokens[1]);
    else {
        free_array(&tokens);
        return 1;
    }
    free_array(&tokens);
    return 0;
}

int parse_colors(char *line)
{
    char **tokens = ft_split(line, ' ');
    if (!tokens || ft_array_length(tokens) != 2) {
        free_array(&tokens);
        return 1;
    }

    t_cub *data = get_cub_data();
    if (strcmp(tokens[0], "F") == 0) data->colors[0] = strdup(tokens[1]);
    else if (strcmp(tokens[0], "C") == 0) data->colors[1] = strdup(tokens[1]);
    else {
        free_array(&tokens);
        return 1;
    }
    free_array(&tokens);
    return 0;
}

int parse_file(int fd)
{
    char *line;
    int ret;

    t_cub *data = get_cub_data();
    data->textures = calloc(TEXTURE_COUNT + 1, sizeof(char *));
    data->colors = calloc(RGB_COUNT + 1, sizeof(char *));
    if (!data->textures || !data->colors) return 1;

    while (ft_array_length(data->textures) < TEXTURE_COUNT || ft_array_length(data->colors) < RGB_COUNT) {
        ret = get_next_line(fd, &line);
        if (ret == -1) return 1;
        if (*line) {
            if (parse_textures(line) && parse_colors(line)) {
                free(line);
                return 1;
            }
        }
        free(line);
        if (ret == 0) break;
    }
    return 0;
}

int parse_map(int fd)
{
    // Implement map parsing logic
    return 0; // Return 1 if map validation fails
}

int parse(char *file)
{
    if (!validate_file_extension(file)) handle_error(ERR_INVALID_EXT, -1);

    int fd = open(file, O_RDONLY);
    if (fd < 0) handle_error(ERR_FILE_OPEN, -1);

    if (parse_file(fd)) handle_error(ERR_INVALID_INFO, fd);
    if (parse_map(fd)) handle_error(ERR_INVALID_MAP, fd);

    close(fd);
    return 0;
}

void error_args(const char *message)
{
    write(2, message, strlen(message));
    exit(EXIT_FAILURE);
}

// Main Function
int main(int argc, char **argv)
{
    if (argc < 2)
    {
        error_args(ERR_TOO_FEW_ARGS);
    }
    if (argc > 2)
    {
        error_args(ERR_TOO_MANY_ARGS);
    }
    parse(argv[1]);
    return EXIT_SUCCESS;
}