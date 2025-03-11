/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 06:04:56 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 06:05:30 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	handle_player_movement(t_game_state *game)
{
	double	move_speed;
	double	rot_speed;

	move_speed = 0.02;
	rot_speed = 0.02;
	if (game->keys.w_pressed)
		move_forward_backward(game->data, move_speed, 1);
	if (game->keys.s_pressed)
		move_forward_backward(game->data, move_speed, -1);
	if (game->keys.a_pressed)
		move_strafe(game->data, move_speed, 1);
	if (game->keys.d_pressed)
		move_strafe(game->data, move_speed, -1);
	if (game->keys.left_pressed)
		rotate_view(game->data, rot_speed, -1);
	if (game->keys.right_pressed)
		rotate_view(game->data, rot_speed, 1);
}
