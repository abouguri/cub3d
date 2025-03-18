/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_update.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 05:37:04 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/18 01:56:52 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	update_enemies(t_game_state *game)
{
	int		i;
	t_enemy	*enemy;

	if (game->game_status != GAME_ACTIVE)
		return ;
	i = 0;
	while (i < game->enemy_manager.enemy_count)
	{
		enemy = &game->enemy_manager.enemies[i];
		if (enemy->state == 0)
		{
			if (is_player_detected(enemy, game->data))
				enemy->state = 1;
			else
				update_enemy_random_movement(game, enemy);
		}
		else
		{
			update_enemy_following(game, enemy);
			check_enemy_damage(game, enemy);
		}
		i++;
	}
}

static void	swap_enemies_if_needed(t_game_state *game, t_cub *data, int j)
{
	double	dist1;
	double	dist2;
	t_enemy	temp;

	dist1 = pow(data->var.position_x - game->enemy_manager.enemies[j].pos_x, 2)
		+ pow(data->var.position_y - game->enemy_manager.enemies[j].pos_y, 2);
	dist2 = pow(data->var.position_x - game->enemy_manager.enemies[j + 1].pos_x,
			2) + pow(data->var.position_y - game->enemy_manager.enemies[j
			+ 1].pos_y, 2);
	if (dist1 < dist2)
	{
		temp = game->enemy_manager.enemies[j];
		game->enemy_manager.enemies[j] = game->enemy_manager.enemies[j + 1];
		game->enemy_manager.enemies[j + 1] = temp;
	}
}

void	sort_enemies_by_distance(t_game_state *game)
{
	t_index	i;
	t_cub	*data;

	i.i = 0;
	data = game->data;
	while (i.i < game->enemy_manager.enemy_count - 1)
	{
		i.j = 0;
		while (i.j < game->enemy_manager.enemy_count - i.i - 1)
		{
			swap_enemies_if_needed(game, data, i.j);
			i.j++;
		}
		i.i++;
	}
}

void	apply_damage_effect(t_game_state *game)
{
	int	i;
	int	j;
	int	color;

	if (game->player.is_damaged)
	{
		i = 0;
		while (i < SCREEN_WIDTH)
		{
			j = 0;
			while (j < SCREEN_HEIGHT)
			{
				color = ((unsigned char *)game->img->addr)[j
					* game->img->line_length + i * (game->img->bits_per_pixel
						/ 8)];
				color = ((color & 0xFF0000)) | ((color & 0x00FF00))
					| ((color & 0x0000FF));
				my_mlx_pixel_put(game->img, i, j, color);
				j++;
			}
			i++;
		}
	}
}
