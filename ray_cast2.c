/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ray_cast2.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 19:39:34 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/03 02:50:19 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */


#include "cub3d.h"







int	exitbyx(int keycode, t_data *data)
{
    (void)data;
    printf("keycode |%d|\n", keycode);
    if (keycode == 17)
        {
            printf("laaah\n");
            return 1;
            // exit(1);
        }
	return (0);
}





// static int get_cell_color(char cell)
// {
//     if (cell == '0') // Floor
//         return (WHITE); // White
//     if (cell == '1') // Wall
//         return (BLUE); // Blue
//     return (0); // Default color for invalid cells
// }

// void draw_grid(t_data *img, int map_width, int map_height, int cell_size)
// {
//     int x;
//     int y;
    
//     x = 0;
//     // Vertical
//     while (x <= map_width)
//     {
//         y = 0;
//         while (y < map_height * cell_size)
//         {
//             my_mlx_pixel_put(img, x * cell_size, y, BLACK);
//             y++;
//         }
//         x++;
//     }
//     // Horizontal
//     y = 0;
//     while (y <= map_height)
//     {
//         x = 0;
//         while (x < map_width * cell_size)
//         {
//             my_mlx_pixel_put(img, x, y * cell_size, BLACK);
//             x++;
//         }
//         y++;
//     }
// }

// static void draw_circle(t_data *img, int center_x, int center_y, int radius, int color)
// {
//     int y = -radius;

//     while (y <= radius)
//     {
//         int x = -radius;
//         while (x <= radius)
//         {
//             if (x * x + y * y <= radius * radius) // Check if the pixel is within the circle
//             {
//                 my_mlx_pixel_put(img, center_x + x, center_y + y, color);
//             }
//             x++;
//         }
//         y++;
//     }
// }

// static void draw_cell(t_data *img, int x, int y, int cell_size, int color)
// {
//     int p_x = 0;

//     while (p_x < cell_size)
//     {
//         int p_y = 0;
//         while (p_y < cell_size)
//         {
//             int pixel_x = p_x + x * cell_size;
//             int pixel_y = p_y + y * cell_size;
//             my_mlx_pixel_put(img, pixel_x, pixel_y, color);
//             p_y++;
//         }
//         p_x++;
//     }
// }

// static void draw_player(t_data *img, int x, int y, int cell_size)
// {
//     draw_cell(img, x, y, cell_size, WHITE);

//     int center_x = x * cell_size + cell_size / 2;
//     int center_y = y * cell_size + cell_size / 2;
//     draw_circle(img, center_x, center_y, cell_size / 4, GREEN);
// }

// void render_map_cells(t_data *img, char **map, int cell_size)
// {
//     int y = 0;

//     while (y < ft_array_length(map))
//     {
//         int x = 0;
//         while (x < (int)strlen(map[y]))
//         {
//             int color = get_cell_color(map[y][x]);

//             draw_cell(img, x, y, cell_size, color);

//             if (map[y][x] == 'N' || map[y][x] == 'S' || map[y][x] == 'E' || map[y][x] == 'W')
//             {
//                 draw_player(img, x, y, cell_size);
//             }
//             x++;
//         }
//         y++;
//     }
// }




