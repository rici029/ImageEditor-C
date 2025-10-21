// 314CA Panaitiu Bogdan-Ionut
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "util.h"
#include "struct.h"

int is_rgb(image_data_t *image_data)
{
	if (strcmp(image_data->magic_word, "P6") == 0)
		return 1;
	if (strcmp(image_data->magic_word, "P3") == 0)
		return 1;
	return 0;
}

int is_grayscale(image_data_t *image_data)
{
	if (strcmp(image_data->magic_word, "P5") == 0)
		return 1;
	if (strcmp(image_data->magic_word, "P2") == 0)
		return 1;
	return 0;
}

int is_binary(image_data_t *image_data)
{
	if (strcmp(image_data->magic_word, "P6") == 0)
		return 1;
	if (strcmp(image_data->magic_word, "P5") == 0)
		return 1;
	return 0;
}

int is_ascii(image_data_t *image_data)
{
	if (strcmp(image_data->magic_word, "P3") == 0)
		return 1;
	if (strcmp(image_data->magic_word, "P2") == 0)
		return 1;
	return 0;
}

// Functia de incarcare a imaginii binare
void load_binaryimage(image_data_t *image_data)
{
	char buffer[1024];
	image_data->input_file = fopen(image_data->input_filename, "rb");
	if (!image_data->input_file) {
		printf("Error opening file.\n");
		return;
	}
	// Citim tipul imaginii
	fread(image_data->magic_word, 1, 3, image_data->input_file);
	image_data->magic_word[2] = '\0';
	// Citim dimensiunile imaginii
	fscanf(image_data->input_file, "%d", &image_data->width);
	fscanf(image_data->input_file, "%d", &image_data->height);
	fscanf(image_data->input_file, "%hhu", &image_data->max_value);
	fread(&buffer, sizeof(char), 1, image_data->input_file);
	if (strcmp(image_data->magic_word, "P6") == 0) {
		// Alocam memorie pentru pixelii imaginii RGB
		image_data->rgb_image = malloc(image_data->height *
									   sizeof(rgb_image_t *));
		for (int i = 0; i < image_data->height; i++)
			image_data->rgb_image[i] = malloc(image_data->width *
											  sizeof(rgb_image_t));

		// Citim pixelii imaginii RGB
		for (int i = 0; i < image_data->height; i++) {
			for (int j = 0; j < image_data->width; j++) {
				fread(&image_data->rgb_image[i][j].red,
					  sizeof(unsigned char), 1, image_data->input_file);
				fread(&image_data->rgb_image[i][j].green,
					  sizeof(unsigned char), 1, image_data->input_file);
				fread(&image_data->rgb_image[i][j].blue,
					  sizeof(unsigned char), 1, image_data->input_file);
			}
		}
	} else if (strcmp(image_data->magic_word, "P5") == 0) {
		// Aloca memorie pentru pixelii imaginii grayscale
		image_data->grayscale = malloc(image_data->height
									   * sizeof(unsigned char *));
		for (int i = 0; i < image_data->height; i++)
			image_data->grayscale[i] = malloc(image_data->width
											  * sizeof(unsigned char));

		// Citim pixelii imaginii grayscale
		for (int i = 0; i < image_data->height; i++)
			for (int j = 0; j < image_data->width; j++) {
				fread(&image_data->grayscale[i][j],
					  sizeof(unsigned char), 1, image_data->input_file);
			}
	}

	fclose(image_data->input_file);
}

// Functia de incarcare a imaginii ASCII
void load_image(image_data_t *image_data)
{
	image_data->input_file = fopen(image_data->input_filename, "r");
	if (!image_data->input_file) {
		printf("Error opening file.\n");
		return;
	}
	// Citim tipul imaginii
	fread(image_data->magic_word, 1, 3, image_data->input_file);
	image_data->magic_word[2] = '\0';
	if (strcmp(image_data->magic_word, "P3") == 0) {
		// Citim dimensiunile imaginii
		fscanf(image_data->input_file, "%d", &image_data->width);
		fscanf(image_data->input_file, "%d", &image_data->height);
		fscanf(image_data->input_file, "%hhu", &image_data->max_value);
		// Alocam memorie pentru pixelii imaginii RGB
		image_data->rgb_image = malloc(image_data->height
									   * sizeof(rgb_image_t *));
		for (int i = 0; i < image_data->height; i++) {
			image_data->rgb_image[i] = malloc(image_data->width
											  * sizeof(rgb_image_t));
		}
		// Citim pixelii imaginii RGB
		for (int i = 0; i < image_data->height; i++) {
			for (int j = 0; j < image_data->width; j++) {
				fscanf(image_data->input_file,
					   "%hhu", &image_data->rgb_image[i][j].red);
				fscanf(image_data->input_file,
					   "%hhu", &image_data->rgb_image[i][j].green);
				fscanf(image_data->input_file,
					   "%hhu", &image_data->rgb_image[i][j].blue);
			}
		}
	} else if (strcmp(image_data->magic_word, "P2") == 0) {
		// Citim dimensiunile imaginii
		fscanf(image_data->input_file, "%d", &image_data->width);
		fscanf(image_data->input_file, "%d", &image_data->height);
		fscanf(image_data->input_file, "%hhu", &image_data->max_value);

		// Alocam memorie pentru pixelii imaginii grayscale
		image_data->grayscale = malloc(image_data->height
									   * sizeof(unsigned char *));
		for (int i = 0; i < image_data->height; i++) {
			image_data->grayscale[i] = malloc(image_data->width
											  * sizeof(unsigned char));
		}

		// Citim pixelii imaginii grayscale
		for (int i = 0; i < image_data->height; i++)
			for (int j = 0; j < image_data->width; j++) {
				fscanf(image_data->input_file, "%hhu",
					   &image_data->grayscale[i][j]);
			}
	}

	fclose(image_data->input_file);
}

