/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   render.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 21:09:34 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/05 19:59:22 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"


// Function to draw a filled rectangle
void    draw_rectangle(t_data *img, int x, int y, int width, int height, int color)
{
    int i;
    int j;

    i = x;
    while (i < x + width)
    {
        j = y;
        while (j < y + height)
        {
            my_mlx_pixel_put(img, i, j, color);
            j++;
        }
        i++;
    }
}

// Function to draw a line between two points
void    draw_line(t_data *img, int x1, int y1, int x2, int y2, int color)
{
    double  dx;
    double  dy;
    double  step;
    double  x;
    double  y;
    int     i;

    dx = x2 - x1;
    dy = y2 - y1;
    step = (fabs(dx) > fabs(dy)) ? fabs(dx) : fabs(dy);
    dx = dx / step;
    dy = dy / step;
    x = x1;
    y = y1;
    i = 0;
    while (i <= step)
    {
        my_mlx_pixel_put(img, (int)x, (int)y, color);
        x += dx;
        y += dy;
        i++;
    }
}

// Function to draw the grid
// void    draw_grid(t_data *img, int map_width, int map_height, int cell_size)
// {
//     int     i;
//     int     grid_color;

//     grid_color = 0x444444; // Dark gray color for grid lines
    
//     // Draw horizontal grid lines
//     i = 0;
//     while (i <= map_height)
//     {
//         draw_line(img, 0, i * cell_size, map_width * cell_size, i * cell_size, grid_color);
//         i++;
//     }
    
//     // Draw vertical grid lines
//     i = 0;
//     while (i <= map_width)
//     {
//         draw_line(img, i * cell_size, 0, i * cell_size, map_height * cell_size, grid_color);
//         i++;
//     }
// }

// Function to set initial player direction based on map character
void    set_player_direction(t_cub *data, char dir_char)
{
    // Initialize direction vectors based on the character in the map
    if (dir_char == 'N')
    {
        data->var.direction_x = 0;
        data->var.direction_y = -1;
        data->var.plane_x = 0.66;
        data->var.plane_y = 0;
    }
    else if (dir_char == 'S')
    {
        data->var.direction_x = 0;
        data->var.direction_y = 1;
        data->var.plane_x = -0.66;
        data->var.plane_y = 0;
    }
    else if (dir_char == 'E')
    {
        data->var.direction_x = 1;
        data->var.direction_y = 0;
        data->var.plane_x = 0;
        data->var.plane_y = 0.66;
    }
    else if (dir_char == 'W')
    {
        data->var.direction_x = -1;
        data->var.direction_y = 0;
        data->var.plane_x = 0;
        data->var.plane_y = -0.66;
    }
}
char	*ft_strchr(const char *s, int c)
{
	while ((*s != '\0') && (*s != c))
	{
		s++;
	}
	if (*s == c)
	{
		return ((char *)s);
	}
	return ((char*)NULL);
}

// Function to render map cells
// void    render_map_cells(t_data *img, char **map, int cell_size)
// {
//     int     i;
//     int     j;
//     int     wall_color;
//     int     floor_color;
//     int     player_color;
//     t_cub   *data;
    
//     data = get_cub_data();
//     wall_color = 0x808080;  // Gray color for walls
//     floor_color = 0x303030; // Dark color for floor
//     player_color = 0xFF0000; // Red color for player
    
//     i = 0;
//     while (map[i])
//     {
//         j = 0;
//         while (map[i][j])
//         {
//             if (map[i][j] == '1')
//                 draw_rectangle(img, j * cell_size, i * cell_size, cell_size, cell_size, wall_color);
//             else if (map[i][j] == '0' || ft_strchr("NSEW", map[i][j]))
//                 draw_rectangle(img, j * cell_size, i * cell_size, cell_size, cell_size, floor_color);
            
//             // Check if this cell has the player
//             if (ft_strchr("NSEW", map[i][j]))
//             {
//                 // Set player position in the middle of the cell
//                 data->var.position_x = j + 0.5;
//                 data->var.position_y = i + 0.5;
                
//                 // Set player direction based on character
//                 set_player_direction(data, map[i][j]);
                
//                 // Draw player as a circle in 2D map
//                 int center_x = j * cell_size + cell_size / 2;
//                 int center_y = i * cell_size + cell_size / 2;
//                 int radius = cell_size / 4;
                
//                 // Simple circle drawing
//                 for (int y = -radius; y <= radius; y++)
//                 {
//                     for (int x = -radius; x <= radius; x++)
//                     {
//                         if (x*x + y*y <= radius*radius)
//                             my_mlx_pixel_put(img, center_x + x, center_y + y, player_color);
//                     }
//                 }
                
