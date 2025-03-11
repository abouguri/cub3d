/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 23:56:44 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/10 23:58:38 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	handle_keypress(int keycode, t_game_state *game)
{
	if (keycode == KEY_ESC)
		exit_game(game);
	else if (keycode == KEY_W)
		game->keys.w_pressed = 1;
	else if (keycode == KEY_S)
		game->keys.s_pressed = 1;
	else if (keycode == KEY_A)
		game->keys.a_pressed = 1;
	else if (keycode == KEY_D)
		game->keys.d_pressed = 1;
	else if (keycode == KEY_LEFT)
		game->keys.left_pressed = 1;
	else if (keycode == KEY_RIGHT)
		game->keys.right_pressed = 1;
	return (0);
}

int	handle_keyrelease(int keycode, t_game_state *game)
{
	if (keycode == KEY_W)
		game->keys.w_pressed = 0;
	else if (keycode == KEY_S)
		game->keys.s_pressed = 0;
	else if (keycode == KEY_A)
		game->keys.a_pressed = 0;
	else if (keycode == KEY_D)
		game->keys.d_pressed = 0;
	else if (keycode == KEY_LEFT)
		game->keys.left_pressed = 0;
	else if (keycode == KEY_RIGHT)
		game->keys.right_pressed = 0;
	return (0);
}
