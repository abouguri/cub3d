/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 21:09:34 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/07 05:57:39 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

char	*ft_strchr(const char *s, int c)
{
	while ((*s != '\0') && (*s != c))
		s++;
	if (*s == c)
		return ((char *)s);
	return ((char *) NULL);
}

void	init_window(t_cub *data, t_data *img, int win_width, int win_height)
{
	data->mlx = mlx_init();
	if (!data->mlx)
		error_exit_cleanup(ERR_INIT_MLX);
	data->win = mlx_new_window(data->mlx, win_width, win_height, "cub3D");
	if (!data->win)
		error_exit_cleanup(ERR_CREATE_WINDOW);
	img->img = mlx_new_image(data->mlx, win_width, win_height);
	if (!img->img)
		error_exit_cleanup(ERR_CREATE_IMAGE);
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel,
			&img->line_length, &img->endian);
	if (!img->addr)
		error_exit_cleanup(ERR_GET_IMAGE_ADDR);
}

int render_frame(void *param)
{
    t_game_state    *game;
    t_data          img;
    double          move_speed;
    double          rot_speed;

    game = (t_game_state *)param;
    move_speed = 0.02;
    rot_speed = 0.01;
    
    // Handle movement based on key states
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
    
    // Original rendering code
    img.img = mlx_new_image(game->data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!img.img)
        error_exit_cleanup(ERR_CREATE_IMAGE);
    img.addr = mlx_get_data_addr(img.img, &img.bits_per_pixel, &img.line_length,
            &img.endian);
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

void init(void)
{
    t_cub           *data;
    t_game_state    *g;
    int             win_height;
    int             win_width;

    data = get_cub_data();
    win_height = SCREEN_HEIGHT;
    win_width = SCREEN_WIDTH;
    g = malloc(sizeof(t_game_state));
    if (!g)
        error_exit_cleanup(ERR_MEMORY_ALLOCATION);
    
    // Initialize key states and mouse position
    g->keys.w_pressed = 0;
    g->keys.a_pressed = 0;
    g->keys.s_pressed = 0;
    g->keys.d_pressed = 0;
    g->keys.left_pressed = 0;
    g->keys.right_pressed = 0;
    g->prev_mouse_x = win_width / 2; // Start with mouse centered
    
    g->data = data;
    g->img = malloc(sizeof(t_data));
    if (!g->img)
    {
        free(g);
        error_exit_cleanup(ERR_MEMORY_ALLOCATION);
    }
    init_window(data, g->img, win_width, win_height);
    if (init_textures_and_colors(data))
    {
        free(g->img);
        free(g);
        return;
    }
    mlx_put_image_to_window(data->mlx, data->win, g->img->img, 0, 0);
    mlx_hook(data->win, 2, 1L << 0, handle_keypress, g);
    mlx_hook(data->win, 3, 1L << 1, handle_keyrelease, g);
    mlx_hook(data->win, 6, 1L << 6, handle_mouse_move, g);
    mlx_mouse_hide(data->mlx, data->win);
    mlx_mouse_move(data->mlx, data->win, win_width / 2, win_height / 2);
    mlx_hook(data->win, 17, 0L, on_destroy, data);
    mlx_loop_hook(data->mlx, render_frame, g);
    mlx_loop(data->mlx);
    free(g->img);
    free(g);
}