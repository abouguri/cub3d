/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minimap_draw.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 04:39:29 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 04:42:15 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_map_tiles_circular_zoomed(t_cub *data, t_data *img, t_map_params *p)
{
	int		x;
	int		y;
	int		draw_x;
	int		draw_y;
	double	distance;

	// int	color;
	y = -1;
	while (++y < p->view_height)
	{
		if (p->start_y + y >= get_map_height(data->map))
			break ;
		x = -1;
		while (++x < p->view_width)
		{
			if (p->start_x + x >= (int)strlen(data->map[p->start_y + y]))
				break ;
			// Center the view around the player
			draw_x = p->center_x + (x - p->view_width / 2) * p->tile_size;
			draw_y = p->center_y + (y - p->view_height / 2) * p->tile_size;
			// Check if tile is within circle
			distance = sqrt(pow(draw_x - p->center_x, 2) + pow(draw_y
						- p->center_y, 2));
			if (distance > p->radius)
				continue ;
			if (data->map[p->start_y + y][p->start_x + x] == '1')
				fill_circle(img, draw_x, draw_y, 3, CYAN);
			else if (data->map[p->start_y + y][p->start_x + x] == '0')
				fill_circle(img, draw_x, draw_y, 4, COOL_DARKGREY);
		}
	}
}

void	draw_direction_line_circular(t_data *img, int x0, int y0, t_map_params *p, t_cub *data)
{
	int		end_x;
	int		end_y;
	int		dx;
	int		dy;
	int		sx;
	int		sy;
	int		err;
	int		e2;
	double	distance;

	end_x = x0 + (int)(10 * data->var.direction_x);
	end_y = y0 + (int)(10 * data->var.direction_y);
	dx = abs(end_x - x0);
	sx = x0 < end_x ? 1 : -1;
	dy = -abs(end_y - y0);
	sy = y0 < end_y ? 1 : -1;
	err = dx + dy;
	while (1)
	{
		// Check if line point is within the map circle
		distance = sqrt(pow(x0 - p->center_x, 2) + pow(y0 - p->center_y, 2));
		if (distance <= p->radius)
			my_mlx_pixel_put(img, x0, y0, WHITE);
		if (x0 == end_x && y0 == end_y)
			break ;
		e2 = 2 * err;
		if (e2 >= dy)
		{
			err += dy;
			x0 += sx;
		}
		if (e2 <= dx)
		{
			err += dx;
			y0 += sy;
		}
	}
}

void	draw_player_on_circular_map(t_cub *data, t_data *img, t_map_params *p)
{
	int		player_x;
	int		player_y;
	double	distance;

	player_x = p->center_x + (int)((data->var.position_x - p->start_x
				- p->view_width / 2) * p->tile_size);
	player_y = p->center_y + (int)((data->var.position_y - p->start_y
				- p->view_height / 2) * p->tile_size);
	distance = sqrt(pow(player_x - p->center_x, 2) + pow(player_y - p->center_y,
				2));
	if (distance > p->radius)
		return ;
	fill_circle(img, player_x, player_y, 5, YELLOW);
	draw_direction_line_circular(img, player_x, player_y, p, data);
}

void draw_enemies_on_circular_map(t_game_state *game, t_data *img, t_map_params *p)
{
    int i;
    int draw_x, draw_y;
    double distance;
    
    // Check if enemies exist
    if (!game->enemy_manager.enemies || game->enemy_manager.enemy_count <= 0)
        return;
    
    // Draw each enemy
    for (i = 0; i < game->enemy_manager.enemy_count; i++)
    {
        t_enemy *enemy = &game->enemy_manager.enemies[i];
        
        // Calculate drawing position (similar to player drawing)
        draw_x = p->center_x + ((int)enemy->pos_x - p->start_x - p->view_width / 2) * p->tile_size;
        draw_y = p->center_y + ((int)enemy->pos_y - p->start_y - p->view_height / 2) * p->tile_size;
        
        // Check if enemy is within the circular view
        distance = sqrt(pow(draw_x - p->center_x, 2) + pow(draw_y - p->center_y, 2));
        if (distance > p->radius)
            continue;
        
        // Draw enemy (red circle)
        fill_circle(img, draw_x, draw_y, 3, RED);
    }
}
