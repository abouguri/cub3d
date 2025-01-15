/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/05 17:48:57 by abouguri          #+#    #+#             */
/*   Updated: 2025/01/15 21:21:51 by abouguri         ###   ########.fr       */
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
    // printf(".........%d\n", i);
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
// funcs extra

static void	ft_exit_parse(char *s, int fd)
{
	if (fd < 0)
		close(fd);
    t_cub *data = get_cub_data();
    
	write(2, s, strlen(s));
	free_array(&data->textures);
	free_array(&data->rgb);
	free_array(&data->map);
	exit(1);
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
    data->textures = calloc(sizeof(char *), TEXTURE_COUNT + 1);
    data->colors = calloc(sizeof(char *), RGB_COUNT + 1);
    if (!data->textures || !data->colors)
        return 1;

    while (ft_array_length(data->textures) != TEXTURE_COUNT && ft_array_length(data->colors) != RGB_COUNT)
    {
        ret = get_next_line(fd, &line);
        if (ret == -1)
            return 1;
        if (strlen(line) == 0)
            ;
        else if (parse_textures(line) && parse_colors(line))
        {
            free(line);
            return 1;
        }

        free(line);
        if (ret == 0) break;
    printf("**%s**** %d ******\n", data->textures[0],ft_array_length(data->textures));
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
        data->textures[0] = strdup(tokens[1]);
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
    // printf("%s\n%s\n", tokens[0], tokens[1]);
    t_cub *data = get_cub_data();
    if (!tokens || ft_array_length(tokens) != 2)
    {
        free_array(&tokens);
        return 1;
    }
    if (strncmp(tokens[0], "F", 1) == 0)
        data->colors[0] = strdup(tokens[1]);
    else if (strncmp(tokens[0], "C", 1) == 0)
        data->colors[1] = strdup(tokens[1]);
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

    if (parse_file(fd) == 1)
        error_exit_with_cleanup(ERR_INVALID_INFO, fd);

    if (parse_map(fd) == 1)
        error_exit_with_cleanup(ERR_INVALID_MAP, fd);
    
    // if (validate_full_map() == 1)
    //     ft_exit_parse(ERR_INVALID_MAP, fd);
    
    close(fd);
    return 0;
}

//added

int main(int argc, char **argv)
{
    if (argc < 2)
        error_exit(ERR_TOO_FEW_ARGS);
    if (argc > 2)
        error_exit(ERR_TOO_MANY_ARGS);
    parse(argv[1]); 
    return EXIT_SUCCESS; 
}
