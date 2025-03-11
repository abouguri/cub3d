/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_render.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 05:17:01 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 07:18:30 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	render_enemy(t_render_context *ctx)
{
	int	stripe;
	int	tex_x;
	int	y;
	int	d;
	int	tex_y;
	int	color;

	if (ctx->transform.transform_y <= 0)
		return ;
	stripe = ctx->transform.draw_start_x;
	while (stripe < ctx->transform.draw_end_x)
	{
		if (ctx->transform.transform_y > 0 && stripe > 0
			&& stripe < SCREEN_WIDTH
			&& ctx->transform.transform_y < ctx->z_buffer[stripe])
		{
			tex_x = (int)(256 * (stripe - (-ctx->transform.sprite_width / 2
							+ ctx->transform.sprite_screen_x)) * TEXTURE_WIDTH
					/ ctx->transform.sprite_width) / 256;
			y = ctx->transform.draw_start_y;
			while (y < ctx->transform.draw_end_y)
			{
				d = y * 256 - SCREEN_HEIGHT * 128 + ctx->transform.sprite_height
					* 128;
				tex_y = ((d * TEXTURE_HEIGHT) / ctx->transform.sprite_height)
					/ 256;
				color = ctx->game->data->texture[4][TEXTURE_WIDTH * tex_y
					+ tex_x];
				if ((color & 0x00FFFFFF) != 0)
				{
					my_mlx_pixel_put(ctx->game->img, stripe, y, color);
				}
				y++;
			}
		}
		stripe++;
	}
}

void	render_enemies(t_game_state *game, double *z_buffer)
{
    sort_enemies_by_distance(game);
    int i = 0;
    while (i < game->enemy_manager.enemy_count)
    {
        t_transform transform = {0};
        t_render_context ctx = {game, &game->enemy_manager.enemies[i], z_buffer, transform};
        setup_render_context(&ctx);
        render_enemy(&ctx);
        i++;
    }
}

void	calculate_enemy_transform(t_render_context *ctx)
{
	t_cub	*data;
	double	sprite_x;
	double	sprite_y;
	double	inv_det;

	data = ctx->game->data;
	sprite_x = ctx->enemy->pos_x - data->var.position_x;
	sprite_y = ctx->enemy->pos_y - data->var.position_y;
	inv_det = 1.0 / (data->var.plane_x * data->var.direction_y
			- data->var.direction_x * data->var.plane_y);
	ctx->transform.transform_x = inv_det * (data->var.direction_y * sprite_x
			- data->var.direction_x * sprite_y);
	ctx->transform.transform_y = inv_det * (-data->var.plane_y * sprite_x
			+ data->var.plane_x * sprite_y);
}

void	setup_render_context(t_render_context *ctx)
{
	calculate_enemy_transform(ctx);
	if (ctx->transform.transform_y <= 0)
		return ;

	ctx->transform.sprite_screen_x = (int)((SCREEN_WIDTH / 2) * (1
				+ ctx->transform.transform_x / ctx->transform.transform_y));
	ctx->transform.sprite_height = abs((int)(SCREEN_HEIGHT
				/ ctx->transform.transform_y));
	ctx->transform.draw_start_y = fmax(0, -ctx->transform.sprite_height / 2
			+ SCREEN_HEIGHT / 2);
	ctx->transform.draw_end_y = fmin(SCREEN_HEIGHT - 1,
			ctx->transform.sprite_height / 2 + SCREEN_HEIGHT / 2);
	ctx->transform.sprite_width = abs((int)(SCREEN_HEIGHT
				/ ctx->transform.transform_y));
	ctx->transform.draw_start_x = fmax(0, -ctx->transform.sprite_width / 2
			+ ctx->transform.sprite_screen_x);
	ctx->transform.draw_end_x = fmin(SCREEN_WIDTH - 1,
			ctx->transform.sprite_width / 2 + ctx->transform.sprite_screen_x);
}