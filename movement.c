/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 21:06:54 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/06 03:42:49 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"
/*
** Key handling function
*/
int	handle_keypress(int keycode, t_game_state *game)
{
	double	move_speed;
	double	rot_speed;

	move_speed = 0.1;
	rot_speed = 0.05;
	if (keycode == KEY_ESC)
		exit_game(game);
	else if (keycode == KEY_W)
		move_forward_backward(game->data, move_speed, 1);
	else if (keycode == KEY_S)
		move_forward_backward(game->data, move_speed, -1);
	else if (keycode == KEY_A)
		move_strafe(game->data, move_speed, 1);
	else if (keycode == KEY_D)
		move_strafe(game->data, move_speed, -1);
	else if (keycode == KEY_LEFT)
		rotate_view(game->data, rot_speed, -1);
	else if (keycode == KEY_RIGHT)
		rotate_view(game->data, rot_speed, 1);
	render_frame(game);
	return (0);
}
/*
** Handle forward/backward movement
*/
void	move_forward_backward(t_cub *data, double move_speed, int direction)
{
	double	new_pos_x;
	double	new_pos_y;

	new_pos_x = data->var.position_x + direction
		* data->var.direction_x * move_speed;
	new_pos_y = data->var.position_y + direction
		* data->var.direction_y * move_speed;
	if (is_valid_position(data->map, new_pos_x, data->var.position_y))
		data->var.position_x = new_pos_x;
	if (is_valid_position(data->map, data->var.position_x, new_pos_y))
		data->var.position_y = new_pos_y;
}
/*
** Handle strafe movement (left/right)
*/
void	move_strafe(t_cub *data, double move_speed, int direction)
{
	double	new_pos_x;
	double	new_pos_y;

	new_pos_x = data->var.position_x + direction
		* data->var.direction_y * move_speed;
	new_pos_y = data->var.position_y - direction
		* data->var.direction_x * move_speed;
	if (is_valid_position(data->map, new_pos_x, data->var.position_y))
		data->var.position_x = new_pos_x;
	if (is_valid_position(data->map, data->var.position_x, new_pos_y))
		data->var.position_y = new_pos_y;
}
/*
** Handle rotation (looking left/right)
*/
void	rotate_view(t_cub *data, double rot_speed, int direction)
{
	double	old_dir_x;
	double	old_plane_x;

	old_dir_x = data->var.direction_x;
	data->var.direction_x = data->var.direction_x * cos(direction * rot_speed)
		- data->var.direction_y * sin(direction * rot_speed);
	data->var.direction_y = old_dir_x * sin(direction * rot_speed)
		+ data->var.direction_y * cos(direction * rot_speed);
	old_plane_x = data->var.plane_x;
	data->var.plane_x = data->var.plane_x * cos(direction * rot_speed)
		- data->var.plane_y * sin(direction * rot_speed);
	data->var.plane_y = old_plane_x * sin(direction * rot_speed)
		+ data->var.plane_y * cos(direction * rot_speed);
}
/*
** Check if the new position is valid (not inside a wall)
*/
int	is_valid_position(char **map, double pos_x, double pos_y)
{
	if (map[(int)pos_y][(int)pos_x] == '0'
		|| map[(int)pos_y][(int)pos_x] == 'N'
		|| map[(int)pos_y][(int)pos_x] == 'S'
		|| map[(int)pos_y][(int)pos_x] == 'E'
		|| map[(int)pos_y][(int)pos_x] == 'W')
		return (1);
	return (0);
}

/*
** Exit function
*/
int	exit_game(t_game_state *game)
{
	mlx_destroy_window(game->data->mlx, game->data->win);
	// Free resources
	free_resources(game->data);
	exit(EXIT_SUCCESS);
	return (0);
}
int on_destroy(void *data)
{
    t_cub *cub_data = (t_cub*)data;
    
    // End the MLX loop first
    mlx_loop_end(cub_data->mlx);
    
    // Destroy window
    if (cub_data->win)
        mlx_destroy_window(cub_data->mlx, cub_data->win);
    
    // Destroy display (Linux only)
    if (cub_data->mlx) {
        mlx_destroy_display(cub_data->mlx);
        free(cub_data->mlx);
    }
    
    exit(0);
    return (0);
}