void free_image(image_data_t *image_data)
{
	if (is_rgb(image_data)) {
		// Eliberam memoria pentru pixelii imaginii RGB
		for (int i = 0; i < image_data->height; i++)
			free(image_data->rgb_image[i]);
		free(image_data->rgb_image);
	} else if (is_grayscale(image_data)) {
		// Eliberam memoria pentru pixelii imaginii grayscale
		for (int i = 0; i < image_data->height; i++)
			free(image_data->grayscale[i]);
		free(image_data->grayscale);
	}
	image_data->x1 = 0;
	image_data->x2 = 0;
	image_data->y1 = 0;
	image_data->y2 = 0;
}

//Functia de incarcare a imaginii
void load_operation(image_data_t *image_data)
{
	// Citim numele imaginii
	scanf("%s", image_data->input_filename);
	image_data->input_file = fopen(image_data->input_filename, "rb");
	// Verificam daca s-a deschis fisierul
	if (!image_data->input_file) {
		printf("Failed to load %s\n", image_data->input_filename);
		image_data->ok = 0;
	} else {
		fscanf(image_data->input_file, "%s ", image_data->magic_word);
		fclose(image_data->input_file);
		if (is_binary(image_data)) {
			// Incarcam imaginea binara
			load_binaryimage(image_data);
		} else {
			// Incarcam imaginea ASCII
			if (is_ascii(image_data))
				load_image(image_data);
		}
		printf("Loaded %s\n", image_data->input_filename);
	}
	// Initializam selectia
	image_data->x1 = 0;
	image_data->x2 = image_data->width;
	image_data->y1 = 0;
	image_data->y2 = image_data->height;
}

// Functia de salvare a imaginii binare
void save_binary_image(image_data_t *image_data)
{
	image_data->output_file = fopen(image_data->output_filename, "wb");
	if (strcmp(image_data->magic_word, "P6") == 0) {
		// Scriem tipul imaginii
		fprintf(image_data->output_file, "%s\n", image_data->magic_word);
		// Scriem dimensiunile imaginii
		fprintf(image_data->output_file, "%d %d\n",
				image_data->width, image_data->height);
		fprintf(image_data->output_file, "%d\n", image_data->max_value);
		for (int i = 0; i < image_data->height; i++) {
			for (int j = 0; j < image_data->width; j++) {
				// Scriem pixelii imaginii RGB
				fwrite(&image_data->rgb_image[i][j].red,
					   sizeof(unsigned char), 1, image_data->output_file);
				fwrite(&image_data->rgb_image[i][j].green,
					   sizeof(unsigned char), 1, image_data->output_file);
				fwrite(&image_data->rgb_image[i][j].blue,
					   sizeof(unsigned char), 1, image_data->output_file);
			}
		}
	} else {
		if (strcmp(image_data->magic_word, "P5") == 0) {
			// Scriem tipul imaginii
			fprintf(image_data->output_file, "%s\n", image_data->magic_word);
			// Scriem dimensiunile imaginii
			fprintf(image_data->output_file, "%d %d\n",
					image_data->width, image_data->height);
			fprintf(image_data->output_file, "%d\n", image_data->max_value);
			for (int i = 0; i < image_data->height; i++) {
				for (int j = 0; j < image_data->width; j++) {
					// Scriem pixelii imaginii grayscale
					fwrite(&image_data->grayscale[i][j],
						   sizeof(unsigned char), 1, image_data->output_file);
				}
			}
		} else {
			if (strcmp(image_data->magic_word, "P3") == 0) {
				// Scriem tipul imaginii
				fprintf(image_data->output_file, "P6\n");
				// Scriem dimensiunile imaginii
				fprintf(image_data->output_file,
						"%d %d\n", image_data->width, image_data->height);
				fprintf(image_data->output_file, "%d\n", image_data->max_value);
				for (int i = 0; i < image_data->height; i++) {
					for (int j = 0; j < image_data->width; j++) {
						// Scriem pixelii imaginii RGB
						fwrite(&image_data->rgb_image[i][j].red,
							   sizeof(unsigned char), 1,
							   image_data->output_file);
						fwrite(&image_data->rgb_image[i][j].green,
							   sizeof(unsigned char), 1,
							   image_data->output_file);
						fwrite(&image_data->rgb_image[i][j].blue,
							   sizeof(unsigned char), 1,
							   image_data->output_file);
					}
				}
			} else {
				if (strcmp(image_data->magic_word, "P2") == 0) {
					// Scriem tipul imaginii
					fprintf(image_data->output_file, "P5\n");
					// Scriem dimensiunile imaginii
					fprintf(image_data->output_file, "%d %d\n",
							image_data->width, image_data->height);
					fprintf(image_data->output_file,
							"%d\n", image_data->max_value);
				for (int i = 0; i < image_data->height; i++) {
					for (int j = 0; j < image_data->width; j++) {
						// Scriem pixelii imaginii grayscale
						fwrite(&image_data->grayscale[i][j],
							   sizeof(unsigned char), 1,
							   image_data->output_file);
						}
					}
				}
			}
		}
	}
	fclose(image_data->output_file);
}

