/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_movement.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 05:36:10 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/20 04:55:31 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	can_enemy_move(t_game_state *game, double new_x, double new_y)
{
	int	map_x;
	int	map_y;

	map_x = (int)new_x;
	map_y = (int)new_y;
	if (map_y >= 0 && map_x >= 0 && game->data->map[map_y]
		&& game->data->map[map_y][map_x] == '1')
		return (0);
	return (1);
}

void	update_enemy_random_movement(t_game_state *game, t_enemy *enemy)
{
	double	new_x;
	double	new_y;
	double	angle;

	enemy->move_timer++;
	if (enemy->move_timer >= 100)
	{
		enemy->move_timer = 0;
		angle = (custom_rand(game) % 628) / 100.0;
		enemy->dir_x = cos(angle);
		enemy->dir_y = sin(angle);
	}
	new_x = enemy->pos_x + enemy->dir_x * enemy->move_speed;
	new_y = enemy->pos_y + enemy->dir_y * enemy->move_speed;
	if (can_enemy_move(game, new_x, new_y))
	{
		enemy->pos_x = new_x;
		enemy->pos_y = new_y;
	}
	else
	{
		enemy->dir_x = -enemy->dir_x;
		enemy->dir_y = -enemy->dir_y;
		enemy->move_timer = 99;
	}
}

int	is_player_detected(t_enemy *enemy, t_cub *data)
{
	double	dx;
	double	dy;
	double	distance;

	dx = data->var.position_x - enemy->pos_x;
	dy = data->var.position_y - enemy->pos_y;
	distance = sqrt(dx * dx + dy * dy);
	return (distance <= enemy->detection_radius);
}

void	update_enemy_following(t_game_state *game, t_enemy *enemy)
{
	t_cub		*data;
	double		length;
	t_vector	d;
	t_vector	new;

	data = game->data;
	d.x = data->var.position_x - enemy->pos_x;
	d.y = data->var.position_y - enemy->pos_y;
	length = sqrt(d.x * d.x + d.y * d.y);
	if (length > 0)
	{
		d.x /= length;
		d.y /= length;
	}
	enemy->dir_x = d.x;
	enemy->dir_y = d.y;
	new.x = enemy->pos_x + enemy->dir_x * enemy->move_speed;
	new.y = enemy->pos_y + enemy->dir_y * enemy->move_speed;
	if (can_enemy_move(game, new.x, new.y))
	{
		enemy->pos_x = new.x;
		enemy->pos_y = new.y;
	}
	if (!is_player_detected(enemy, data))
		enemy->state = 0;
}

int	is_player_colliding_with_enemy(t_enemy *enemy, t_cub *data)
{
	double	dx;
	double	dy;
	double	distance;

	dx = data->var.position_x - enemy->pos_x;
	dy = data->var.position_y - enemy->pos_y;
	distance = sqrt(dx * dx + dy * dy);
	return (distance <= 0.5);
}
