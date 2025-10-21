// 314CA Panaitiu Bogdan-Ionut
#include "struct.h"

int is_rgb(image_data_t *image_data);
int is_grayscale(image_data_t *image_data);
int is_binary(image_data_t *image_data);
int is_ascii(image_data_t *image_data);
void load_binaryimage(image_data_t *image_data);
void load_image(image_data_t *image_data);
void free_image(image_data_t *image_data);
void load_operation(image_data_t *image_data);
void save_binary_image(image_data_t *image_data);
void save_ascii_image(image_data_t *image_data);
void save_operation(image_data_t *image_data);
void select_operation(image_data_t *image_data);
void crop_image(image_data_t *image_data);
void histogram_image(image_data_t *image_data);
void equalize_image(image_data_t *image_data);
void clamp(int *red, int *green, int *blue);
void apply_edge(image_data_t *image_data);
void apply_sharpen(image_data_t *image_data);
void apply_blur(image_data_t *image_data);
void apply_gaussian_blur(image_data_t *image_data);
void apply_operation(image_data_t *image_data);
void invalid_command(void);
void rotate_rgb(image_data_t *image_data, int angle);
void rotate_rgb_full(image_data_t *image_data, int angle);
void rotate_grayscale(image_data_t *image_data, int angle);
void rotate_grayscale_full(image_data_t *image_data, int angle);
void rotate_operation(image_data_t *image_data);
