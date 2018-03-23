#include <stdio.h>

typedef struct _pixel {
    unsigned short int red;
    unsigned short int green;
    unsigned short int blue;
} Pixel;

typedef struct _image {
    Pixel pixel[512][512]; 
    unsigned int width;
    unsigned int height;
} Image;

int equal_pixel(Pixel p1, Pixel p2) {
    if (p1.red == p2.red &&
        p1.green == p2.green &&
        p1.blue == p2.blue)
        return 1;
    return 0;
}


Image grey_scale(Image img) {
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            int media = img.pixel[i][j].red +
                        img.pixel[i][j].green +
                        img.pixel[i][j].blue;
            media /= 3;
            img.pixel[i][j].red = media;
            img.pixel[i][j].green = media;
            img.pixel[i][j].blue = media;
        }
    }

    return img;
}

Image blur(int T, Image img) {

    Image image = img;

    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            Pixel media = {0, 0, 0};

            int menor_h = (image.height - 1 > i + T/2) ? i + T/2 : image.height - 1;
            int min_w = (image.width - 1 > j + T/2) ? j + T/2 : image.width - 1;
            for(int x = (0 > i - T/2 ? 0 : i - T/2); x <= menor_h; ++x) {
                for(int y = (0 > j - T/2 ? 0 : j - T/2); y <= min_w; ++y) {
                    media.red += image.pixel[x][y].red;
                    media.green += image.pixel[x][y].green;
                    media.blue += image.pixel[x][y].blue;
                }
            }
            media.red /= T * T;
            media.green /= T * T;
            media.blue /= T * T;

            image.pixel[i][j].red = media.red;
            image.pixel[i][j].green = media.green;
            image.pixel[i][j].blue = media.blue;
        }
    }

    return image;
}

Image rotate90right(Image img) {
    Image rotated;

    rotated.width = img.height;
    rotated.height = img.width;

    for (unsigned int i = 0, y = 0; i < rotated.height; ++i, ++y) {
        for (int j = rotated.width - 1, x = 0; j >= 0; --j, ++x) {
            rotated.pixel[i][j].red = img.pixel[x][y].red;
            rotated.pixel[i][j].green = img.pixel[x][y].green;
            rotated.pixel[i][j].blue = img.pixel[x][y].blue;
        }
    }

    return rotated;
}

Image invert_colors(Image img){
    Image image = img;
    for (unsigned int i = 0; i < image.height; ++i) {
        for (unsigned int j = 0; j < image.width; ++j) {
            image.pixel[i][j].red = 255 - image.pixel[i][j].red;
            image.pixel[i][j].green = 255 - image.pixel[i][j].green;
            image.pixel[i][j].blue = 255 - image.pixel[i][j].blue;
        }
    }
    return image;
}

Image crop_image(Image img, int x, int y, int width, int height) {
    Image cropped;

    cropped.width = width;
    cropped.height = height;

    for(int i = 0; i < height; ++i) {
        for(int j = 0; j < width; ++j) {
            cropped.pixel[i][j].red = img.pixel[i + y][j + x].red;
            cropped.pixel[i][j].green = img.pixel[i + y][j + x].green;
            cropped.pixel[i][j].blue = img.pixel[i + y][j + x].blue;
        }
    }

    return cropped;
}


int main() {
    Image img;

    // read type of image
    char p3[4];
    scanf("%s", p3);

    // read width height and color of image
    int max_color;
    scanf("%u %u %d", &img.width, &img.height, &max_color);

    // read all pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            scanf("%hu %hu %hu", &img.pixel[i][j].red,
                                 &img.pixel[i][j].green,
                                 &img.pixel[i][j].blue);

        }
    }

    int n_options;
    scanf("%d", &n_options);

    for(int i = 0; i < n_options; ++i) {
        int option;
        scanf("%d", &option);

        switch(option) {
            case 1: { 
                img = grey_scale(img);
                break;
            }
            case 2: { //Sepia Filter
                for (unsigned int x = 0; x < img.height; ++x) {
                    for (unsigned int j = 0; j < img.width; ++j) {
                       Pixel pixel;
                        pixel.red = img.pixel[x][j].red;
                        pixel.green = img.pixel[x][j].green;
                        pixel.blue = img.pixel[x][j].blue;

                        int p =  pixel.red * .393 + pixel.green * .769 + pixel.blue * .189;
                        int menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j].red = menor_r;

                        p =  pixel.red * .349 + pixel.green * .686 + pixel.blue * .168;
                        menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j].green = menor_r;

                        p =  pixel.red * .272 + pixel.green * .534 + pixel.blue * .131;
                        menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j].blue = menor_r;
                    }
                }

                break;
            }
            case 3: { // Blur
                int size = 0;
                scanf("%d", &size);
                img = blur(size, img);
                break;
            }
            case 4: { // Rotation
                int number_of_times = 0;
                scanf("%d", &number_of_times);
                number_of_times %= 4;
                for (int j = 0; j < number_of_times; ++j) {
                    img = rotate90right(img);
                }
                break;
            }
            case 5: { // Mirroring
                int horizontal = 0;
                scanf("%d", &horizontal);

                int width = img.width, height = img.height;

                if (horizontal == 1) width /= 2;
                else height /= 2;

                for (int i2 = 0; i2 < height; ++i2) {
                    for (int j = 0; j < width; ++j) {
                        int x = i2, y = j;

                        if (horizontal == 1) y = img.width - 1 - j;
                        else x = img.height - 1 - i2;

                        Pixel aux1;
                        aux1.red = img.pixel[i2][j].red;
                        aux1.green = img.pixel[i2][j].green;
                        aux1.blue = img.pixel[i2][j].blue;

                        img.pixel[i2][j].red = img.pixel[x][y].red;
                        img.pixel[i2][j].green = img.pixel[x][y].green;
                        img.pixel[i2][j].blue = img.pixel[x][y].blue;

                        img.pixel[x][y].red = aux1.red;
                        img.pixel[x][y].green = aux1.green;
                        img.pixel[x][y].blue = aux1.blue;
                    }
                }
                break;
            }
            case 6: { // Color Inversion
                img = invert_colors(img);
                break;
            }
            case 7: { // Cropping image
                int x, y;
                scanf("%d %d", &x, &y);
                int width, height;
                scanf("%d %d", &width, &height);

                img = crop_image(img, x, y, width, height);
                break;
            }
        }

    }

    // print type of image
    printf("P3\n");
    // print width height and color of image
    printf("%u %u\n255\n", img.width, img.height);

    // print pixels of image
    for (unsigned int i = 0; i < img.height; ++i) {
        for (unsigned int j = 0; j < img.width; ++j) {
            printf("%hu %hu %hu ", img.pixel[i][j].red,
                                   img.pixel[i][j].green,
                                   img.pixel[i][j].blue);

        }
        printf("\n");
    }
    return 0;
}
