/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   input_handling.c                                   :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/10 23:56:44 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/18 00:26:38 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

static void reset_game(t_game_state *game)
{
    game->player.health = 100;
    game->player.damage_cooldown = 0;
    game->player.is_damaged = 0;
    game->game_status = GAME_ACTIVE;
    game->data->var.position_x = game->initial_pos_x;
    game->data->var.position_y = game->initial_pos_y;
    if (game->enemy_manager.enemies)
    {
        free(game->enemy_manager.enemies);
        game->enemy_manager.enemies = NULL;
    }
    initialize_enemies(game);
    init_input_state(game);
}

int	handle_keypress(int keycode, t_game_state *game)
{
	if (keycode == KEY_ESC)
		on_destroy(game);
	if (game->game_status == GAME_OVER)
	{
		if (keycode == KEY_R)
			reset_game(game);
		return (0);
	}
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

int	on_destroy(void *data)
{
	t_game_state	*game;

	game = (t_game_state *)data;
	if (game && game->data && game->data->mlx)
		mlx_loop_end(game->data->mlx);
	return (0);
}