// Functia de salvare a imaginii ASCII
void save_ascii_image(image_data_t *image_data)
{
	image_data->output_file = fopen(image_data->output_filename, "w");
	if (is_rgb(image_data)) {
		// Scriem tipul imaginii
		fprintf(image_data->output_file, "P3\n");
		// Scriem dimensiunile imaginii
		fprintf(image_data->output_file,
				"%d %d\n", image_data->width, image_data->height);
		fprintf(image_data->output_file, "%d\n", image_data->max_value);
		for (int i = 0; i < image_data->height; i++) {
			for (int j = 0; j < image_data->width; j++) {
				// Scriem pixelii imaginii RGB
				fprintf(image_data->output_file,
						"%d ", image_data->rgb_image[i][j].red);
				fprintf(image_data->output_file,
						"%d ", image_data->rgb_image[i][j].green);
				fprintf(image_data->output_file,
						"%d ", image_data->rgb_image[i][j].blue);
			}
			fprintf(image_data->output_file, "\n");
		}
	} else {
		if (is_grayscale(image_data)) {
			// Scriem tipul imaginii
			fprintf(image_data->output_file, "P2\n");
			// Scriem dimensiunile imaginii
			fprintf(image_data->output_file,
					"%d %d\n", image_data->width, image_data->height);
			fprintf(image_data->output_file, "%d\n", image_data->max_value);
			for (int i = 0; i < image_data->height; i++) {
				for (int j = 0; j < image_data->width; j++) {
					// Scriem pixelii imaginii grayscale
					fprintf(image_data->output_file,
							"%d ", image_data->grayscale[i][j]);
				}
				fprintf(image_data->output_file, "\n");
			}
		}
	}
	fclose(image_data->output_file);
}

// Functia de salvare a imaginii
void save_operation(image_data_t *image_data)
{
	char buffer[50], ascii[10];
	fgets(buffer, 50, stdin);
	if (sscanf(buffer, "%s", image_data->output_filename) != 1)
		return;
	strcpy(ascii, buffer + strlen(image_data->output_filename) + 2);
	if (!fopen(image_data->input_filename, "rb")) {
		printf("No image loaded\n");
		return;
	}
	if (strcmp(ascii, "ascii\n") == 0)
		save_ascii_image(image_data);
	else
		save_binary_image(image_data);
	strcpy(buffer, "");
	printf("Saved %s\n", image_data->output_filename);
}

// Functia de executare a selectiei pixelilor
void select_operation(image_data_t *image_data)
{
	char buffer[50], all[20];
	int x1, x2, y1, y2;
	// Salvare selectie anterioara
	x1 = image_data->x1;
	x2 = image_data->x2;
	y1 = image_data->y1;
	y2 = image_data->y2;
	// Citire comanda
	fgets(buffer, 50, stdin);
	// Verificare comanda(ALL sau coordonate)
	if (sscanf(buffer, "%s", all) != 1)
		return;
	if (strcmp(all, "ALL") == 0) {
		if (!fopen(image_data->input_filename, "rb")) {
			printf("No image loaded\n");
			return;
		}
		image_data->x1 = 0;
		image_data->y1 = 0;
		image_data->x2 = image_data->width;
		image_data->y2 = image_data->height;
		printf("Selected ALL\n");
		return;
	}
	// Citire coordonate
	if (sscanf(buffer, "%d %d %d %d", &image_data->x1,
			   &image_data->y1, &image_data->x2, &image_data->y2) != 4) {
		printf("Invalid command\n");
		image_data->x1 = x1;
		image_data->x2 = x2;
		image_data->y1 = y1;
		image_data->y2 = y2;
		return;
	}
	// Cazuri de eroare
	if (!fopen(image_data->input_filename, "rb")) {
		printf("No image loaded\n");
		return;
	}
	if (image_data->x1 < 0 || image_data->x2 < 0 ||
		image_data->y1 < 0 || image_data->y2 < 0) {
		printf("Invalid set of coordinates\n");
		image_data->x1 = x1;
		image_data->x2 = x2;
		image_data->y1 = y1;
		image_data->y2 = y2;
		return;
	}
	if (image_data->x1 > image_data->width ||
		image_data->x2 > image_data->width ||
		image_data->y1 > image_data->height ||
		image_data->y2 > image_data->height) {
		printf("Invalid set of coordinates\n");
		image_data->x1 = x1;
		image_data->x2 = x2;
		image_data->y1 = y1;
		image_data->y2 = y2;
		return;
	}
	if (image_data->x1 == image_data->x2 || image_data->y1 == image_data->y2) {
		printf("Invalid set of coordinates\n");
		image_data->x1 = x1;
		image_data->x2 = x2;
		image_data->y1 = y1;
		image_data->y2 = y2;
		return;
	}
	if (image_data->x1 > image_data->x2) {
		int aux = image_data->x1;
		image_data->x1 = image_data->x2;
		image_data->x2 = aux;
	}
	if (image_data->y1 > image_data->y2) {
		int aux = image_data->y1;
		image_data->y1 = image_data->y2;
		image_data->y2 = aux;
	}
	printf("Selected %d %d %d %d\n", image_data->x1,
		   image_data->y1, image_data->x2, image_data->y2);
}

