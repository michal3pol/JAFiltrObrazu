// Języki Asemblerowe
// Filtrowanie obrazu - filtr uwypuklający
// Każda ze składowych obrazu RGB przekształcana jest przez maskę, tak aby wykryć krawędzie w obrazie.Efekt powinien być porównywalny do płaskorzeźby
// 11.01.2022 r.Rok akademicki : 3, semestr : 5, grupa : V
// Autor: Michał Foks
// wersja: 1.0
// 
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
        int paddingAmount = ((4 - (width * 3) % 4) % 4);
        int row = (width*3) + paddingAmount;
        bool last_row = false;

        if (start_height == 0) {
            //left corner + row
            for (int i = 0; i < (width * 3) - 3; i++)
                colors_filtered[i] = colors[i + row + 3];
            //corner, last pixel
            int i = (width * 3) - 3;
            colors_filtered[i] = colors[i];
            i++;
            colors_filtered[i] = colors[i];
            i++;
            colors_filtered[i] = colors[i];
            actualRow++;
         }
        //another rows
        if (stop_height == height) {
            stop_height--;
            last_row = true;
        }
        for (int j = actualRow; j < stop_height ; j++){
            int i = j * row;
            //first pixel
            colors_filtered[i] = colors[i + row + 3];
            i++;
            colors_filtered[i] = colors[i + row + 3];
            i++;
            colors_filtered[i] = colors[i + row + 3];
            i++;
            //row
            for (; i < j * row + (width * 3) - 3; i++)
                colors_filtered[i] = -colors[i - row - 3] + colors[i + row + 3];
            //last pixel in row
            colors_filtered[i] = -colors[i - row - 3];
            i++;
            colors_filtered[i] = -colors[i - row - 3];
            i++; 
            colors_filtered[i] = -colors[i - row - 3];
        }
        if (last_row){
            //corner
            int i = row * (height - 1);
            colors_filtered[i] = colors[i];
            i++;
            colors_filtered[i] = colors[i];
            i++;
            colors_filtered[i] = colors[i];
            i++;
            //row + corner
            for (; i < row * (height - 1) + (width * 3); i++)
                colors_filtered[i] = colors[i - row - 3];
        }

    }

#ifdef __cplusplus
}
#endif