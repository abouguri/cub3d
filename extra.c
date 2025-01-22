#include "cub3d.h"

typedef struct s_position {
    double x;
    double y;
} t_position;

typedef struct s_direction {
    double x;
    double y;
} t_direction;

typedef struct s_plane {
    double x;
    double y;
} t_plane;

typedef struct s_ray {
    double camera_x;
    double direction_x;
    double direction_y;
    double side_distance_x;
    double side_distance_y;
    double delta_distance_x;
    double delta_distance_y;
    double perspective_wall_distance;
    double wall_x;
} t_ray;


typedef struct s_movement {
    double frame_time;
    double move_speed;
    double rotate_speed;
} t_movement;

typedef struct s_drawing {
    int line_height;
    int draw_start;
    int draw_end;
    unsigned int color;
} t_drawing;

typedef struct s_texture {
    int number;
    int x;
    int y;
    double step;
    double position;
} t_texture;

typedef struct s_map_info {
    int map_x;
    int map_y;
    int step_x;
    int step_y;
    int hit;
    int side;
} t_map_info;

typedef struct s_var {
    t_position position;
    t_direction direction;
    t_plane plane;
    t_ray ray;
    t_movement movement;
    t_drawing drawing;
    t_texture texture;
    t_map_info map_info;
} t_var;

t_cub *get_cub_data(void)
{
    static t_cub data;
    
    return &data;
}

void store_texture_pixels(int i)
{
    int x, y;
    t_cub *data = get_cub_data();

    // Ensure the dimensions do not exceed TEXTURE_HEIGHT and TEXTURE_WIDTH
    int max_height = (data->img2[i]->height > TEXTURE_HEIGHT) ? TEXTURE_HEIGHT : data->img2[i]->height;
    int max_width = (data->img2[i]->width > TEXTURE_WIDTH) ? TEXTURE_WIDTH : data->img2[i]->width;
    y = -1;
    while (++y < max_height)
    {
        x = -1;
        while (++x < max_width)
        {
            // Write pixel data into texture array
            data->texture[i][y * TEXTURE_WIDTH + x] = data->img2[i]->data_addr[y * data->img2[i]->width + x];
        }
    }

}


int init_textures(void)
{
    int     i;
    void    *temporary;
    t_cub   *data = get_cub_data();

    i = 0;
    while (i < 4)
    {
        // Allocate memory for img2[i]
        data->img2[i] = malloc(sizeof(t_img2));
        temporary = mlx_xpm_file_to_image(
            data->mlx, data->textures[i], &(data->img2[i]->width), &(data->img2[i]->height)
        );

        
        data->img2[i]->img_ptr = temporary;

        // Get the pixel data address
        data->img2[i]->data_addr = (int *)mlx_get_data_addr(
            data->img2[i]->img_ptr, &data->img2[i]->bpp, 
            &data->img2[i]->line_size, &data->img2[i]->endian
        );

        // Populate texture pixel array
        store_texture_pixels(i);

        // Destroy the MiniLibX image after storing pixel data
        mlx_destroy_image(data->mlx, data->img2[i]->img_ptr);
        i++;
    }

    return (0);
}


unsigned long	rgb_to_hex(int red, int green, int blue)
{
	return (((red & 0xff) << 16) + ((green & 0xff) << 8) + (blue & 0xff));
}

void	store_color(int *rgb, int i)
{
	unsigned long	temporary;
    t_cub *data = get_cub_data();

	if (i == 0)
	{
		temporary = rgb_to_hex(rgb[0], rgb[1], rgb[2]);
		data->floor = temporary;
	}
	if (i == 1)
	{
		temporary = rgb_to_hex(rgb[0], rgb[1], rgb[2]);
		data->ceilling = temporary;
	}
}

int	*is_rgb_valid(char **array)
{
	int	i;
	int	*rgb;

	i = 0;
	rgb = malloc(sizeof(int) * 4);
	if (!rgb)
		return (NULL);
	while (array[i])
	{
		rgb[i] = atoi(array[i]);
		if (rgb[i] < 0 || rgb[i] > 255)
			return (NULL);
		i++;
	}
	rgb[i] = 0;
	return (rgb);
}

int	init_colors(void)
{
	int				i;
	char			**temporary;
	int				*rgb;
    t_cub *data = get_cub_data();

	i = 0;
	while (data->colors[i])
	{
		temporary = ft_split(data->colors[i], ',');
		if (!temporary)
			return (1);
		rgb = is_rgb_valid(temporary);
		if (!rgb)
		{
			free(rgb);
			return (1);
		}
		store_color(rgb, i);
		free_array(&temporary);
		free(rgb);
		i++;
	}
	return (0);
}