//                 // Draw direction line
//                 draw_line(img, center_x, center_y, 
//                           center_x + data->var.direction_x * radius * 2, 
//                           center_y + data->var.direction_y * radius * 2, 
//                           0xFFFF00);
//             }
            
//             j++;
//         }
//         i++;
//     }
// }

// void render_map_to_image(t_data *img, char **map, int cell_size)
// {
//     int map_width = ft_max_length(map);
//     int map_height = ft_array_length(map);

//     // Clear image first with black color
//     draw_rectangle(img, 0, 0, map_width * cell_size, map_height * cell_size, 0x000000);

//     // Render cells and player
//     render_map_cells(img, map, cell_size);

//     // Grid
//     draw_grid(img, map_width, map_height, cell_size);
// }

// Function for raycasting rendering
void    render_raycasting(t_data *img, t_cub *data)
{
    int x;
    double camera_x;
    double ray_dir_x;
    double ray_dir_y;
    t_dda dda;
    t_render render;
    int window_width = ft_max_length(data->map) * CELL_SIZE;
    int window_height = ft_array_length(data->map) * CELL_SIZE;

    // Clear the screen - draw ceiling and floor
    draw_rectangle(img, 0, 0, window_width, window_height / 2, data->ceilling);
    draw_rectangle(img, 0, window_height / 2, window_width, window_height / 2, data->floor);
    
    x = 0;
    while (x < window_width)
    {
        // Calculate ray position and direction
        camera_x = 2 * x / (double)window_width - 1; // x-coordinate in camera space
        ray_dir_x = data->var.direction_x + data->var.plane_x * camera_x;
        ray_dir_y = data->var.direction_y + data->var.plane_y * camera_x;
        
        // Which box of the map we're in
        dda.map_x = (int)data->var.position_x;
        dda.map_y = (int)data->var.position_y;
        
        // Length of ray from current position to next x or y-side
        dda.delta_dist_x = (ray_dir_x == 0) ? 1e30 : fabs(1 / ray_dir_x);
        dda.delta_dist_y = (ray_dir_y == 0) ? 1e30 : fabs(1 / ray_dir_y);
        
        // Calculate step and initial sideDist
        if (ray_dir_x < 0)
        {
            dda.step_x = -1;
            dda.side_dist_x = (data->var.position_x - dda.map_x) * dda.delta_dist_x;
        }
        else
        {
            dda.step_x = 1;
            dda.side_dist_x = (dda.map_x + 1.0 - data->var.position_x) * dda.delta_dist_x;
        }
        
        if (ray_dir_y < 0)
        {
            dda.step_y = -1;
            dda.side_dist_y = (data->var.position_y - dda.map_y) * dda.delta_dist_y;
        }
        else
        {
            dda.step_y = 1;
            dda.side_dist_y = (dda.map_y + 1.0 - data->var.position_y) * dda.delta_dist_y;
        }
        
        // Perform DDA
        dda.hit = 0;
        while (dda.hit == 0)
        {
            // Jump to next map square, either in x-direction, or in y-direction
            if (dda.side_dist_x < dda.side_dist_y)
            {
                dda.side_dist_x += dda.delta_dist_x;
                dda.map_x += dda.step_x;
                dda.side = 0;
            }
            else
            {
                dda.side_dist_y += dda.delta_dist_y;
                dda.map_y += dda.step_y;
                dda.side = 1;
            }
            
            // Check if ray has hit a wall
            if (dda.map_y >= 0 && dda.map_y < ft_array_length(data->map) &&
                dda.map_x >= 0 && dda.map_x < (int)strlen(data->map[dda.map_y]) &&
                data->map[dda.map_y][dda.map_x] == '1')
                dda.hit = 1;
        }
        
        
        if (dda.side == 0)
            render.perp_wall_dist = (dda.map_x - data->var.position_x + (1 - dda.step_x) / 2) / fabs(ray_dir_x);
        else
            render.perp_wall_dist = (dda.map_y - data->var.position_y + (1 - dda.step_y) / 2) / fabs(ray_dir_y);

        if (render.perp_wall_dist < 0.0001) 
            render.perp_wall_dist = 0.0001;  // Prevent near-zero values


        // Calculate height of line to draw on screen
        render.line_height = (int)(window_height / render.perp_wall_dist);
        
        // Calculate lowest and highest pixel to fill in current stripe
        render.draw_start = -render.line_height / 2 + window_height / 2;
        if (render.draw_start < 0)
            render.draw_start = 0;
        render.draw_end = render.line_height / 2 + window_height / 2;
        if (render.draw_end >= window_height)
            render.draw_end = window_height - 1;
        
        // Choose wall color based on which side was hit
        int wall_color;
        if (dda.side == 0)
            wall_color = 0xFF0000; // Red for x-side
        else
            wall_color = 0xCC0000; // Darker red for y-side
        
        // Draw the vertical line
        int y = render.draw_start;
        while (y < render.draw_end)
        {
            my_mlx_pixel_put(img, x, y, wall_color);
            y++;
        }
        
        x++;
    }
}

