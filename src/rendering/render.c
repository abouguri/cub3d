/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 21:09:34 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/17 21:31:53 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_data	*create_frame_image(t_cub *data)
{
	t_data	*img;

	img = malloc(sizeof(t_data));
	if (!img)
		return (NULL);
	img->img = mlx_new_image(data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
	if (!img->img)
	{
		free(img);
		error_exit_cleanup(ERR_CREATE_IMAGE);
		return (NULL);
	}
	img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel,
			&img->line_length, &img->endian);
	if (!img->addr)
	{
		mlx_destroy_image(data->mlx, img->img);
		free(img);
		error_exit_cleanup(ERR_GET_IMAGE_ADDR);
		return (NULL);
	}
	return (img);
}

void	apply_screen_overlay(t_data *img, int factor)
{
	int	i;
	int	j;
	int	color;
	int	darkened;

	i = 0;
	while (i < SCREEN_WIDTH)
	{
		j = 0;
		while (j < SCREEN_HEIGHT)
		{
			color = *(int *)(img->addr + (j * img->line_length + i
						* (img->bits_per_pixel / 8)));
			darkened = (color & ALPHA_MASK) | ((color & RED_MASK)
					/ factor) | ((color & GREEN_MASK)
					/ factor) | ((color & BLUE_MASK) / factor);
			my_mlx_pixel_put(img, i, j, darkened);
			j++;
		}
		i++;
	}
}

void	render_and_display(t_game_state *game, t_data *frame_img)
{
	t_data	*saved_img;

	saved_img = game->img;
	game->img = frame_img;
	raycast(game);
	if (game->game_status == GAME_OVER)
		apply_screen_overlay(frame_img, 3);
	game->img = saved_img;
	mlx_put_image_to_window(game->data->mlx, game->data->win, frame_img->img, 0,
		0);
}

void	display_game_over_text(t_game_state *game)
{
	mlx_string_put(game->data->mlx, game->data->win, SCREEN_WIDTH / 2 - 50,
		SCREEN_HEIGHT / 2 - 20, RED, "GAME OVER");
	mlx_string_put(game->data->mlx, game->data->win, SCREEN_WIDTH / 2 - 70,
		SCREEN_HEIGHT / 2 + 10, WHITE, "Press R to restart");
	mlx_string_put(game->data->mlx, game->data->win, SCREEN_WIDTH / 2 - 70,
		SCREEN_HEIGHT / 2 + 30, WHITE, "Press ESC to exit");
}

int	render_frame(void *param)
{
	t_game_state	*game;
	t_data			*frame_img;

	game = (t_game_state *)param;
	if (game->game_status == GAME_ACTIVE)
		handle_player_movement(game);
	frame_img = create_frame_image(game->data);
	if (!frame_img)
		return (1);
	render_and_display(game, frame_img);
	if (game->game_status == GAME_OVER)
	{
		if (game->game_over_timer > 0)
			game->game_over_timer--;
		display_game_over_text(game);
	}
	mlx_destroy_image(game->data->mlx, frame_img->img);
	free(frame_img);
	return (0);
}
