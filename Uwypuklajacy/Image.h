// Jêzyki Asemblerowe
// Filtrowanie obrazu - filtr uwypuklaj¹cy
// Ka¿da ze sk³adowych obrazu RGB przekszta³cana jest przez maskê, tak aby wykryæ krawêdzie w obrazie.Efekt powinien byæ porównywalny do p³askorzeŸby
// 11.01.2022 r.Rok akademicki : 3, semestr : 5, grupa : V
// Autor: Micha³ Foks
// wersja: 1.0

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
    bool SetImage(wchar_t* n);

    unsigned char* GetColorsPtr() { return _colors; }
    unsigned char* GetColorsFilterPtr() { return _colorsFilter; }

    int GetHeight() { return _height; }
    int GetWidth() { return _width; }


};