// Functia de decupare a imaginii
void crop_image(image_data_t *image_data)
{
	// Verificare daca exista imagine incarcata
	if (!fopen(image_data->input_filename, "rb")) {
		printf("No image loaded\n");
		return;
	}
	int x1, y1;
	x1 = image_data->x1;
	y1 = image_data->y1;
	// Verificare daca selectia este valida(All sau coordonate)
	if (image_data->width != image_data->x2 - image_data->x1 ||
		image_data->height != image_data->y2 - image_data->y1) {
		if (is_rgb(image_data)) {
			// Alocare memorie pentru noua imagine RGB
			rgb_image_t **aux = malloc((image_data->y2
										- image_data->y1)
										* sizeof(rgb_image_t *));
			for (int i = 0; i < image_data->y2 - image_data->y1; i++) {
				aux[i] = malloc((image_data->x2 - image_data->x1)
								* sizeof(rgb_image_t));
			}
			// Copierea pixelilor din selectie in noua imagine
			for (int i = image_data->y1; i < image_data->y2; i++) {
				for (int j = image_data->x1; j < image_data->x2; j++) {
					aux[i - y1][j - x1].red = image_data->rgb_image[i][j].red;
					aux[i - y1][j -
								x1].green = image_data->rgb_image[i][j].green;
					aux[i - y1][j -
								x1].blue = image_data->rgb_image[i][j].blue;
				}
			}
			// Eliberare memorie pentru vechea imagine RGB
			for (int i = 0; i < image_data->height; i++)
				free(image_data->rgb_image[i]);
			free(image_data->rgb_image);
			// Copierea noii imagini RGB in imaginea initiala
			image_data->rgb_image = aux;
			// Actualizare dimensiuni
			image_data->width = image_data->x2 - image_data->x1;
			image_data->height = image_data->y2 - image_data->y1;

		} else {
			if (is_grayscale(image_data)) {
				// Alocare memorie pentru noua imagine grayscale
				unsigned char **aux_grayscale = malloc((image_data->y2 - y1) *
													   sizeof(unsigned
													   char *));
				for (int i = 0; i < image_data->y2 - image_data->y1; i++) {
					aux_grayscale[i] = malloc((image_data->x2 - x1) *
											  sizeof(unsigned char));
				}
				// Copierea pixelilor din selectie in noua imagine
				for (int i = image_data->y1; i < image_data->y2; i++) {
					for (int j = image_data->x1; j < image_data->x2; j++) {
						aux_grayscale[i -
									  y1][j - x1] = image_data->grayscale[i][j];
					}
				}
				// Eliberare memorie pentru vechea imagine grayscale
				for (int i = 0; i < image_data->height; i++)
					free(image_data->grayscale[i]);
				free(image_data->grayscale);
				// Copierea noii imagini grayscale in imaginea initiala
				image_data->grayscale = aux_grayscale;
				// Actualizare dimensiuni
				image_data->width = image_data->x2 - image_data->x1;
				image_data->height = image_data->y2 - image_data->y1;
			}
		}
	}
	// Actualizare selectie
	image_data->x1 = 0;
	image_data->x2 = image_data->width;
	image_data->y1 = 0;
	image_data->y2 = image_data->height;
	printf("Image cropped\n");
}

void histogram_image(image_data_t *image_data)
{
	int fmax = 0, x, y, z;//fmax = frecventa maxima
	char buffer[15];
	// Citire comanda
	fgets(buffer, 15, stdin);
	// Verificare daca exista imagine incarcata
	if (!fopen(image_data->input_filename, "rb")) {
		printf("No image loaded\n");
		return;
	}
	// Verificare comanda
	if (sscanf(buffer, "%d %d %d", &x, &y, &z) != 2) {
		printf("Invalid command\n");
		return;
	}
	// Verificare daca imaginea este grayscale
	if (is_rgb(image_data)) {
		printf("Black and white image needed\n");
		return;
	}
	// Alocare memorie pentru vectorul de frecventa
	int *histogram = calloc(256, sizeof(int));
	// Calculare frecventa pixeli
	for (int i = 0; i < image_data->height; i++) {
		for (int j = 0; j < image_data->width; j++)
			histogram[image_data->grayscale[i][j]]++;
	}
	int k = 0;
	// Afisare histograma
	if (y == 256) {
		// Cazul in care se afiseaza histograma pe 256 de linii
		// Calculare frecventa maxima
		for (int i = 0; i < 256; i++)
			if (histogram[i] > fmax)
				fmax = histogram[i];
		for (int i = 0; i < 256; i++) {
			// calculare numar de stelute
			k = histogram[i] * x / fmax;
			// Afisare linie histograma
			printf("%d\t|\t", k);
			for (int j = 0; j < k; j++)
				printf("*");
			printf("\n");
		}
	} else {
		// Cazul in care se afiseaza histograma pe y linii
		int *aux_histogram = calloc(y, sizeof(int));
		// Calculare bin-uri
		for (int i = 0; i < y; i++) {
			for (int j = 0; j < 256 / y; j++)
				aux_histogram[i] += histogram[i * 256 / y + j];
		}
		// Calculare frecventa maxima
		for (int i = 0; i < y; i++)
			if (aux_histogram[i] > fmax)
				fmax = aux_histogram[i];
		// Afisare histograma
		for (int i = 0; i < y; i++) {
			// Calculare numar de stelute
			k = (aux_histogram[i] * x) / fmax;
			// Afisare linie histograma
			printf("%d\t|\t", k);
			for (int j = 0; j < k; j++)
				printf("*");
			printf("\n");
		}
		free(aux_histogram);
	}
	free(histogram);
}

// Functia de EQUALIZE a imaginii
void equalize_image(image_data_t *image_data)
{
	double newpixel;
	// Verificare daca exista imagine incarcata
	if (!fopen(image_data->input_filename, "rb")) {
		printf("No image loaded\n");
		return;
	}
	// Verificare daca imaginea este grayscale
	if (is_rgb(image_data)) {
		printf("Black and white image needed\n");
		return;
	}
	// Alocare memorie pentru vectorul de frecventa
	int *histogram = calloc(256, sizeof(int));
	// Calculare frecventa pixeli
	for (int i = 0; i < image_data->height; i++)
		for (int j = 0; j < image_data->width; j++)
			histogram[image_data->grayscale[i][j]]++;
	for (int i = 0; i < image_data->height; i++) {
		for (int j = 0; j < image_data->width; j++) {
			newpixel = 0;
			// Calculare noua valoare a pixelului
			for (int k = 0; k <= image_data->grayscale[i][j]; k++)
				newpixel += histogram[k];
			newpixel = (newpixel * 255 * 1.0) / (image_data->width
												 * image_data->height * 1.0);
			newpixel = round(newpixel);
			// Clamp
			if (newpixel > 255)
				newpixel = 255;
			else if (newpixel < 0)
				newpixel = 0;
			// Actualizare pixel
			image_data->grayscale[i][j] = (int)newpixel;
		}
	}
	printf("Equalize done\n");
	free(histogram);
}

