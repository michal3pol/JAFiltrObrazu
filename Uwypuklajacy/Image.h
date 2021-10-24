#pragma once
#include <vector>
#include <string>

//odczytywanie informacji o pliku
class Image
{
private:
    int _width;
    int _height;
    int _fileSize;
    wchar_t* _path;
    std::vector<float> _colors;

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

    const int GetHeight() { return _height; }

    void filter(int start, int end);

};