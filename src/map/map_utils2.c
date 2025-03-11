/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   map_utils2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 06:02:42 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 06:04:26 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void get_map_dimensions(t_cub *data, int *width, int *height)
{
    int i;
    
    *height = ft_array_length(data->map);
    *width = 0;
    i = 0;
    while (i < *height)
    {
        int current_width = 0;
        while (data->map[i][current_width])
            current_width++;
        if (current_width > *width)
            *width = current_width;
        i++;
    }
}

int valid_position(t_cub *data, int x, int y)
{
    int row_len = strlen(data->map[y]);
    
    return (x < row_len && data->map[y][x] == '0');
}

int count_valid_positions(t_cub *data, int map_width, int map_height)
{
    int x, y;
    int valid_count = 0;
    
    y = 1;
    while (y < map_height - 1)
    {
        x = 1;
        while (x < map_width - 1)
        {
            if (valid_position(data, x, y))
                valid_count++;
            x++;
        }
        y++;
    }
    
    return valid_count;
}

void store_valid_positions(t_cub *data, int map_width, int map_height, t_position *valid_positions)
{
    int x, y;
    int position_index = 0;
    
    y = 1;
    while (y < map_height - 1)
    {
        x = 1;
        while (x < map_width - 1)
        {
            if (valid_position(data, x, y))
            {
                valid_positions[position_index].x = x;
                valid_positions[position_index].y = y;
                position_index++;
            }
            x++;
        }
        y++;
    }
}

t_position *collect_valid_positions(t_cub *data, int map_width, int map_height, int *count)
{
    t_position *valid_positions;
    
    *count = count_valid_positions(data, map_width, map_height);
    if (*count <= 0)
        return NULL;
    valid_positions = malloc(sizeof(t_position) * (*count));
    if (!valid_positions)
        return NULL;
    store_valid_positions(data, map_width, map_height, valid_positions);
    return valid_positions;
}
