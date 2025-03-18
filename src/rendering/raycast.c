/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   raycast.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 20:59:25 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/18 03:14:45 by abouguri         ###   ########.fr       */
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

void	update_render_enemies(t_game_state *game)
{
	update_enemies(game);
	render_enemies(game, game->z_buffer);
}

void	raycast(t_game_state *game)
{
	t_raycast	ctx;
	int			x;

	ctx.data = game->data;
	ctx.img = game->img;
	x = -1;
	while (++x < SCREEN_WIDTH)
	{
		calc_ray_pos_dir(&ctx.data->var, x, &ctx.ray_dir.x, &ctx.ray_dir.y);
		set_dda_params(&ctx.dda, &ctx.data->var, ctx.ray_dir.x, ctx.ray_dir.y);
		perform_dda(&ctx.dda, ctx.data->map);
		calc_wall_params(&ctx.dda, &ctx.data->var, ctx.ray_dir, &ctx.render);
		game->z_buffer[x] = ctx.render.perp_wall_dist;
		calc_texture_coords(&ctx.dda, &ctx.data->var, &ctx.render, ctx.ray_dir);
		draw_floor_ceiling(ctx.img, x, &ctx.render, ctx.data);
		draw_textured_line(ctx.img, x, &ctx.render, ctx.data);
	}
	if (game->enemy_manager.enemies != NULL)
		update_render_enemies(game);
	if (game->player.is_damaged)
		apply_damage_effect(game);
	draw_health_bar(game);
	draw_2d_map(game);
}