// Clamp pentru functia Apply
void clamp(int *red, int *green, int *blue)
{
	if (*red > 255)
		*red = 255;
	if (*red < 0)
		*red = 0;
	if (*green > 255)
		*green = 255;
	if (*green < 0)
		*green = 0;
	if (*blue > 255)
		*blue = 255;
	if (*blue < 0)
		*blue = 0;
}

// Functia de aplicare a filtrului Edge
void apply_edge(image_data_t *image_data)
{
	int x1, x2, y1, y2;
	x1 = image_data->x1;
	x2 = image_data->x2;
	y1 = image_data->y1;
	y2 = image_data->y2;
	// Alocare memorie pentru noua imagine RGB
	rgb_image_t **aux = malloc((y2 - y1) * sizeof(rgb_image_t *));
	for (int i = 0; i < y2 - y1; i++)
		aux[i] = malloc((x2 - x1) * sizeof(rgb_image_t));
	// Matricea kernel
	int m[3][3] = {{-1, -1, -1},
				   {-1, 8, -1},
				   {-1, -1, -1}};
	// Aplicare filtru
	for (int i = image_data->y1; i < image_data->y2; i++)
		for (int j = image_data->x1; j < image_data->x2; j++) {
			if (i == 0 || i == image_data->height - 1 ||
				j == 0 || j == image_data->width - 1) {
				// Cazurile in care nu se aplica filtrul
				aux[i - y1][j - x1].red = image_data->rgb_image[i][j].red;
				aux[i - y1][j - x1].green = image_data->rgb_image[i][j].green;
				aux[i - y1][j - x1].blue = image_data->rgb_image[i][j].blue;
			} else {
				// Calculare valori noi pentru pixeli dupa aplicarea filtrului
				int red = 0, green = 0, blue = 0;
				for (int k = 0; k <= 2; k++)
					for (int q = 0; q <= 2; q++) {
						int l, c;
						l = i + k - 1;
						c = j + q - 1;
						red += image_data->rgb_image[l][c].red * m[k][q];
						green += image_data->rgb_image[l][c].green * m[k][q];
						blue += image_data->rgb_image[l][c].blue * m[k][q];
					}
				clamp(&red, &green, &blue);
				aux[i - y1][j - x1].red = red;
				aux[i - y1][j - x1].green = green;
				aux[i - y1][j - x1].blue = blue;
			}
		}
	// Copierea noii imagini in imaginea initiala
	for (int i = image_data->y1; i < image_data->y2; i++)
		for (int j = image_data->x1; j < image_data->x2; j++) {
			image_data->rgb_image[i][j].red = aux[i - y1][j - x1].red;
			image_data->rgb_image[i][j].green = aux[i - y1][j - x1].green;
			image_data->rgb_image[i][j].blue = aux[i - y1][j - x1].blue;
		}
	// Eliberare memorie
	for (int i = 0; i < y2 - y1; i++)
		free(aux[i]);
	free(aux);
	printf("APPLY EDGE done\n");
}

void apply_sharpen(image_data_t *image_data)
{
	int x1, x2, y1, y2;
	x1 = image_data->x1;
	x2 = image_data->x2;
	y1 = image_data->y1;
	y2 = image_data->y2;
	// Alocare memorie pentru noua imagine RGB
	rgb_image_t **aux = malloc((y2 - y1) * sizeof(rgb_image_t *));
	for (int i = 0; i < y2 - y1; i++)
		aux[i] = malloc((x2 - x1) * sizeof(rgb_image_t));
	// Matricea kernel
	int m[3][3] = {{0, -1, 0},
				   {-1, 5, -1},
				   {0, -1, 0}};
	// Aplicare filtru
	for (int i = image_data->y1; i < image_data->y2; i++)
		for (int j = image_data->x1; j < image_data->x2; j++) {
			if (i == 0 || i == image_data->height - 1 ||
				j == 0 || j == image_data->width - 1) {
				// Cazurile in care nu se aplica filtrul
				aux[i - y1][j - x1].red = image_data->rgb_image[i][j].red;
				aux[i - y1][j - x1].green = image_data->rgb_image[i][j].green;
				aux[i - y1][j - x1].blue = image_data->rgb_image[i][j].blue;
			} else {
				// Calculare valori noi pentru pixeli dupa aplicarea filtrului
				int red = 0, green = 0, blue = 0;
				for (int k = 0; k <= 2; k++)
					for (int q = 0; q <= 2; q++) {
						int l, c;
						l = i + k - 1;
						c = j + q - 1;
						red += image_data->rgb_image[l][c].red * m[k][q];
						green += image_data->rgb_image[l][c].green * m[k][q];
						blue += image_data->rgb_image[l][c].blue * m[k][q];
					}
				clamp(&red, &green, &blue);
				aux[i - y1][j - x1].red = red;
				aux[i - y1][j - x1].green = green;
				aux[i - y1][j - x1].blue = blue;
			}
		}
	// Copierea noii imagini in imaginea initiala
	for (int i = image_data->y1; i < image_data->y2; i++)
		for (int j = image_data->x1; j < image_data->x2; j++) {
			image_data->rgb_image[i][j].red = aux[i - y1][j - x1].red;
			image_data->rgb_image[i][j].green = aux[i - y1][j - x1].green;
			image_data->rgb_image[i][j].blue = aux[i - y1][j - x1].blue;
		}
	// Eliberare memorie
	for (int i = 0; i < y2 - y1; i++)
		free(aux[i]);
	free(aux);
	printf("APPLY SHARPEN done\n");
}

