/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   math_utils.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/11 04:36:19 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/12 04:19:59 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	is_in_circle_border(int x, int y, int outer_radius, int inner_radius)
{
	double	distance;

	distance = sqrt(x * x + y * y);
	return (distance <= outer_radius && distance >= inner_radius);
}

unsigned int	custom_rand(t_game_state *g)
{
	g->rand_state = g->rand_state * 1103515245 + 12345;
	return ((g->rand_state / 65836) % 32768);
}

void	custom_srand(t_game_state *g, unsigned int seed)
{
	g->rand_state = seed;
}
