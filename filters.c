#include <stdint.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>


// BMP-related data types based on Microsoft's own

/**
 * BITMAPFILEHEADER
 *
 * The BITMAPFILEHEADER structure contains information about the type, size,
 * and layout of a file that contains a DIB [device-independent bitmap].
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/dd183374(VS.85).aspx.
 */

#pragma pack(push, 1)

typedef struct
{
    uint16_t  bfType;
    uint32_t  bfSize;
    uint16_t  bfReserved1;
    uint16_t  bfReserved2;
    uint32_t  bfOffBits;
} BITMAPFILEHEADER;

#pragma pack(pop)

/**
 * BITMAPINFOHEADER
 *
 * The BITMAPINFOHEADER structure contains information about the
 * dimensions and color format of a DIB [device-independent bitmap].
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/dd183376(VS.85).aspx.
 */

#pragma pack(push, 1)

typedef struct
{
    uint32_t  biSize;
    int32_t   biWidth;
    int32_t   biHeight;
    uint16_t  biPlanes;
    uint16_t  biBitCount;
    uint32_t  biCompression;
    uint32_t  biSizeImage;
    int32_t   biXPelsPerMeter;
    int32_t   biYPelsPerMeter;
    uint32_t  biClrUsed;
    uint32_t  biClrImportant;
} BITMAPINFOHEADER;

#pragma pack(pop)

/**
 * RGBTRIPLE
 *
 * This structure describes a color consisting of relative intensities of
 * red, green, and blue.
 *
 * Adapted from http://msdn.microsoft.com/en-us/library/aa922590.aspx.
 */

#pragma pack(push, 1)

typedef struct
{
    uint8_t  rgbtBlue;
    uint8_t  rgbtGreen;
    uint8_t  rgbtRed;
} RGBTRIPLE;

#pragma pack(pop)

