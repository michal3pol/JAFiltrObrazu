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

void Image::filter(int start, int end)
{
	//imitacja funkcji z dll C i asm
}

Image::~Image()
{
}

void Image::SetColor(float r, float g, float b, int x, int y)
{

}

void Image::Read()
{
	std::ifstream f;
	CStringA cstringa(_path);
	f.open(cstringa, std::ios::in | std::ios::binary);

	if (!f.is_open())
	{
		return;
	}

	const int fileHeaderSize = 14;
	const int informationHeaderSize = 40;
	f.read(reinterpret_cast<char*>(fileHeader), fileHeaderSize);

	f.read(reinterpret_cast<char*>(informationHeader), informationHeaderSize);

	_fileSize = fileHeader[2] + (fileHeader[3] << 8) + (fileHeader[4] << 16) + (fileHeader[5] << 24);

	_width = informationHeader[4] + (informationHeader[5] << 8) + (informationHeader[6] << 16) + (informationHeader[7] << 24);
	_height = informationHeader[8] + (informationHeader[9] << 8) + (informationHeader[10] << 16) + (informationHeader[11] << 24);

	//czytanie - przeniesc do innnej funkcji podzielic miedzy watki
	const int paddingAmount = ((4 - (_width * 3) % 4) % 4);
	_colors.resize(_width * _height * 3);//*3 bo rgb


	int indeks = 0;
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			unsigned char color[3];
			f.read(reinterpret_cast<char*>(color), 3);

			_colors[ indeks] = static_cast<float>(color[0]); //b
			indeks++;
			_colors[indeks] = static_cast<float>(color[1]); //g
			indeks++;
			_colors[ indeks] = static_cast<float>(color[2]); //r
			indeks++;
		}
		f.ignore(paddingAmount);
	}
	f.close();
}

void Image::Save()
{
	std::ofstream f;

	CStringA cstringa(_path);
	f.open("proba44.bmp", std::ios::out | std::ios::binary);
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

	int indeks = 0;
	for (int y = 0; y < _height; y++)
	{
		for (int x = 0; x < _width; x++)
		{
			unsigned char b = static_cast<unsigned char>(_colors[ indeks]); //b
			indeks++;
			unsigned char g = static_cast<unsigned char>(_colors[ indeks]); //g
			indeks++;
			unsigned char r = static_cast<unsigned char>(_colors[ indeks]); //r
			indeks++;

			unsigned char color[] = { b, g, r };
			f.write(reinterpret_cast<char*>(color), 3);
		}
		f.write(reinterpret_cast<char*>(bmpPad), paddingAmount);
	}

	f.close();
}

