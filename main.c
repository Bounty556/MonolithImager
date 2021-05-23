/* File: Main.c
 * Author: Jacob Peterson
 * Last updated: 5/22/2021
 */

#include <stdio.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

enum color {
    White = 0,
    Pink = 1,
    Yellow = 2,
    Blue = 3
};

struct rgb {
    unsigned char r;
    unsigned char g;
    unsigned char b;
};

const struct rgb color_values[4] = {
    {200, 200, 200},
    {240, 157, 198},
    {239, 189, 112},
    {89, 174, 196}
};

const int grid_width = 22;
const int grid_height = 11;
enum color grid_colors[22*11];

int get_color_difference(struct rgb color1, struct rgb color2) {
    int x = color1.r - color2.r;
    int y = color1.g - color2.g;
    int z = color1.b - color2.b;

    // Sum of abs value of difference
    return ((x>=0) ? x: -x) + ((y >= 0) ? y : -y) + ((z >= 0) ? z : -z);
}

enum color get_closest_color(struct rgb color) {
    int smallest_diff = 9999999;
    int smallest_diff_index = 0;
    for (int i = 0; i < 4; i++) {
        int diff = get_color_difference(color, color_values[i]);
        if (diff < smallest_diff) {
            smallest_diff = diff;
            smallest_diff_index = i;
        }
    }

    return smallest_diff_index;
}

struct rgb get_image_pixel_color(unsigned char* data, int x, int y, int width, int color_components) {
    struct rgb color;

    color.r = data[(y * width + x) * color_components];
    color.g = data[(y * width + x) * color_components + 1];
    color.b = data[(y * width + x) * color_components + 2];

    return color;
}

int main() {
    
    // Get filename
    printf("Enter name of Monolith screenshot:");
    char filename[256];
    scanf_s("%s", &filename);

    // Load image
    int img_width,img_height,n;
    unsigned char* data = stbi_load(filename, &img_width, &img_height, &n, 0);
    // ... x = width, y = height, n = # 8-bit components per pixel ...

    // Save image data
    const int block_width = img_width / grid_width;
    const int block_height = img_height / grid_height;

    int i = 0;
    for (int y = 0; y < 11; y++) {
        for (int x = 0; x < 22; x++) {
            grid_colors[i] =
                get_closest_color(get_image_pixel_color(data, (x*block_width)+block_width/2, (y*block_height)+block_height/2, img_width, n));
            i++;
        }
    }
    stbi_image_free(data);

    FILE* fp;
    fp = fopen("output.csv", "w");
    i = 0;
    for (int y = 0; y < 11; y++) {
        for (int x = 0; x < 22; x++) {
            fprintf(fp, "%d", grid_colors[i] + 1);
            i++;
            if (x != 21)
                fprintf(fp, ",");
        }
        fprintf(fp, "\n");
    }

    fclose(fp);

    return 0;
}