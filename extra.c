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