// Apply Sepia filter to image
void sepia(int height, int width, RGBTRIPLE **image) {

    int i,j,k;
    float A[3];

    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            A[0]=(image[i][j].rgbtRed*0.393)+(image[i][j].rgbtBlue*0.769)+(image[i][j].rgbtGreen*0.189);   // r,g,b values being multiplied using a specific formula to get vintage look
            A[1]=(image[i][j].rgbtRed*0.349)+(image[i][j].rgbtBlue*0.686)+(image[i][j].rgbtGreen*0.168);
            A[2]=(image[i][j].rgbtRed*0.272)+(image[i][j].rgbtBlue*0.534)+(image[i][j].rgbtGreen*0.131);
            for(k=0;k<3;k++)
            {
                if (A[k]>255)
                {
                    A[k]=255;
                }
                if (A[k]<((int)A[k]+0.5))
                {
                    A[k]=(int)A[k];
                }
                else
                {
                    A[k]=(int)A[k]+1;
                }

                switch (k){
                    case 0:
                        image[i][j].rgbtRed=A[0];
                        break ;
                    case 1 :
                        image[i][j].rgbtBlue=A[1];
                        break ;
                    case 2 :
                        image[i][j].rgbtGreen=A[2];
                        break;
                }
            }

        }
    }
    return;
}

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE **image)
{
    float av;
    int A1,i,j;
    for(i=0;i<height;i++)
    {
        for(j=0;j<width;j++)
        {
            av=image[i][j].rgbtRed+image[i][j].rgbtGreen+image[i][j].rgbtBlue;
            av=av/3;
            A1=av;
            if (av<(A1+0.5))
            {
                A1=av ;
            }
            else
            {
                A1=(av+1);
            }
            image[i][j].rgbtRed=A1;
            image[i][j].rgbtGreen=A1;
            image[i][j].rgbtBlue=A1;
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE **image)
{
    int choice;
    printf("How do you want to reflect the image? : \n\n1.Horizontally\n2.Vertically\n");
    scanf("%d", &choice);

    int i,j;
    RGBTRIPLE temp;

    switch(choice) {
        case 1:

            for(i=0;i<height;i++)
            {
                for(j=0;j<width/2;j++)
                {
                    temp=image[i][j];
                    image[i][j]=image[i][width-1-j];
                    image[i][width-1-j]=temp;
                }
            }
            break;

        case 2:

            for(i=0;i<height/2;i++)
            {
                for(j=0;j<width;j++)
                {
                    temp=image[i][j];
                    image[i][j]=image[height-1-i][j];
                    image[height-1-i][j]=temp;
                }
            }
            break;

        default:
            printf("Invalid choice\n");
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE **image)
{
    RGBTRIPLE **temp = malloc(height * sizeof(RGBTRIPLE*));
    for(int i = 0; i < height; i++) {
        temp[i] = malloc(width * sizeof(RGBTRIPLE));
    }

    for (int i = 1; i < height-1; i++) {
        for (int j = 1; j < width-1; j++) {
            temp[i][j].rgbtRed = (image[i-1][j-1].rgbtRed + image[i-1][j].rgbtRed + image[i-1][j+1].rgbtRed +
                                 image[i][j-1].rgbtRed + image[i][j].rgbtRed + image[i][j+1].rgbtRed +
                                 image[i+1][j-1].rgbtRed + image[i+1][j].rgbtRed + image[i+1][j+1].rgbtRed)/9;

            temp[i][j].rgbtGreen = (image[i-1][j-1].rgbtGreen + image[i-1][j].rgbtGreen + image[i-1][j+1].rgbtGreen +
                                 image[i][j-1].rgbtGreen + image[i][j].rgbtGreen + image[i][j+1].rgbtGreen +
                                 image[i+1][j-1].rgbtGreen + image[i+1][j].rgbtGreen + image[i+1][j+1].rgbtGreen)/9;

            temp[i][j].rgbtBlue = (image[i-1][j-1].rgbtBlue + image[i-1][j].rgbtBlue + image[i-1][j+1].rgbtBlue +
                                 image[i][j-1].rgbtBlue + image[i][j].rgbtBlue + image[i][j+1].rgbtBlue +
                                 image[i+1][j-1].rgbtBlue + image[i+1][j].rgbtBlue + image[i+1][j+1].rgbtBlue)/9;
        }
    }

    for (int i = 1; i < height-1; i++) {
        for (int j = 1; j < width-1; j++) {
            image[i][j] = temp[i][j];
        }
    }

    free(temp);

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE **image) {

    RGBTRIPLE **copy = malloc(height * sizeof(RGBTRIPLE*));
    for(int i = 0; i < height; i++) {
        copy[i] = malloc(width * sizeof(RGBTRIPLE));
    }

    int Gx_red, Gx_blue, Gx_green;
    int Gy_red, Gy_blue, Gy_green;

    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            Gx_red = image[i-1][j-1].rgbtRed * -1 + image[i-1][j+1].rgbtRed +
                     image[i][j-1].rgbtRed * -2 + image[i][j+1].rgbtRed * 2 +
                     image[i+1][j-1].rgbtRed * -1 + image[i+1][j+1].rgbtRed;

            Gx_green = image[i-1][j-1].rgbtGreen * -1 + image[i-1][j+1].rgbtGreen +
                       image[i][j-1].rgbtGreen * -2 + image[i][j+1].rgbtGreen * 2 +
                       image[i+1][j-1].rgbtGreen * -1 + image[i+1][j+1].rgbtGreen;

            Gx_blue = image[i-1][j-1].rgbtBlue * -1 + image[i-1][j+1].rgbtBlue +
                      image[i][j-1].rgbtBlue * -2 + image[i][j+1].rgbtBlue * 2 +
                      image[i+1][j-1].rgbtBlue * -1 + image[i+1][j+1].rgbtBlue;

            Gy_red = image[i-1][j-1].rgbtRed * -1 + image[i-1][j].rgbtRed * -2 + image[i-1][j+1].rgbtRed * -1 +
                     image[i+1][j-1].rgbtRed + image[i+1][j].rgbtRed * 2 + image[i+1][j+1].rgbtRed;

            Gy_blue = image[i-1][j-1].rgbtBlue * -1 + image[i-1][j].rgbtBlue * -2 + image[i-1][j+1].rgbtBlue * -1 +
                      image[i+1][j-1].rgbtBlue + image[i+1][j].rgbtBlue * 2 + image[i+1][j+1].rgbtBlue;

            Gy_green = image[i-1][j-1].rgbtGreen * -1 + image[i-1][j].rgbtGreen * -2 + image[i-1][j+1].rgbtGreen * -1 +
                      image[i+1][j-1].rgbtGreen + image[i+1][j].rgbtGreen * 2 + image[i+1][j+1].rgbtGreen;

            (sqrt(Gx_red*Gx_red + Gy_red*Gy_red) < 256) ? (copy[i][j].rgbtRed = sqrt(Gx_red*Gx_red + Gy_red*Gy_red)) : (copy[i][j].rgbtRed = 255);
            (sqrt(Gx_green*Gx_green + Gy_green*Gy_green) < 256) ? (copy[i][j].rgbtGreen = sqrt(Gx_green*Gx_green + Gy_green*Gy_green)) : (copy[i][j].rgbtGreen = 255);
            (sqrt(Gx_blue*Gx_blue + Gy_blue*Gy_blue) < 256) ? (copy[i][j].rgbtBlue = sqrt(Gx_blue*Gx_blue + Gy_blue*Gy_blue)) : (copy[i][j].rgbtBlue = 255);
        }
    }

    for (int i = 1; i < height - 1; i++) {
        for (int j = 1; j < width - 1; j++) {
            image[i][j] = copy[i][j];
        }
    }
    free(copy);

    return;
}

void invert(int height, int width, RGBTRIPLE **image) {

    for (int i = 0; i < height; i++) {
        for (int j = 0; j < width; j++) {
            image[i][j].rgbtBlue = 255 - image[i][j].rgbtBlue;
            image[i][j].rgbtGreen = 255 - image[i][j].rgbtGreen;
            image[i][j].rgbtRed = 255 - image[i][j].rgbtRed;
        }
    }
    return;
}


void grain(int height, int width, RGBTRIPLE **image) {

    int intensity;
    printf("Enter the intensity(1-10) :\n\n");
    scanf("%d", &intensity);

    int i, j;
    for (i = 0; i < height; i++) {
        for (j = 0; j < width; j++) {

            int noiseRed = (rand() % 10) * intensity;
            int noiseGreen = (rand() % 10) * intensity;
            int noiseBlue = (rand() % 10) * intensity;


            if (image[i][j].rgbtRed + noiseRed < 255) image[i][j].rgbtRed += noiseRed;
            if (image[i][j].rgbtGreen + noiseBlue < 255) image[i][j].rgbtGreen += noiseGreen;
            if (image[i][j].rgbtBlue + noiseGreen < 255) image[i][j].rgbtBlue +=noiseBlue;
        }
    }

    return;
}


int main()
{


    // Remember filenames
    char *infile = (char *)malloc(200);
    char *outfile = (char *)malloc(200);

    printf("Enter the input file's name: ");
    scanf("%s", infile);
    printf("Enter the output file's name: ");
    scanf("%s", outfile);

    // Open input file
    FILE *inptr = fopen(infile, "r");
    if (inptr == NULL)
    {
        printf("Could not open %s.\n", infile);
        return 4;
    }

    // Open output file
    FILE *outptr = fopen(outfile, "w");
    if (outptr == NULL)
    {
        fclose(inptr);
        printf("Could not create %s.\n",    outfile);
        return 5;
    }

    // Read infile's BITMAPFILEHEADER
    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, inptr);

    // Read infile's BITMAPINFOHEADER
    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, inptr);

    // Ensure infile is (likely) a 24-bit uncompressed BMP 4.0
    if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
        bi.biBitCount != 24 || bi.biCompression != 0)
    {
        fclose(outptr);
        fclose(inptr);
        printf("Unsupported file format.\n");
        return 6;
    }

    // Get image's dimensions
    int height = abs(bi.biHeight);
    int width = bi.biWidth;

    // Allocate memory for image
    RGBTRIPLE **image = malloc(height * sizeof(RGBTRIPLE*));
    for (int i = 0; i < height; i++) {
        image[i] = malloc(width*sizeof(RGBTRIPLE));
    }

    if (image == NULL)
    {
        printf("Not enough memory to store image.\n");
        fclose(outptr);
        fclose(inptr);
        return 7;
    }

    // Determine padding for scanlines
    int padding = (4 - (width * sizeof(RGBTRIPLE)) % 4) % 4;

    // Iterate over infile's scanlines
    for (int i = 0; i < height; i++)
    {
        // Read row into pixel array
        fread(image[i], sizeof(RGBTRIPLE), width, inptr);

        // Skip over padding
        fseek(inptr, padding, SEEK_CUR);
    }

    // Get filter flag and check validity
    int filter;
    printf("Enter the filter you want to apply to your .bmp image: ");
    printf("\n\n1.blur\n2.edge detection\n3.grayscale\n4.reflect\n5.invert\n6.graining\n7.sepia\n\n");
    scanf("%d", &filter);

    // Filter image
    switch (filter)
    {
        // Blur
        case 1:
            blur(height, width, image);
            break;

        // Edges
        case 2:
            edges(height, width, image);
            break;

        // Grayscale
        case 3:
            grayscale(height, width, image);
            break;

        // Reflect
        case 4:
            reflect(height, width, image);
            break;

        case 5:
            invert(height, width, image);
            break;

        case 6:
            grain(height, width, image);
            break;

        case 7:
            sepia(height, width, image);
            break;

        default:
            printf("Invalid filter.");
            return 8;
    }

    // Write outfile's BITMAPFILEHEADER
    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, outptr);

    // Write outfile's BITMAPINFOHEADER
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, outptr);

    // Write new pixels to outfile
    for (int i = 0; i < height; i++)
    {
        // Write row to outfile
        fwrite(image[i], sizeof(RGBTRIPLE), width, outptr);

        // Write padding at end of row
        for (int k = 0; k < padding; k++)
        {
            fputc(0x00, outptr);
        }
    }

    // Free memory for image
    for (int i = 0; i < height; i++) {
        free(image[i]);
    }
    free(image);

    // Close files
    fclose(inptr);
    fclose(outptr);
    return 0;
}

