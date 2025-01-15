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

static void	initialize_camera(double direction_x, double direction_y, double pla_x, double pla_y)
{
    t_cub *data = get_cub_data();

	data->var.direction.x = direction_x;
	data->var.direction.y = direction_y;
	data->var.plane.x = pla_x;
	data->var.plane.y = pla_y;
}

// void	init(void)
// {
// 	void	*temporary;

//     t_cub *data = get_cub_data();
// 	data->mlx = mlx_init();
// 	// if (init_textures() == 1)
// 	// 	ft_exit_init("Error\nCannot load textures\n");
// 	// if (init_colours() == 1)
// 	// 	ft_exit_init("Error\nCannot load colors\n");
// 	if (validate_full_map() == 1)
// 		ft_exit_init("Error\nInvalid map\n");
// 	temporary = mlx_new_window(data->mlx, SCREEN_WIDTH, SCREEN_HEIGHT, "cub3D");
// 	data->win = temporary;
// }

static void	initialize_player_vectors(int x, int y)
{
    t_cub *data = get_cub_data();

	data->var.position.x = x + 0.5;
	data->var.position.y = y + 0.5;
	if (data->map[y][x] == 'N')
		initialize_camera(0, -1, 0.66, 0);
	else if (data->map[y][x] == 'S')
		initialize_camera(0, 1, -0.66, 0);
	else if (data->map[y][x] == 'E')
		initialize_camera(1, 0, 0, 0.66);
	else if (data->map[y][x] == 'W')
		initialize_camera(-1, 0, 0, -0.66);
}

int	validate_full_map_characters(void)
{
	int	i;
	int	j;
	int	count;
    
    t_cub *data = get_cub_data();
	i = 0;
	count = 0;
	while (data->map[i])
	{
		j = 0;
		while (data->map[i][j])
		{
			if (!strchr(" 10NSEW", data->map[i][j]))
				return (0);
			if (data->map[i][j] == 'N' || data->map[i][j] == 'S' || data->map[i][j] == 'E' || data->map[i][j] == 'W')
			{
				initialize_player_vectors(j, i);
				count++;
			}
			j++;
		}
		i++;
	}
	return (count);
}

static int	is_cell_enclosed(int i, int j)
{
    t_cub *data = get_cub_data();

	if (data->map[i][j] == '0' || (data->map[i][j] != '1' && data->map[i][j] != ' '))
	{
		if (i == 0 || !data->map[i + 1] || j == 0 || !data->map[i][j + 1])
			return (1);
		if (data->map[i - 1] && data->map[i - 1][j] && data->map[i - 1][j] == ' ')
			return (1);
		if (data->map[i + 1] && data->map[i + 1][j] && data->map[i + 1][j] == ' ')
			return (1);
		if (data->map[i] && data->map[i][j - 1] && data->map[i][j - 1] == ' ')
			return (1);
		if (data->map[i] && data->map[i][j + 1] && data->map[i][j + 1] == ' ')
			return (1);
	}
	return (0);
}

static int	validate_full_map_end(int index)
{
	int		i;

    t_cub *data = get_cub_data();
	i = index;
	i++;
	while (data->map[i])
	{
		if (strlen(data->map[i]) > 0)
			return (1);
		i++;
	}
	data->map[i] = NULL;
	return (0);
}

int	validate_full_map(void)
{
	int	i;
	int	j;
    
    t_cub *data = get_cub_data();
	if (ft_array_length(data->map) < 3 || validate_full_map_characters() != 1)
		return (1);
	i = 0;
	while (data->map[i])
	{
		if (strlen(data->map[i]) == 0)
		{
			if (validate_full_map_end(i) == 1)
				return (1);
			break ;
		}
		j = 0;
		while (data->map[i][j])
		{
			if (is_cell_enclosed(i, j) == 1)
				return (1);
			j++;
		}
		i++;
	}
	return (0);
}
