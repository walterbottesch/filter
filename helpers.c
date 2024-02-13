#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int average = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int sepiaRed, sepiaGreen, sepiaBlue;
            int originalRed = image[i][j].rgbtRed;
            int originalGreen = image[i][j].rgbtGreen;
            int originalBlue = image[i][j].rgbtBlue;

            sepiaRed = (int)(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            sepiaGreen = (int)(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            sepiaBlue =(int) (.272 * originalRed + .534 * originalGreen + .131 * originalBlue);

            image[i][j].rgbtBlue = fmin(255, fmax(0, sepiaBlue));
            image[i][j].rgbtGreen = fmin(255, fmax(0, sepiaGreen));
            image[i][j].rgbtRed = fmin(255, fmax(0, sepiaRed));
        }   
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width / 2; j++) {
            RGBTRIPLE temp = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1] = temp;
        }   
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width]) {
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int count = 0;
            int totalRed = 0;
            int totalGreen = 0;
            int totalBlue = 0;

            for (int k = i - 1; k < i + 2; k++) {
                for (int p = j - 1; p < j + 2; p++) {
                    if ((k >= 0) && (k < height) && (p >= 0) && (p < width)) {
                    totalBlue += image[k][p].rgbtBlue;
                    totalGreen += image[k][p].rgbtGreen;
                    totalRed += image[k][p].rgbtRed;
                    count++;
                    }
                }
            }
            image[i][j].rgbtBlue = totalBlue / count;
            image[i][j].rgbtGreen = totalGreen / count;
            image[i][j].rgbtRed = totalRed / count;
        } 
    }  
}

int Gx[3][3] = { {-1, 0, 1},
                {-2, 0, 2},
                {-1, 0, 1}};

int Gy [3][3] = { {-1, -2, 1},
                { 0,  0, 0},
                { 1,  2, 1}};



void edges(int height, int width, RGBTRIPLE image[height][width]) {
    RGBTRIPLE new_image[height][width];
    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            int Gx_red = 0, Gx_green = 0, Gx_blue = 0;
            int Gy_red = 0, Gy_green = 0, Gy_blue = 0;

            for (int k = i - 1; k < i + 2; k++) {
                for (int p = j - 1; p < j + 2; p++) {
                    if ((k >= 0) && (k < height) && (p >= 0) && (p < width)) {
                    Gx_red += image[k][p].rgbtRed * Gx[k - i + 1][p - j + 1];
                    Gx_green += image[k][p].rgbtGreen * Gx[k - i + 1][p - j + 1];
                    Gx_blue += image[k][p].rgbtBlue * Gx[k - i + 1][p - j + 1];

                    Gy_red += image[k][p].rgbtRed * Gy[k - i + 1][p - j + 1];
                    Gy_green += image[k][p].rgbtGreen * Gy[k - i + 1][p - j + 1];
                    Gy_blue += image[k][p].rgbtBlue * Gy[k - i + 1][p - j + 1];
                    }
                }
            }
            int final_red = round(sqrt(Gx_red * Gx_red + Gy_red * Gy_red));
            int final_green = round(sqrt(Gx_green * Gx_green + Gy_green * Gy_green));
            int final_blue = round(sqrt(Gx_blue * Gx_blue + Gy_blue * Gy_blue));

            new_image[i][j].rgbtRed = (final_red > 255) ? 255 : final_red;
            new_image[i][j].rgbtGreen = (final_green > 255) ? 255 : final_green;
            new_image[i][j].rgbtBlue = (final_blue > 255) ? 255 : final_blue;

        } 
    } 

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j] = new_image[i][j];
        } 
    }
}