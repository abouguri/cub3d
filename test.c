#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct s_config {
    char *north_texture;
    char *south_texture;
    char *west_texture;
    char *east_texture;
    int floor_color[3];
    int ceiling_color[3];
    char **map;
} t_config;

// Function prototypes
void parse_file(const char *filename, t_config *config);
void parse_line(const char *line, t_config *config);
void validate_map(char **map);
void free_config(t_config *config);

int main(int argc, char **argv)
{
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <map_file.cub>\n", argv[0]);
        return 1;
    }

    t_config config = {0};
    parse_file(argv[1], &config);

    // Use the parsed configuration
    // ...

    free_config(&config);
    return 0;
}

void parse_file(const char *filename, t_config *config)
{
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(1);
    }

    char *line = NULL;
    size_t len = 0;
    while (getline(&line, &len, file) != -1) {
        parse_line(line, config);
    }
    free(line);
    fclose(file);

    // Validate map after reading the file
    validate_map(config->map);
}

void parse_line(const char *line, t_config *config)
{
    // Implement parsing logic for textures, colors, and map
}

void validate_map(char **map)
{
    // Implement map validation logic
}

void free_config(t_config *config)
{
    free(config->north_texture);
    free(config->south_texture);
    free(config->west_texture);
    free(config->east_texture);
    // Free map memory
}
