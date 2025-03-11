/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 21:09:34 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/11 06:20:21 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

t_data *create_frame_image(t_cub *data)
{
    t_data *img;
    
    img = malloc(sizeof(t_data));
    if (!img)
        return NULL;
    
    img->img = mlx_new_image(data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT);
    if (!img->img)
    {
        free(img);
        error_exit_cleanup(ERR_CREATE_IMAGE);
        return NULL;
    }
    
    img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, 
                                &img->line_length, &img->endian);
    if (!img->addr)
    {
        mlx_destroy_image(data->mlx, img->img);
        free(img);
        error_exit_cleanup(ERR_GET_IMAGE_ADDR);
        return NULL;
    }
    
    return img;
}

void render_and_display(t_game_state *game, t_data *frame_img)
{
    t_data *saved_img;
    
    saved_img = game->img;
    game->img = frame_img;
    raycast(game);
    game->img = saved_img;
    mlx_put_image_to_window(game->data->mlx, game->data->win, frame_img->img, 0, 0);
}

int render_frame(void *param)
{
    t_game_state *game;
    t_data *frame_img;
    
    game = (t_game_state *)param;
    handle_player_movement(game);
    frame_img = create_frame_image(game->data);
    if (!frame_img)
        return (1);
    render_and_display(game, frame_img);
    mlx_destroy_image(game->data->mlx, frame_img->img);
    free(frame_img);
    return (0);
}
