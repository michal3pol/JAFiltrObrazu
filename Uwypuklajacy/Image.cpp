// Jêzyki Asemblerowe
// Filtrowanie obrazu - filtr uwypuklaj¹cy
// Ka¿da ze sk³adowych obrazu RGB przekszta³cana jest przez maskê, tak aby wykryæ krawêdzie w obrazie.Efekt powinien byæ porównywalny do p³askorzeŸby
// 11.01.2022 r.Rok akademicki : 3, semestr : 5, grupa : V
// Autor: Micha³ Foks
// wersja: 1.0
#include "Image.h"
#include <fstream>

#include "atlbase.h"
#include "atlstr.h"
#include "comutil.h"

Image::Image()
{ 
}

void Image::SetImage(wchar_t* n)
{
	_path = n;
}

Image::~Image()
{
}

void Image::SetColor(float r, float g, float b, int x, int y)
{

}

void Image::Read()
{
	//wyzeruj pamiec jesli odczytujesz nowy plik
	if(_colors)
	{
		delete[]_colors;
	}
	std::ifstream f;
	CStringA path(_path);
	f.open(path, std::ios::in | std::ios::binary);
	if (!f.is_open())
		return;

	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;
	f.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

	f.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	_fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);

	_width = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);
	_height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] << 24);

	//czytanie - przeniesc do innnej funkcji podzielic miedzy watki
	const int paddingAmount = ((4 - (_width * 3) % 4) % 4);
	
	int size = (_width + paddingAmount) * _height * 3;
	_colors = new unsigned char[size];
	_colorsFilter = new unsigned char[size];
	f.read((char*)_colors, size);
	f.close();

	/*int row = (_width * 3) + paddingAmount;
	//szarosc
	for (int j = 0; j < _height; j++) {
		for (int i = j * row; i < j * row + (_width * 3); i += 3) {

			_colors[i] = _colors[i] * 0.299 + _colors[i + 1] * 0.587 + _colors[i + 2] * 0.114;
			_colors[i + 1] = _colors[i];
			_colors[i + 2] = _colors[i];

		}
	}*/
	
}

void Image::Save()
{
	std::ofstream f;

	CStringA path(_path);
	path.Replace(".bmp", "_filter.bmp");
	f.open(path, std::ios::out | std::ios::binary);
	if (!f.is_open())
	{
		//nie otwarto pliku
		return;
	}

	unsigned char bmpPad[3] = { 0,0,0 };
	const int paddingAmount = ((4 - (_width * 3) % 4) % 4);

	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;
	_fileSize = fileHeaderSize + informationHeaderSize + _width * _height * 3 +paddingAmount *_height;

	//file type, size
	fileHeader[0] = 'B';
	fileHeader[1] = 'M';
	fileHeader[2] = _fileSize;
	fileHeader[3] = _fileSize >> 8;
	fileHeader[4] = _fileSize >> 16;
	fileHeader[5] = _fileSize >> 24;
	fileHeader[6] = 0;
	fileHeader[7] = 0;
	fileHeader[8] = 0;
	fileHeader[9] = 0;
	fileHeader[10] = fileHeaderSize + informationHeaderSize;
	fileHeader[11] = 0;
	fileHeader[12] = 0;
	fileHeader[13] = 0;

	//header
	informationHeader[0] = informationHeaderSize;
	informationHeader[1] = 0;
	informationHeader[2] = 0;
	informationHeader[3] = 0;
	informationHeader[4] = _width; //width informarmation
	informationHeader[5] = _width >>8;
	informationHeader[6] = _width >>16;
	informationHeader[7] = _width >> 24;
	informationHeader[8] = _height; //height informarmation
	informationHeader[9] = _height >> 8;
	informationHeader[10] = _height >> 16;
	informationHeader[11] = _height >> 24;
	informationHeader[12] = 1; //planes
	informationHeader[13] = 0;
	informationHeader[14] = 24; //bit per pixel
	for (int i = 15; i <= 39; i++)
		informationHeader[i] = 0;

	f.write(reinterpret_cast<char*>(fileHeader), fileHeaderSize);
	f.write(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	int size = (_width + paddingAmount) * _height * 3;
	f.write((char*)_colorsFilter, size);

	f.close();
}

