/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 20:45:45 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/03 00:39:08 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

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

char *create_line(const char *buffer, int length)
{
    char *line = malloc(length + 1);
    if (!line)
        return NULL;

    strncpy(line, buffer, length);
    line[length] = '\0';
    return line;
}

char *get_remaining_content(const char *buffer, int index)
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