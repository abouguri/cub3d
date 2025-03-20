/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 06:04:56 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/20 04:40:22 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	handle_player_movement(t_game_state *game)
{
	double	move_speed;
	double	rot_speed;
	
    double reference_width = 1280.0;
    double reference_height = 720.0;
    double base_move_speed = 0.03;
    double base_rot_speed = 0.02;
    double scale_factor_width = (double)SCREEN_WIDTH / reference_width;
    double scale_factor_height = (double)SCREEN_HEIGHT / reference_height;
	double scale_factor = (scale_factor_width + scale_factor_height) / 2.0;
	move_speed = base_move_speed * scale_factor;
	rot_speed = base_rot_speed * scale_factor;
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
