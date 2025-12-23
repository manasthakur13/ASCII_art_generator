#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

void sanitizePath(char *path)
{
    // remove the newline
    for (int i = 0; path[i]; i++)
    {
        if (path[i] == '\n')
        {
            path[i] = '\0';
            break;
        }
    }
    int len = strlen(path);
    // remove the quotes in the path name
    if (len >= 2 && path[0] == '"' && path[len - 1] == '"')
    {
        // shift left by 1
        for (int i = 0; i < len - 1; i++)
        {
            path[i] = path[i + 1];
        }
        path[len - 2] = '\0';
    }
}

void ascii_convert(int *height, int *width, int *channels, unsigned char *image)
{
    // ASCII density scale
    const char *ascii = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
    int asciiLen = strlen(ascii) - 1;
    // Target ASCII size (terminal-friendly)
    int targetWidth = 120;
    int targetHeight = 40;

    int xStep = (*width) / targetWidth;
    int yStep = (*height) / targetHeight;

    if (xStep < 1)
        xStep = 1;
    if (yStep < 1)
        yStep = 1;
    printf("\n--- ASCII IMAGE ART ---\n\n");
    for (int y = 0; y < *height; y += yStep)
    { // aspect ratio matching
        for (int x = 0; x < *width; x += xStep)
        {

            int index = (y * (*width) + x) * (*channels);
            unsigned char r = image[index];
            unsigned char g = image[index + 1];
            unsigned char b = image[index + 2];
            // grayscale conversion ITU-R BT.601
            int gray = (int)(0.299 * r + 0.587 * g + 0.114 * b);
            // Mapping brightness to ASCII
            int asciiIndex = (gray * asciiLen) / 255;
            printf("\033[38;2;%d;%d;%dm%c\033[0m", r, g, b, ascii[asciiIndex]);
        }
        putchar('\n');
    }
}

int main()
{
    char imagePath[512];
    int choice;
    printf("********IMAGE TO ASCII ART CONVERSION PROGRAM********\n\n");
label:
    printf("\nSelect your choice: \n");
    printf("1.Convert the image (^.^)\n");
    printf("0. Exit!!!\n");
    scanf("%d", &choice);
    if (choice != 1 && choice != 0)
    {
        printf("Invalid choice!!!\n");
        goto label;
    }
    while (1)
    {
        if (choice == 1)
        {
            printf("Enter the image path: ");
            //clear any buffer
            int c;
            while((c = getchar())!= '\n' && c!=EOF);
            fgets(imagePath, sizeof(imagePath), stdin);

            sanitizePath(imagePath);

            int width, height, channels;

            printf("Loading: %s\n", imagePath);

            // loading the image using std_image librabry function
            unsigned char *image = stbi_load(imagePath, &width, &height, &channels, 3);
            // failure
            if (image == NULL)
            {
                printf("Image Loading Failed!!!.\n");
                printf("Reason: %s\n", stbi_failure_reason());
                return 1;
            }
            // successs
            printf("Succsessfully Loaded!!!.\n");
            printf("Width : %d pixels\nHeight : %d pixels\nChannels : %d\n", width, height, channels);
            ascii_convert(&height, &width, &channels, image);
            // free the memory
            stbi_image_free(image);
            goto label;
        }
        else if (choice == 0)
        {
            printf("\nExiting the program!!!\nSEE YA................");
            return 0;
        }
    }
}