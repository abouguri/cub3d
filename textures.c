/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 21:03:47 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/04 02:11:55 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

/*
** Load a texture from file
*/
// int	load_texture(t_cub *data, int index, char *path)
// {
// 	t_img2	img;
// 	int		x;
// 	int		y;
// 	img.img_ptr = mlx_xpm_file_to_image(data->mlx, path,
// 			&img.width, &img.height);
// 	if (!img.img_ptr)
// 		return (1);
// 	if (img.width != TEXTURE_WIDTH || img.height != TEXTURE_HEIGHT)
// 	{
//     	mlx_destroy_image(data->mlx, img.img_ptr);
//     	return (1);
// 	}
// 	img.data_addr = (int *)mlx_get_data_addr(img.img_ptr, &img.bpp,
// 			&img.line_size, &img.endian);
// 	y = 0;
// 	while (y < img.height)
// 	{
// 		x = 0;
// 		while (x < img.width)
// 		{
// 			data->texture[index][img.width * y + x] = img.data_addr[img.width * y + x];
// 			x++;
// 		}
// 		y++;
// 	}
// 	mlx_destroy_image(data->mlx, img.img_ptr);
// 	return (0);
// }

int load_texture(t_cub *data, int index, char *path)
{
    t_img2 img;
    int x, y;
    float x_ratio, y_ratio;
    
    img.img_ptr = mlx_xpm_file_to_image(data->mlx, path, &img.width, &img.height);
    if (!img.img_ptr)
        return (1);
    
    img.data_addr = (int *)mlx_get_data_addr(img.img_ptr, &img.bpp, &img.line_size, &img.endian);
    
    // Calculate scaling ratios
    x_ratio = (float)img.width / TEXTURE_WIDTH;
    y_ratio = (float)img.height / TEXTURE_HEIGHT;
    
	printf("Original Texture Size: %d x %d\n", img.width, img.height);
	printf("Scaling Ratios: x_ratio = %f, y_ratio = %f\n", x_ratio, y_ratio);

    // Scale down the texture
    for (y = 0; y < TEXTURE_HEIGHT; y++)
    {
        for (x = 0; x < TEXTURE_WIDTH; x++)
        {
            // Calculate source coordinates
            // int src_x = (int)(x * x_ratio);
            // int src_y = (int)(y * y_ratio);
			int src_x = round(x * x_ratio);
			int src_y = round(y * y_ratio);

            
            // Copy pixel with scaling
            data->texture[index][TEXTURE_WIDTH * y + x] = 
                img.data_addr[img.width * src_y + src_x];
        }
    }
    
    mlx_destroy_image(data->mlx, img.img_ptr);
    return (0);
}

// int load_texture(t_cub *data, int index, char *path)
// {
//     t_img2 img;
//     int x, y;
    
//     // Load texture
//     img.img_ptr = mlx_xpm_file_to_image(data->mlx, path, &img.width, &img.height);
//     if (!img.img_ptr)
//         return (1);
    
//     img.data_addr = (int *)mlx_get_data_addr(img.img_ptr, &img.bpp, &img.line_size, &img.endian);
    
//     // ✅ Ensure the texture is 64x64 (or expected size)
//     if (img.width != TEXTURE_WIDTH || img.height != TEXTURE_HEIGHT)
//     {
//         printf("WARNING: Texture %d has unexpected size %d x %d\n", index, img.width, img.height);
//     }

//     // ✅ Corrected Texture Copying (No Scaling)
//     for (y = 0; y < TEXTURE_HEIGHT; y++)
//     {
//         for (x = 0; x < TEXTURE_WIDTH; x++)
//         {
//             data->texture[index][TEXTURE_WIDTH * y + x] = 
//                 img.data_addr[img.width * y + x]; // Direct pixel copy
//         }
//     }
    
//     mlx_destroy_image(data->mlx, img.img_ptr);
//     return (0);
// }



/*
** Convert RGB string to color value
*/
int	parse_rgb(char *str)
{
	char			**rgb_values;
	int				r;
	int				g;
	int				b;
	int				color;

	rgb_values = ft_split(str, ',');
	if (!rgb_values || ft_array_length(rgb_values) != 3)
	{
		free_array(&rgb_values);
		return (-1);
	}
	r = atoi(rgb_values[0]);
	g = atoi(rgb_values[1]);
	b = atoi(rgb_values[2]);
	free_array(&rgb_values);
	if (r < 0 || r > 255 || g < 0 || g > 255 || b < 0 || b > 255)
		return (-1);
	color = (r << 16) | (g << 8) | b;
	return (color);
}
/*
** Initialize all textures and colors
*/
int	init_textures_and_colors(t_cub *data)
{
	int	i;

	i = 0;
	while (i < TEXTURE_COUNT)
	{
		if (load_texture(data, i, data->textures[i]))
			return (1);
		i++;
	}

	data->floor = parse_rgb(data->colors[0]);
	data->ceilling = parse_rgb(data->colors[1]);
	if (data->floor == -1 || data->ceilling == -1)
		return (1);
	return (0);
}