/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init3.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/21 00:47:53 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/21 00:51:30 by abouguri         ###   ########.fr       */
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

static double	calculate_enemy_speed(void)
{
	double	base_move_speed;
	double	scale_factor;

	base_move_speed = 0.02;
	scale_factor = calculate_scale_factor();
	return (base_move_speed * scale_factor);
}

static double	calculate_detection_radius(void)
{
	double	base_detection_radius;
	double	scale_factor;

	base_detection_radius = 2.0;
	scale_factor = calculate_scale_factor();
	return (base_detection_radius * scale_factor);
}

void	init_enemy(t_enemy *enemy, double pos_x, double pos_y)
{
	enemy->pos_x = pos_x;
	enemy->pos_y = pos_y;
	enemy->dir_x = 1.0;
	enemy->dir_y = 0.0;
	enemy->move_speed = calculate_enemy_speed();
	enemy->detection_radius = calculate_detection_radius();
	enemy->state = 0;
	enemy->move_timer = 0;
}
