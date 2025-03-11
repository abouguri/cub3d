/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 04:36:19 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 06:34:13 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

void	rotate_point(int *x, int *y, int center_x, int center_y, double angle)
{
	int	original_x;
	int	original_y;

	original_x = *x - center_x;
	original_y = *y - center_y;
	*x = center_x + (int)(original_x * cos(angle) - original_y * sin(angle));
	*y = center_y + (int)(original_x * sin(angle) + original_y * cos(angle));
}

int	is_in_circle_border(int x, int y, int center_x, int center_y, int outer_radius, int inner_radius)
{
	double	distance;

	(void)center_x;
	(void)center_y;
	distance = sqrt(x * x + y * y);
	return (distance <= outer_radius && distance >= inner_radius);
}

unsigned int custom_rand(t_game_state *g) 
{
    g->rand_state = g->rand_state * 1103515245 + 12345;
    return (g->rand_state / 65836) % 32768;
}

void custom_srand(t_game_state *g, unsigned int seed) 
{
    g->rand_state = seed;
}
