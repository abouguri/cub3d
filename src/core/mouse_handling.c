/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 23:58:07 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 07:12:33 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	handle_mouse_move(int x, int y, t_game_state *game)
{
	int		mouse_delta;
	double	rot_speed;
	int		direction;

	(void)y;
	mouse_delta = x - game->prev_mouse_x;
	if (mouse_delta != 0)
	{
		rot_speed = 0.006 * mouse_delta;
		if (rot_speed > 0)
			direction = 1;
		else
			direction = -1;
		rotate_view(game->data, fabs(rot_speed), direction);
		game->prev_mouse_x = x;
	}
	return (0);
}
