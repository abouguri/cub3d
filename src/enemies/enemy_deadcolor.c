/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_deadcolor.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 01:43:04 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/21 01:43:15 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static unsigned int	apply_red_tint(unsigned int color)
{
	unsigned int	r;
	unsigned int	g;
	unsigned int	b;

	r = (color & 0xFF0000) >> 16;
	g = (color & 0x00FF00) >> 8;
	b = (color & 0x0000FF);
	if (r > 200)
		r = 255;
	else
		r = r + 55;
	g = g / 2;
	b = b / 2;
	return ((r << 16) | (g << 8) | b);
}

static void	process_damage_row(t_game_state *game, int row)
{
	int				i;
	unsigned int	color;

	i = 0;
	while (i < SCREEN_WIDTH)
	{
		color = *(unsigned int *)(game->img->addr + row * game->img->line_length
				+ i * (game->img->bits_per_pixel / 8));
		color = apply_red_tint(color);
		my_mlx_pixel_put(game->img, i, row, color);
		i++;
	}
}

void	apply_damage_effect(t_game_state *game)
{
	int	j;

	if (game->player.is_damaged)
	{
		j = 0;
		while (j < SCREEN_HEIGHT)
		{
			process_damage_row(game, j);
			j++;
		}
	}
}