void apply_blur(image_data_t *image_data)
{
	int x1, x2, y1, y2;
	x1 = image_data->x1;
	x2 = image_data->x2;
	y1 = image_data->y1;
	y2 = image_data->y2;
	// Alocare memorie pentru noua imagine RGB
	rgb_image_t **aux = malloc((y2 - y1) * sizeof(rgb_image_t *));
	for (int i = 0; i < y2 - y1; i++)
		aux[i] = malloc((x2 - x1) * sizeof(rgb_image_t));
	// Aplicare filtru
	for (int i = image_data->y1; i < image_data->y2; i++)
		for (int j = image_data->x1; j < image_data->x2; j++) {
			if (i == 0 || i == image_data->height - 1 ||
				j == 0 || j == image_data->width - 1) {
				// Cazurile in care nu se aplica filtrul
				aux[i - y1][j - x1].red = image_data->rgb_image[i][j].red;
				aux[i - y1][j - x1].green = image_data->rgb_image[i][j].green;
				aux[i - y1][j - x1].blue = image_data->rgb_image[i][j].blue;
			} else {
				// Calculare valori noi pentru pixeli dupa aplicarea filtrului
				int red = 0, green = 0, blue = 0;
				for (int k = 0; k <= 2; k++)
					for (int q = 0; q <= 2; q++) {
						int l, c;
						l = i + k - 1;
						c = j + q - 1;
						red += image_data->rgb_image[l][c].red;
						green += image_data->rgb_image[l][c].green;
						blue += image_data->rgb_image[l][c].blue;
					}
				red = round((1.0 * red) / 9);
				green = round((1.0 * green) / 9);
				blue = round((1.0 * blue) / 9);
				clamp(&red, &green, &blue);
				aux[i - y1][j - x1].red = red;
				aux[i - y1][j - x1].green = green;
				aux[i - y1][j - x1].blue = blue;
			}
		}
	// Copierea noii imagini in imaginea initiala
	for (int i = image_data->y1; i < image_data->y2; i++)
		for (int j = image_data->x1; j < image_data->x2; j++) {
			image_data->rgb_image[i][j].red = aux[i - y1][j - x1].red;
			image_data->rgb_image[i][j].green = aux[i - y1][j - x1].green;
			image_data->rgb_image[i][j].blue = aux[i - y1][j - x1].blue;
		}
	// Eliberare memorie
	for (int i = 0; i < y2 - y1; i++)
		free(aux[i]);
	free(aux);
	printf("APPLY BLUR done\n");
}

void apply_gaussian_blur(image_data_t *image_data)
{
	int x1, x2, y1, y2;
	x1 = image_data->x1;
	x2 = image_data->x2;
	y1 = image_data->y1;
	y2 = image_data->y2;
	// Alocare memorie pentru noua imagine RGB
	rgb_image_t **aux = malloc((y2 - y1) * sizeof(rgb_image_t *));
	for (int i = 0; i < y2 - y1; i++)
		aux[i] = malloc((x2 - x1) * sizeof(rgb_image_t));
	// Aplicare filtru
	int m[3][3] = {{1, 2, 1},
				   {2, 4, 2},
				   {1, 2, 1}};
	for (int i = image_data->y1; i < image_data->y2; i++)
		for (int j = image_data->x1; j < image_data->x2; j++) {
			if (i == 0 || i == image_data->height - 1 ||
				j == 0 || j == image_data->width - 1) {
				// Cazurile in care nu se aplica filtrul
				aux[i - y1][j - x1].red = image_data->rgb_image[i][j].red;
				aux[i - y1][j - x1].green = image_data->rgb_image[i][j].green;
				aux[i - y1][j - x1].blue = image_data->rgb_image[i][j].blue;
			} else {
				// Calculare valori noi pentru pixeli dupa aplicarea filtrului
				int red = 0, green = 0, blue = 0;
				for (int k = 0; k <= 2; k++)
					for (int q = 0; q <= 2; q++) {
						int l, c;
						l = i + k - 1;
						c = j + q - 1;
						red += image_data->rgb_image[l][c].red * m[k][q];
						green += image_data->rgb_image[l][c].green * m[k][q];
						blue += image_data->rgb_image[l][c].blue * m[k][q];
					}
				red = round((1.0 * red) / 16);
				green = round((1.0 * green) / 16);
				blue = round((1.0 * blue) / 16);
				clamp(&red, &green, &blue);
				aux[i - y1][j - x1].red = red;
				aux[i - y1][j - x1].green = green;
				aux[i - y1][j - x1].blue = blue;
			}
		}
	// Copierea noii imagini in imaginea initiala
	for (int i = image_data->y1; i < image_data->y2; i++)
		for (int j = image_data->x1; j < image_data->x2; j++) {
			image_data->rgb_image[i][j].red = aux[i - y1][j - x1].red;
			image_data->rgb_image[i][j].green = aux[i - y1][j - x1].green;
			image_data->rgb_image[i][j].blue = aux[i - y1][j - x1].blue;
		}
	// Eliberare memorie
	for (int i = 0; i < y2 - y1; i++)
		free(aux[i]);
	free(aux);
	printf("APPLY GAUSSIAN_BLUR done\n");
}

// Functia de aplicare a filtrului de sepia
void apply_operation(image_data_t *image_data)
{
	char buffer[30];
	// Citire comanda
	fgets(buffer, 30, stdin);
	// Verificare daca exista imagine incarcata
	if (!fopen(image_data->input_filename, "rb")) {
		printf("No image loaded\n");
		return;
	}
	// Verificare comanda
	if (strcmp(buffer, "\n") == 0) {
		printf("Invalid command\n");
		return;
	}
	// Verificare daca imaginea este grayscale
	if (image_data->magic_word[1] == '5' || image_data->magic_word[1] == '2') {
		printf("Easy, Charlie Chaplin\n");
		return;
	}
	// Verificare daca exista selectie
	if (strcmp(buffer, " EDGE\n") == 0) {
		apply_edge(image_data);
	} else {
		if (strcmp(buffer, " SHARPEN\n") == 0) {
			apply_sharpen(image_data);
		} else {
			if (strcmp(buffer, " BLUR\n") == 0) {
				apply_blur(image_data);
			} else {
				if (strcmp(buffer, " GAUSSIAN_BLUR\n") == 0)
					apply_gaussian_blur(image_data);
				else
					printf("APPLY parameter invalid\n");
			}
		}
	}
}

