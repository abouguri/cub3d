/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 20:59:25 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 04:43:44 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	get_map_height(char **map)
{
	int	height;

	height = 0;
	while (map[height])
		height++;
	return (height);
}

void raycast(t_game_state *game)
{
    t_cub *data = game->data;
    t_data *img = game->img;
    int x;
    double ray_dir_x;
    double ray_dir_y;
    t_dda dda;
    t_render render;

    x = 0;
    while (x < SCREEN_WIDTH)
    {
        calc_ray_pos_dir(&data->var, x, &ray_dir_x, &ray_dir_y);
        set_dda_params(&dda, &data->var, ray_dir_x, ray_dir_y);
        perform_dda(&dda, data->map);
        calc_wall_params(&dda, &data->var, ray_dir_x, ray_dir_y, &render);
        game->z_buffer[x] = render.perp_wall_dist;
        calc_texture_coords(&dda, &data->var, &render, ray_dir_x, ray_dir_y);
        draw_floor_ceiling(img, x, &render, data);
        draw_textured_line(img, x, &render, data);
        x++;
    }
    if (game->enemy_manager.enemies != NULL)
	{
        update_enemies(game);
        render_enemies(game, game->z_buffer);
    }
    draw_2d_map(game);
}
