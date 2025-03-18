/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   enemy_damage.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/18 01:45:32 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/18 01:45:56 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static double	get_player_enemy_distance(t_game_state *game, t_enemy *enemy)
{
	double	dx;
	double	dy;

	dx = game->data->var.position_x - enemy->pos_x;
	dy = game->data->var.position_y - enemy->pos_y;
	return (sqrt(dx * dx + dy * dy));
}

static void	damage_player(t_game_state *game)
{
	game->player.health -= 10;
	game->player.is_damaged = 1;
	game->player.damage_cooldown = 60;
	if (game->player.health <= 0)
	{
		game->player.health = 0;
		game->game_status = GAME_OVER;
		game->game_over_timer = 180;
	}
}

static void	update_damage_cooldown(t_game_state *game)
{
	game->player.damage_cooldown--;
	if (game->player.damage_cooldown == 0)
		game->player.is_damaged = 0;
}

void	check_enemy_damage(t_game_state *game, t_enemy *enemy)
{
	double	distance;

	distance = get_player_enemy_distance(game, enemy);
	if (game->player.damage_cooldown <= 0 && game->game_status == GAME_ACTIVE)
	{
		if (distance <= 0.5)
			damage_player(game);
	}
	else if (game->player.damage_cooldown > 0)
		update_damage_cooldown(game);
}
