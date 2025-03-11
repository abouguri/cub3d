/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   mouse_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 23:58:07 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 00:00:06 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int handle_mouse_move(int x, int y, t_game_state *game)
{
    int mouse_delta;
    double rot_speed;
    (void)y;

    mouse_delta = x - game->prev_mouse_x;
    if (mouse_delta != 0)
    {
        rot_speed = 0.006 * mouse_delta;
        rotate_view(game->data, fabs(rot_speed), (rot_speed > 0) ? 1 : -1);
        game->prev_mouse_x = x;
    }
    
    return (0);
}
