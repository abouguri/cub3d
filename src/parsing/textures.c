/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   textures.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: abouguri <abouguri@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/03/02 21:03:47 by abouguri          #+#    #+#             */
/*   Updated: 2025/03/20 04:26:59 by abouguri         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "cub3d.h"

int	load_image(t_cub *data, t_img2 *img, char *path)
{
	img->img_ptr = mlx_xpm_file_to_image(data->mlx, path, &img->width,
			&img->height);
	if (!img->img_ptr)
		return (1);
	img->data_addr = (int *)mlx_get_data_addr(img->img_ptr, &img->bpp,
			&img->line_size, &img->endian);
	return (0);
}

void	resize_texture(int *texture, t_img2 *img, int x, int y)
{
	float	x_ratio;
	float	y_ratio;
	int		src_x;
	int		src_y;

	x_ratio = (float)img->width / TEXTURE_WIDTH;
	y_ratio = (float)img->height / TEXTURE_HEIGHT;
	y = 0;
	while (y < TEXTURE_HEIGHT)
	{
		x = 0;
		while (x < TEXTURE_WIDTH)
		{
			src_x = round(x * x_ratio);
			src_y = round(y * y_ratio);
			texture[TEXTURE_WIDTH * y + x] = img->data_addr[img->width * src_y
				+ src_x];
			x++;
		}
		y++;
	}
}

int	load_texture(t_cub *data, int index, char *path)
{
	t_img2	img;
	int		x;
	int		y;

	x = 0;
	y = 0;
	if (load_image(data, &img, path))
		return (1);
	resize_texture(data->texture[index], &img, x, y);
	mlx_destroy_image(data->mlx, img.img_ptr);
	return (0);
}

int	parse_rgb(char *str)
{
	char	**rgb_values;
	int		r;
	int		g;
	int		b;
	int		color;

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
	{
		ft_putstr("Error\nColors are not valid\n");
		return (-1);
	}
	color = (r << 16) | (g << 8) | b;
	return (color);
}

int	init_textures_and_colors(t_cub *data)
{
	int	i;

	i = 0;
	while (i < TEXTURE_COUNT)
	{
		if (load_texture(data, i, data->textures[i]))
		{
			printf("Error\nTextures are not valid\n");
			return (1);
		}
		i++;
	}
	data->floor = parse_rgb(data->colors[0]);
	data->ceilling = parse_rgb(data->colors[1]);
	if (data->floor == -1 || data->ceilling == -1)
		return (1);
	return (0);
}
