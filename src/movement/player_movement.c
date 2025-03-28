/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   player_movement.c                                  :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 06:04:56 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/21 01:16:44 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static double	calculate_scale_factor(void)
{
	double	reference_width;
	double	reference_height;
	double	scale_factor_width;
	double	scale_factor_height;

	reference_width = 1280.0;
	reference_height = 720.0;
	scale_factor_width = (double)SCREEN_WIDTH / reference_width;
	scale_factor_height = (double)SCREEN_HEIGHT / reference_height;
	return ((scale_factor_width + scale_factor_height) / 2.0);
}

static double	calculate_movement_speed(void)
{
	double	base_move_speed;
	double	scale_factor;

	base_move_speed = 0.03;
	scale_factor = calculate_scale_factor();
	return (base_move_speed * scale_factor);
}

static double	calculate_rotation_speed(void)
{
	double	base_rot_speed;
	double	scale_factor;

	base_rot_speed = 0.02;
	scale_factor = calculate_scale_factor();
	return (base_rot_speed * scale_factor);
}

void	handle_player_movement(t_game_state *game)
{
	double	move_speed;
	double	rot_speed;

	move_speed = calculate_movement_speed();
	rot_speed = calculate_rotation_speed();
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
