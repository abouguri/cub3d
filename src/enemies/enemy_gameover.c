/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_gameover.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 01:56:31 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/18 02:19:06 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	darken_color(int color, int shift_amount)
{
	return ((color & ALPHA_MASK) | ((color & RED_MASK) >> shift_amount)
		| ((color & GREEN_MASK) >> shift_amount)
		| ((color & BLUE_MASK) >> shift_amount));
}

int	darken_color_quarter(int color)
{
	return (darken_color(color, 2));
}

void	draw_game_over(t_game_state *game)
{
	int	i;
	int	j;
	int	current_color;
	int	darkened;
	int	alpha;

	alpha = 255;
	if (game->game_over_timer > 120)
		alpha = 255 - (game->game_over_timer - 120) * 4;
	if (alpha < 0)
		alpha = 0;
	i = 0;
	while (i < SCREEN_WIDTH)
	{
		j = 0;
		while (j < SCREEN_HEIGHT)
		{
			current_color = *(int *)(game->img->addr + (j
						* game->img->line_length + i
						* (game->img->bits_per_pixel / 8)));
			darkened = darken_color_quarter(current_color);
			my_mlx_pixel_put(game->img, i, j, darkened);
		}
	}
	game->render_game_over_text = 1;
}
