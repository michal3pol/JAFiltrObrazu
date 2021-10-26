#pragma once
#include <vector>
#include <string>
#include <cstddef>


//odczytywanie informacji o pliku
class Image
{
private:
    int _width;
    int _height;
    int _fileSize;
    wchar_t* _path;
    //std::vector<unsigned char> _colors;

    unsigned char* _colors; 
    unsigned char* _colorsFilter;

    unsigned char fileHeader[14];
    unsigned char informationHeader[40];

public:
    ~Image();
    Image();

    void Read();
    void SetColor(float r, float g, float b, int x, int y);
    //Color GetColor(int x, int y) const;
    void Save();
    void SetImage(wchar_t* n);

    unsigned char* GetColorsPtr() { return _colors; }
    unsigned char* GetColorsFilterPtr() { return _colorsFilter; }

    int GetHeight() { return _height; }
    int GetWidth() { return _width; }


};