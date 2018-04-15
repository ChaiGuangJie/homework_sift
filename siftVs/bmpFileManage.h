#pragma once

#define BIT_24   24
#define BIT_8    8
#define BIT_4    4
#define BIT_1    1

typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef unsigned short WORD;

/*
λͼͷ�ṹ
*/
#pragma pack(1)
typedef struct tagBITMAPFILEHEADER
{
	BYTE bfType[2];//�ļ���ʽ
	DWORD bfSize;//�ļ���С
	WORD bfReserved1;//����
	WORD bfReserved2;
	DWORD bfOffBits; //DIB�������ļ��е�ƫ����
}fileHeader;
#pragma pack()

/*
λͼ������Ϣ�ṹ
*/
#pragma pack(1)
typedef struct tagBITMAPINFOHEADER
{
	DWORD biSize;//�ýṹ�Ĵ�С
	DWORD biWidth;//�ļ����
	DWORD biHeight;//�ļ��߶�
	WORD biPlanes;//ƽ����
	WORD biBitCount;//��ɫλ��
	DWORD biCompression;//ѹ������
	DWORD biSizeImage;//DIB��������С
	DWORD biXPixPerMeter;
	DWORD biYPixPerMeter;
	DWORD biClrUsed;//������ɫ������
	DWORD biClrImporant;//������Ҫ��ɫ
}fileInfo;
#pragma pack()

/*
��ɫ��ṹ
*/
#pragma pack(1)
typedef struct tagRGBQUAD
{
	BYTE rgbBlue; //��ɫ��������
	BYTE rgbGreen;//��ɫ��������
	BYTE rgbRed;//��ɫ��������
	BYTE rgbReserved;
}rgbq;
#pragma pack()

typedef struct bmpImg
{
	fileHeader *fHead;
	fileInfo *fInfo;
	rgbq *rgbqList;
	WORD rgbqCount;
	BYTE *pixelData;
}bmpImg;//todo �ǵ��ͷ�


typedef struct bmpArray
{
	BYTE *pixelArray;
	DWORD width;
	DWORD height;
	DWORD realWidth;
	WORD bitCountOfPixel;
}bmpArray;

typedef enum color
{
	red = 0, green, blue, gray, all

}singleColor;

bmpImg* readBmp(const char* fileName);

int writeBmp(const bmpImg* bmp, const char* fileName);

bmpArray* bmpArrBuild(const bmpImg* bmp, singleColor color);

bmpImg* bmpImgBuild(const bmpArray* bmpArr);