// Functia pentru comanda invalida
void invalid_command(void)
{
	printf("Invalid command\n");
	char c, buffer[50];
	scanf("%c", &c);
	if (c == ' ')
		fgets(buffer, 50, stdin);
}

// Functia de rotire a imaginii RGB  in selectie
void rotate_rgb(image_data_t *image_data, int angle)
{
	int x1, x2, y1, y2, k = 0;
	x1 = image_data->x1;
	x2 = image_data->x2;
	y1 = image_data->y1;
	y2 = image_data->y2;
	// Alocare memorie pentru noua imagine RGB
	rgb_image_t **aux = malloc((y2 - y1) * sizeof(rgb_image_t *));
	for (int i = 0; i < y2 - y1; i++)
		aux[i] = malloc((x2 - x1) * sizeof(rgb_image_t));
	// Trecere la unghi pozitiv
	if (angle < 0)
		angle = 360 + angle;
	// Calculare numar de rotatii
	k = angle / 90;
	// Rotirea imaginii de k ori la unghi de 90 de grade
	while (k) {
		for (int i = 0; i < x2 - x1; i++) {
			int c = 0;
			for (int j = y2 - y1 - 1; j >= 0; j--) {
				int l, y;
				l = j + y1;
				y = i + x1;
				aux[i][c].red = image_data->rgb_image[l][y].red;
				aux[i][c].green = image_data->rgb_image[l][y].green;
				aux[i][c++].blue = image_data->rgb_image[l][y].blue;
			}
		}
		for (int i = y1; i < y2; i++)
			for (int j = x1; j < x2; j++) {
				int l, c;
				l = i - y1;
				c = j - x1;
				image_data->rgb_image[i][j].red = aux[l][c].red;
				image_data->rgb_image[i][j].green = aux[l][c].green;
				image_data->rgb_image[i][j].blue = aux[l][c].blue;
			}
		k--;
	}
	// Eliberare memorie
	for (int i = 0; i < y2 - y1; i++)
		free(aux[i]);
	free(aux);
}

// Functia de rotire a intregii imagini RGB
void rotate_rgb_full(image_data_t *image_data, int angle)
{
	int x1 = 0, x2, y1, y2, k = 0, cmax;
	x2 = image_data->width;
	y1 = 0;
	y2 = image_data->height;
	if (x2 > y2)
		cmax = x2;
	else
		cmax = y2;
	k = angle / 90;
	while (k) {
		rgb_image_t **aux = malloc(cmax * sizeof(rgb_image_t *));
		for (int i = 0; i < cmax; i++)
			aux[i] = malloc(image_data->height * sizeof(rgb_image_t));
		if (k > 0) {
			// Rotirea la dreapta
			for (int i = y1; i < y2; i++)
				for (int j = x1; j < x2; j++) {
					int c = y2 - i - 1;
					aux[j][c].red = image_data->rgb_image[i][j].red;
					aux[j][c].green = image_data->rgb_image[i][j].green;
					aux[j][c].blue = image_data->rgb_image[i][j].blue;
				}
			for (int i = 0; i < image_data->height; i++)
				free(image_data->rgb_image[i]);
			free(image_data->rgb_image);
			image_data->rgb_image = malloc(image_data->width
										   * sizeof(rgb_image_t *));
			for (int i = 0; i < image_data->width; i++)
				image_data->rgb_image[i] = malloc(image_data->height
												  * sizeof(rgb_image_t));
			// Copierea noii imagini in imaginea initiala
			for (int i = x1; i < x2; i++)
				for (int j = y1; j < y2; j++) {
					image_data->rgb_image[i][j].red = aux[i][j].red;
					image_data->rgb_image[i][j].green = aux[i][j].green;
					image_data->rgb_image[i][j].blue = aux[i][j].blue;
				}
			k--;
		} else {
			// Rotirea la stanga
			for (int i = y1; i < y2; i++)
				for (int j = x1; j < x2; j++) {
					int l = x2 - j - 1;
					aux[l][i].red = image_data->rgb_image[i][j].red;
					aux[l][i].green = image_data->rgb_image[i][j].green;
					aux[l][i].blue = image_data->rgb_image[i][j].blue;
				}
			// Eliberare memorie matrice veche si alocare memorie matrice noua
			for (int i = 0; i < image_data->height; i++)
				free(image_data->rgb_image[i]);
			free(image_data->rgb_image);
			image_data->rgb_image = malloc(image_data->width
										   * sizeof(rgb_image_t *));
			for (int i = 0; i < image_data->width; i++)
				image_data->rgb_image[i] = malloc(image_data->height
												  * sizeof(rgb_image_t));
			// Copierea noii imagini in imaginea initiala
			for (int i = x1; i < x2; i++)
				for (int j = y1; j < y2; j++) {
					int l, c;
					l = i - y1;
					c = j - x1;
					image_data->rgb_image[i][j].red = aux[l][c].red;
					image_data->rgb_image[i][j].green = aux[l][c].green;
					image_data->rgb_image[i][j].blue = aux[l][c].blue;
				}
			k++;
		}
		// Actualizare dimensiuni
		image_data->width = y2;
		image_data->height = x2;
		x2 = image_data->width;
		y2 = image_data->height;
		image_data->x2 = image_data->width;
		image_data->y2 = image_data->height;
		for (int i = 0; i < cmax; i++)
			free(aux[i]);
		free(aux);
	}
}

