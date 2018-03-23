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

int maximum(int a, int b) {
    if (a > b)
        return a;
    return b;
}

int minimum(int a, int b){
    if(a < b)
      return a;
    return b;
}


Image grey_scale(Image img) {
        for (unsigned int i = 0; i < img.height; ++i) {
                for (unsigned int j = 0; j < img.width; ++j) {
                        int average = img.pixel[i][j].red +
                                    img.pixel[i][j].green +
                                    img.pixel[i][j].blue;
                        average /= 3;
                        img.pixel[i][j].red = average;
                        img.pixel[i][j].green = average;
                        img.pixel[i][j].blue = average;
                }
        }

        return img;
}
Image sepia_filter(Image img){
        Image sepia;
        for (unsigned int x = 0; x < img.height; ++x) {
                for (unsigned int j = 0; j < img.width; ++j) {

                        sepia.pixel[x][j].red = img.pixel[x][j].red;
                        sepia.pixel[x][j].green = img.pixel[x][j].green;
                        sepia.pixel[x][j].blue = img.pixel[x][j].blue;

                        int p =  sepia.pixel[x][j].red * .393 + sepia.pixel[x][j].green * .769 + sepia.pixel[x][j].blue * .189;
                        int menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j].red = menor_r;

                        p =  sepia.pixel[x][j].red * .349 + sepia.pixel[x][j].green * .686 + sepia.pixel[x][j].blue * .168;
                        menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j].green = menor_r;

                        p =  sepia.pixel[x][j].red * .272 + sepia.pixel[x][j].green * .534 + sepia.pixel[x][j].blue * .131;
                        menor_r = (255 >  p) ? p : 255;
                        img.pixel[x][j].blue = menor_r;
                }
        }
        return img;
}

Image blur(Image img) {
        int size = 0;
        scanf("%d", &size);
        Image blurring = img;

        for (unsigned int i = 0; i < blurring.height; ++i) {
                for (unsigned int j = 0; j < blurring.width; ++j) {
                        Pixel average = {0, 0, 0};

                        int minor_height = minimum(i + size/2, blurring.height -1);
                        int minor_width = minimum(j + size/2, blurring.width - 1);
                          for(int x = maximum(0, i - size/2); x <= minor_height; ++x) {
                              for(int y = maximum(0, j - size/2); y <= minor_width; ++y) {
                                        average.red += blurring.pixel[x][y].red;
                                        average.green += blurring.pixel[x][y].green;
                                        average.blue += blurring.pixel[x][y].blue;
                                }
                        }
                        average.red /= size * size;
                        average.green /= size * size;
                        average.blue /= size * size;

                        blurring.pixel[i][j].red = average.red;
                        blurring.pixel[i][j].green = average.green;
                        blurring.pixel[i][j].blue = average.blue;
                }
        }

        return blurring;
}

Image rotate90right(Image img) {
        int number_of_times = 0;
        scanf("%d", &number_of_times);
        number_of_times %= 4;
        Image rotated;

        rotated.width = img.height;
        rotated.height = img.width;
        for (int j = 0; j < number_of_times; ++j) {
                for (unsigned int i = 0, y = 0; i < rotated.height; ++i, ++y) {
                        for (int j = rotated.width - 1, x = 0; j >= 0; --j, ++x) {
                                rotated.pixel[i][j].red = img.pixel[x][y].red;
                                rotated.pixel[i][j].green = img.pixel[x][y].green;
                                rotated.pixel[i][j].blue = img.pixel[x][y].blue;
                        }
                }
        }

        return rotated;
}

Image mirroring(Image img){
        int horizontal = 0;
        scanf("%d", &horizontal);

        int width = img.width, height = img.height;
        Image mirror = img;
        if (horizontal == 1) width /= 2;
        else height /= 2;

        for (int i = 0; i < height; ++i) {
                for (int j = 0; j < width; ++j) {
                        int x = i, y = j;

                        if (horizontal == 1) y = img.width - 1 - j;
                        else x = img.height - 1 - i;

                        Pixel aux1;
                        aux1.red = img.pixel[i][j].red;
                        aux1.green = img.pixel[i][j].green;
                        aux1.blue = img.pixel[i][j].blue;

                        mirror.pixel[i][j].red = mirror.pixel[x][y].red;
                        mirror.pixel[i][j].green = mirror.pixel[x][y].green;
                        mirror.pixel[i][j].blue = mirror.pixel[x][y].blue;

                        mirror.pixel[x][y].red = aux1.red;
                        mirror.pixel[x][y].green = aux1.green;
                        mirror.pixel[x][y].blue = aux1.blue;
                }
        }
        return mirror;
}
Image invert_colors(Image img){
        Image invertedimage = img;
        for (unsigned int i = 0; i < invertedimage.height; ++i) {
                for (unsigned int j = 0; j < invertedimage.width; ++j) {
                        invertedimage.pixel[i][j].red = 255 - invertedimage.pixel[i][j].red;
                        invertedimage.pixel[i][j].green = 255 - invertedimage.pixel[i][j].green;
                        invertedimage.pixel[i][j].blue = 255 - invertedimage.pixel[i][j].blue;
                }
        }
        return invertedimage;
}

Image crop_image(Image img) {
        int x, y;
        scanf("%d %d", &x, &y);
        int width, height;
        scanf("%d %d", &width, &height);
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
                        img = sepia_filter(img);
                        break;
                }
                case 3: { // Blur
                        img = blur(img);
                        break;
                }
                case 4: { // Rotation
                        img = rotate90right(img);
                        break;
                }
                case 5: { // Mirroring
                        img = mirroring(img);
                        break;
                }
                case 6: { // Color Inversion
                        img = invert_colors(img);
                        break;
                }
                case 7: { // Cropping image
                        img = crop_image(img);
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
