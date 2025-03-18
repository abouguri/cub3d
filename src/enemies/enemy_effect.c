/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_effect.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 01:49:59 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/18 01:50:10 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	draw_rectangle(t_game_state *game, t_rectangle rect)
{
	int	i;
	int	j;

	i = 0;
	while (i < rect.width)
	{
		j = 0;
		while (j < rect.height)
		{
			my_mlx_pixel_put(game->img, rect.x + i, rect.y + j, rect.color);
			j++;
		}
		i++;
	}
}

int	get_health_bar_color(int health, int max_health)
{
	int	health_percent;

	health_percent = (health * 100) / max_health;
	if (health_percent > 70)
		return (GREEN);
	else if (health_percent > 30)
		return (YELLOW);
	else
		return (RED);
}

void	draw_health_bar(t_game_state *game)
{
	t_health_bar	bar;
	t_rectangle		bg_rect;
	t_rectangle		fill_rect;

	bar.width = 200;
	bar.height = 20;
	bar.x = SCREEN_WIDTH / 2 - bar.width / 2;
	bar.y = SCREEN_HEIGHT - 40;
	bar.fill_width = (game->player.health * bar.width)
		/ game->player.max_health;
	bar.bg_color = DARK_RED;
	bar.fill_color = get_health_bar_color(game->player.health,
			game->player.max_health);
	bg_rect.x = bar.x;
	bg_rect.y = bar.y;
	bg_rect.width = bar.width;
	bg_rect.height = bar.height;
	bg_rect.color = bar.bg_color;
	fill_rect.x = bar.x;
	fill_rect.y = bar.y;
	fill_rect.width = bar.fill_width;
	fill_rect.height = bar.height;
	fill_rect.color = bar.fill_color;
	draw_rectangle(game, bg_rect);
	draw_rectangle(game, fill_rect);
}
