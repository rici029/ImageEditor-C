// 314CA Panaitiu Bogdan-Ionut
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "struct.h"

int main(void)
{
	image_data_t image_data;
	image_data.ok = 0;
	while (1) {
		// Citire comanda
		scanf("%s", image_data.comanda);
		// Verificare comanda
		if (strcmp(image_data.comanda, "LOAD") == 0) {
			if (image_data.ok == 0) {
				image_data.ok = 1;
				load_operation(&image_data);
			} else {
				free_image(&image_data);
				load_operation(&image_data);
			}
		} else {
			if (strcmp(image_data.comanda, "SAVE") == 0) {
				save_operation(&image_data);
			} else if (strcmp(image_data.comanda, "SELECT") == 0) {
				select_operation(&image_data);
			} else if (strcmp(image_data.comanda, "CROP") == 0) {
				crop_image(&image_data);
			} else if (strcmp(image_data.comanda, "HISTOGRAM") == 0) {
				histogram_image(&image_data);
			} else if (strcmp(image_data.comanda, "EQUALIZE") == 0) {
				equalize_image(&image_data);
			} else if (strcmp(image_data.comanda, "APPLY") == 0) {
				apply_operation(&image_data);
			} else if (strcmp(image_data.comanda, "ROTATE") == 0) {
				rotate_operation(&image_data);
			} else if (strcmp(image_data.comanda, "EXIT") == 0) {
				if (!image_data.input_file)
					printf("No image loaded\n");
						break;
			} else {
				invalid_command();
			}
		}
	}
	if (image_data.ok == 1)
		free_image(&image_data);
	return 0;
}
