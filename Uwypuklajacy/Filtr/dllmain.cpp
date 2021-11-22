// dllmain.cpp : Definiuje punkt wejścia dla aplikacji DLL.
#include "pch.h"

//BIBLIOTEKA Z FUNKCJAMI C++

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
                     )
{
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}


#define EOF (-1)

#ifdef __cplusplus    // If used by C++ code, 
extern "C" {          // we need to export the C interface
#endif

    __declspec(dllexport) void embossingFilter(unsigned char* colors, unsigned char* colors_filtered,int start_height, int stop_height, int width, int height)
    {
        int actualRow = start_height;
        bool last = false;
        int paddingAmount = ((4 - (width * 3) % 4) % 4);
        int row = (width*3) + paddingAmount;

        if (start_height == 0) //first row
        {
            //left corner + row
            for (int i = 0; i < (width * 3); i++)
                colors_filtered[i] = colors[i + row + 3];
            //corner
            for (int i = (width * 3) - 3; i < (width * 3); i++) //one pixel - b,g and r
                colors_filtered[i] = colors[i]; 
            //padding
            for (int i = (width * 3); i < row; i++)
                colors_filtered[i] = 0;
            actualRow++;
         }
        //another rows
        if (stop_height == height)
        {
            stop_height--;
            last = true;
        }
        for (int j = actualRow; j <= stop_height-1 ; j++)
        {
            //1st pixel in row
            for (int i = j*row; i < j *row + 3; i++) //one pixel - b,g and r
                colors_filtered[i] = colors[i + row + 3];
            //row
            for (int i = j * row + 3; i < j * row + (width * 3) - 3; i++)
                colors_filtered[i] = -colors[i - row - 3] + colors[i + row + 3];
            //last pixel in row
            for (int i = j * row + (width * 3) - 3; i < j * row + (width * 3); i++) //one pixel - b,g and r
                colors_filtered[i] = -colors[i - row - 3];
            //padding
            for (int i = j * row + (width * 3); i < j*row; i++)
                colors_filtered[i] = 0;
        }
        if (last)
        {
            //corner
            for (int i = row * (height - 1); i < (row * (height - 1)) + 3; i++) //one pixel - b,g and r
                colors_filtered[i] = colors[i];
            //row + corner
            for (int i = row * (height - 1) + 3; i < row * (height - 1) + (width * 3); i++)
                colors_filtered[i] = -colors[i - row - 3];
            //padding
            for (int i = height * row - paddingAmount; i < height * row; i++)
                colors_filtered[i] = 0;
        }

    }

#ifdef __cplusplus
}
#endif