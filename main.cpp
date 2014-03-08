/*
 *Name:          Anthony Walker
 *File:          pr1.cpp
 *Instructor:    Brygg Ullmer
 *Class/Section: CS4103
 *LogonID:       CS410329
 */

#include <vector>
#include <math.h>
#include "cscbitmap.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

int sobel_x[3][3] = { { 1, 0,-1},
                      { 2, 0,-2},
                      { 1, 0,-1}};

int sobel_y[3][3] = { { 1, 2, 1},
                      { 0, 0, 0},
                      {-1,-2,-1}};

unsigned int height, w, Num_of_Threads;

typedef struct {
    unsigned int start;
    unsigned int end;
} parameter;

/// Declaration of functions.
void* findEdge(void*n);

/// Memory to hold input image data
unsigned char* inData;
std::vector<unsigned char> image_sobeled;

int main(int argc, char *argv[]){
    char* bmpFile;
    char* threads;
    if( argc < 3){
        bmpFile = "4103.bmp";
        printf("Taking 4103.bmp file at current diretory!\n");
        threads = "1";
      }
    /*
    else if(argc < 3)
    {
        bmpFile = argv[1];
        threads = "1";
    }
     */
    else{
      bmpFile = argv[1];
      threads = argv[2];
    }
    
    Bitmap *image = new Bitmap();
    unsigned char*data = image->readGrayBitmap(bmpFile);

    image_sobeled.clear();
    image_sobeled.resize(image->bmpSize, 255);
    inData = data;
    
    //Num_of_Threads = 2;
    Num_of_Threads = atoi(threads);
    //prints to make sure number of threads and arguments match up
    //printf("%d  %s\n", Num_of_Threads, threads);
    height = image->bmpHeight;
    w = image->bmpWidth;
    
    //printf("%d\n",height);
    //printf("%d\n",w);
    
    pthread_t Threadder[Num_of_Threads];
    parameter param[Num_of_Threads];
    
    for(int i = 0; i < Num_of_Threads; ++i){
        param[i].start = (height/Num_of_Threads) * i;
        param[i].end = param[i].start + (height/Num_of_Threads);
        //checks start and ending parameter
        //printf("%d %d %d\n", i,param[i].start, param[i].end);
        pthread_create(&Threadder[i], NULL, findEdge, (void*)&param[i]);
    }
    
    for(int i = 0; i < Num_of_Threads; ++i){
        pthread_join(Threadder[i], NULL);
    }
    /// Write image data passed as argument to a bitmap file
    image->writeGrayBmp(&image_sobeled[0]);
    image_sobeled.clear();
    delete data;

    return 0;
}

/// Function that implements Sobel operator.
/// Returns image data after applyig Sobel operator to the original image.
/// Reimplement findEdge such that it will run in a single thread
/// and can process on a region/group of pixels
//void* findEdge (struct threadParam)
void* findEdge(void* n)
{
    int gradient_X = 0;
    int gradient_Y = 0;
    int value = 0;

    // The FOR loop apply Sobel operator
    // to bitmap image data in per-pixel level.
    parameter* param = (parameter*)n;
    unsigned int starter = param->start;
    unsigned int ender = param->end;
    
    for(unsigned int y = starter+1; y < ender+1; ++y)
        for(unsigned int x = 1; x < w-1; ++x)
        {
            // Compute gradient in +ve x direction
            gradient_X = sobel_x[0][0] * inData[ (x-1) + (y-1) * w ]
                    + sobel_x[0][1] * inData[  x    + (y-1) * w ]
                    + sobel_x[0][2] * inData[ (x+1) + (y-1) * w ]
                    + sobel_x[1][0] * inData[ (x-1) +  y    * w ]
                    + sobel_x[1][1] * inData[  x    +  y    * w ]
                    + sobel_x[1][2] * inData[ (x+1) +  y    * w ]
                    + sobel_x[2][0] * inData[ (x-1) + (y+1) * w ]
                    + sobel_x[2][1] * inData[  x    + (y+1) * w ]
                    + sobel_x[2][2] * inData[ (x+1) + (y+1) * w ];

            gradient_Y = sobel_y[0][0] * inData[ (x-1) + (y-1) * w ]
                    + sobel_y[0][1] * inData[  x    + (y-1) * w ]
                    + sobel_y[0][2] * inData[ (x+1) + (y-1) * w ]
                    + sobel_y[1][0] * inData[ (x-1) +  y    * w ]
                    + sobel_y[1][1] * inData[  x    +  y    * w ]
                    + sobel_y[1][2] * inData[ (x+1) +  y    * w ]
                    + sobel_y[2][0] * inData[ (x-1) + (y+1) * w ]
                    + sobel_y[2][1] * inData[  x    + (y+1) * w ]
                    + sobel_y[2][2] * inData[ (x+1) + (y+1) * w ];

            value = (int)ceil(sqrt( gradient_X * gradient_X +
                                    gradient_Y * gradient_Y));
            image_sobeled[ x + y*w ] = 255 - value;
        }
    
    pthread_exit(0);
    return 0;
}