// void init_window(t_cub *data, t_data *img, int win_width, int win_height)
// {
//     printf("Debug: Calling mlx_init()...\n");
//     data->mlx = mlx_init();
//     if (!data->mlx)
//     {
//         printf("Error: Failed to initialize mlx.\n");
//         error_exit_cleanup(ERR_INIT_MLX);
//     }
//     printf("Debug: mlx initialized, pointer = %p\n", data->mlx);

//     data->win = mlx_new_window(data->mlx, win_width, win_height, "cub3D");
//     printf("Debug: Window created, pointer = %p\n", data->win);

//     img->img = mlx_new_image(data->mlx, win_width, win_height);
//     printf("Debug: Image created, pointer = %p\n", img->img);
    
//     img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, &img->line_length, &img->endian);
//     printf("Debug: Image address = %p\n", img->addr);
// }


void init_window(t_cub *data, t_data *img, int win_width, int win_height)
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

    img->addr = mlx_get_data_addr(img->img, &img->bits_per_pixel, &img->line_length, &img->endian);
    if (!img->addr)
        error_exit_cleanup(ERR_GET_IMAGE_ADDR);
}

void init(void)
{
    t_cub *data = get_cub_data();
    t_game_state *g;

    int win_height = ft_array_length(data->map) * CELL_SIZE;
    int win_width = ft_max_length(data->map) * CELL_SIZE;

    g = malloc(sizeof(t_game_state));
    if (!g)
        error_exit_cleanup(ERR_MEMORY_ALLOCATION);
    
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
        printf("Error: Failed to initialize textures and colors.\n");
        // Clean up before returning
        free(g->img);
        free(g);
        return;
    }
    
    render_raycasting(g->img, data);
    mlx_put_image_to_window(data->mlx, data->win, g->img->img, 0, 0);
    
    mlx_hook(data->win, 2, 1L << 0, handle_keypress, g);
    mlx_hook(data->win, 17, 0L, on_destroy, data);
    mlx_loop_hook(data->mlx, render_frame, g);
    
    mlx_loop(data->mlx);
    
    // The code below will only execute if mlx_loop_end is called
    // but on_destroy already handles cleanup and calls exit(0),
    // so this code won't actually execute
    free(g->img);
    free(g);
}

// void init(void)
// {
//     t_cub *data = get_cub_data();
//     t_game_state *g;

//     printf("Debug: Starting initialization...\n");

//     int win_height = ft_array_length(data->map) * CELL_SIZE;
//     int win_width = ft_max_length(data->map) * CELL_SIZE;

//     g = malloc(sizeof(t_game_state));
//     if (!g)
//         error_exit_cleanup(ERR_MEMORY_ALLOCATION);

//     g->data = data;
//     g->img = malloc(sizeof(t_data));
//     if (!g->img)
//     {
//         free(g);
//         error_exit_cleanup(ERR_MEMORY_ALLOCATION);
//     }
//     init_window(data, g->img, win_width, win_height);

//     // **Initialize textures and colors before rendering**
//     if (init_textures_and_colors(data))
//     {
//         printf("Error: Failed to initialize textures and colors.\n");
//         free(g->img);
//         free(g);
//         return;
//     }

//     printf("Debug: Ceiling Color = 0x%X, Floor Color = 0x%X\n",
//        (unsigned int)data->ceilling, (unsigned int)data->floor);


    
//     printf("Debug: Window initialized successfully.\n");

//     render_raycasting(g->img, data);
//     mlx_put_image_to_window(data->mlx, data->win, g->img->img, 0, 0);

//     // Hook up key press handling
//     mlx_hook(data->win, 2, 1L << 0, handle_keypress, g);
//     mlx_hook(data->win, 17, 0L, on_destroy, data);
//     mlx_loop_hook(data->mlx, render_frame, g);
    
//     printf("Debug: Entering mlx_loop()...\n");
//     mlx_loop(data->mlx);

//     // Cleanup
//     free(g->img);
//     free(g);
// }
