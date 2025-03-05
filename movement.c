/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   movement.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 21:06:54 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/05 20:35:14 by abouguri         ###   ########.fr       */
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

// int render_frame(void *param)
// {
// 	t_game_state *game = (t_game_state *)param;
// 	t_data img;

// 	if (!game || !game->data || !game->data->mlx || !game->data->win)
// 	{
// 		printf("Error: Invalid game state or MLX instance.\n");
// 		return (1);
// 	}

// 	img.img = mlx_new_image(game->data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
// 	if (!img.img)
// 	{
// 		printf("Error: Failed to create new image.\n");
// 		error_exit_cleanup(ERR_CREATE_IMAGE);
// 	}

// 	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel,
// 			&img.line_length, &img.endian);
// 	if (!img.addr)
// 	{
// 		printf("Error: Failed to get image data address.\n");
// 		mlx_destroy_image(game->data->mlx, img.img);
// 		error_exit_cleanup(ERR_GET_IMAGE_ADDR);
// 	}

// 	printf("Debug: Calling raycast function.\n");
// 	raycast(game->data, &img);
// 	printf("Debug: Finished raycast function.\n");

// 	// Debugging: Check if the image buffer is being modified
// 	int *pixels = (int *)img.addr;
// 	int all_black = 1;
// 	for (int i = 0; i < (SCREEN_WIDTH * SCREEN_HEIGHT); i++)
// 	{
// 		if (pixels[i] != 0x000000) // Check if any pixel is not black
// 		{
// 			all_black = 0;
// 			break;
// 		}
// 	}
// 	if (all_black)
// 		printf("Warning: All pixels are black.\n");
// 	else
// 		printf("Debug: Image contains non-black pixels.\n");

// 	mlx_put_image_to_window(game->data->mlx, game->data->win, img.img, 0, 0);
// 	printf("Debug: Image put to window.\n");

// 	mlx_destroy_image(game->data->mlx, img.img);
// 	printf("Debug: Image destroyed.\n");

// 	return (0);
// }



int render_frame(void *param)
{
	t_game_state *game = (t_game_state *)param;
	t_data img;

	img.img = mlx_new_image(game->data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!img.img)
		error_exit_cleanup(ERR_CREATE_IMAGE);
	img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel,
			&img.line_length, &img.endian);
	if (!img.addr)
	{
		mlx_destroy_image(game->data->mlx, img.img);
		error_exit_cleanup(ERR_GET_IMAGE_ADDR);
	}
	raycast(game->data, &img);
	mlx_put_image_to_window(game->data->mlx, game->data->win, img.img, 0, 0);
	mlx_destroy_image(game->data->mlx, img.img);

	return (0);
}