// Functia de rotire a imaginii grayscale in selectie
void rotate_grayscale(image_data_t *image_data, int angle)
{
	int x1, x2, y1, y2, k = 0;
	x1 = image_data->x1;
	x2 = image_data->x2;
	y1 = image_data->y1;
	y2 = image_data->y2;
	// Alocare memorie pentru noua imagine grayscale
	unsigned char **aux = malloc((y2 - y1) * sizeof(unsigned char *));
	for (int i = 0; i < y2 - y1; i++)
		aux[i] = malloc((x2 - x1) * sizeof(unsigned char));
	// Trecere la unghi pozitiv
	if (angle < 0)
		angle = 360 + angle;
	// Calculare numar de rotatii
	k = angle / 90;
	// Rotirea imaginii de k ori la unghi de 90 de grade
	while (k) {
		for (int i = 0; i < x2 - x1; i++) {
			int c = 0;
			for (int j = y2 - y1 - 1; j >= 0; j--)
				aux[i][c++] = image_data->grayscale[j + y1][i + x1];
		}
		for (int i = y1; i < y2; i++)
			for (int j = x1; j < x2; j++)
				image_data->grayscale[i][j] = aux[i - y1][j - x1];
		k--;
	}
	// Eliberare memorie
	for (int i = 0; i < y2 - y1; i++)
		free(aux[i]);
	free(aux);
}

// Functia de rotire a intregii imagini grayscale
void rotate_grayscale_full(image_data_t *image_data, int angle)
{
	int x1, x2, y1, y2, k = 0, cmax;
	x1 = 0;
	x2 = image_data->width;
	y1 = 0;
	y2 = image_data->height;
	// Identificare dimensiune maxima
	if (x2 > y2)
		cmax = x2;
	else
		cmax = y2;
	// Calculare numar de rotatii
	k = angle / 90;
	// Rotirea imaginii de k ori la unghi de 90 de grade
	while (k) {
		// Alocare memorie pentru noua imagine grayscale
		unsigned char **aux = malloc(cmax * sizeof(unsigned char *));
		for (int i = 0; i < cmax; i++)
			aux[i] = malloc(cmax * sizeof(unsigned char));
		if (k > 0) {
			// Rotirea la dreapta
			for (int i = y1; i < y2; i++)
				for (int j = x1; j < x2; j++)
					aux[j][y2 - i - 1] = image_data->grayscale[i][j];
			// Eliberare memorie matrice veche
			for (int i = 0; i < image_data->height; i++)
				free(image_data->grayscale[i]);
			free(image_data->grayscale);
			// Alocare memorie matrice noua
			image_data->grayscale = malloc(image_data->width
										   * sizeof(unsigned char *));
			for (int i = 0; i < image_data->width; i++)
				image_data->grayscale[i] = malloc(image_data->height
												  * sizeof(unsigned char));
			// Copierea noii imagini in imaginea initiala
			for (int i = x1; i < x2; i++)
				for (int j = y1; j < y2; j++)
					image_data->grayscale[i][j] = aux[i - y1][j - x1];
			k--;
		} else {
			// Rotirea la stanga
			for (int i = y1; i < y2; i++)
				for (int j = x1; j < x2; j++)
					aux[x2 - j - 1][i] = image_data->grayscale[i][j];
			// Eliberare memorie matrice veche si alocare memorie matrice noua
			for (int i = 0; i < image_data->height; i++)
				free(image_data->grayscale[i]);
			free(image_data->grayscale);
			image_data->grayscale = malloc(image_data->width
										   * sizeof(unsigned char *));
			for (int i = 0; i < image_data->width; i++)
				image_data->grayscale[i] = malloc(image_data->height
												  * sizeof(unsigned char));
			// Copierea noii imagini in imaginea initiala
			for (int i = x1; i < x2; i++)
				for (int j = y1; j < y2; j++)
					image_data->grayscale[i][j] = aux[i - y1][j - x1];
			k++;
		}
		// Actualizare dimensiuni
		image_data->width = y2;
		image_data->height = x2;
		x2 = image_data->width;
		y2 = image_data->height;
		// Eliberare memorie
		for (int i = 0; i < image_data->width; i++)
			free(aux[i]);
		free(aux);
		image_data->x2 = image_data->width;
		image_data->y2 = image_data->height;
	}
}

// Functia de rotire a imaginii
void rotate_operation(image_data_t *image_data)
{
	int x1, x2, y1, y2, angle, ok = 0;
	x1 = image_data->x1;
	x2 = image_data->x2;
	y1 = image_data->y1;
	y2 = image_data->y2;
	// Citire unghi
	scanf("%d", &angle);
	if (x1 == 0 && x2 == image_data->width &&
		y1 == 0 && y2 == image_data->height)
		ok = 1;
	// Verificare daca exista imagine incarcata
	if (!fopen(image_data->input_filename, "rb")) {
		printf("No image loaded\n");
		return;
	}
	// Verificare unghi
	if (angle % 90 != 0) {
		printf("Unsupported rotation angle\n");
		return;
	}
	// Verificare daca selectia este patratica
	if (x2 - x1 != y2 - y1 && ok == 0) {
		printf("Invalid set of coordinates\n");
		return;
	}
	if ((angle / 90) % 4 == 0) {
		printf("Rotated %d\n", angle);
		return;
	}
	// Verificare daca imaginea este grayscale sau RGB
	if (is_rgb(image_data)) {
		if (ok == 0)
			rotate_rgb(image_data, angle);
		else
			rotate_rgb_full(image_data, angle);
	} else {
		if (ok == 0)
			rotate_grayscale(image_data, angle);
		else
			rotate_grayscale_full(image_data, angle);
	}
	printf("Rotated %d\n", angle);
}
