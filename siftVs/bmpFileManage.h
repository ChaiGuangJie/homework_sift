#pragma once

#define BIT_24   24
#define BIT_8    8
#define BIT_4    4
#define BIT_1    1

typedef unsigned long DWORD;
typedef unsigned char BYTE;
typedef unsigned short WORD;

/*
位图头结构
*/
#pragma pack(1)
typedef struct tagBITMAPFILEHEADER
{
	BYTE bfType[2];//文件格式
	DWORD bfSize;//文件大小
	WORD bfReserved1;//保留
	WORD bfReserved2;
	DWORD bfOffBits; //DIB数据在文件中的偏移量
}fileHeader;
#pragma pack()

/*
位图数据信息结构
*/
#pragma pack(1)
typedef struct tagBITMAPINFOHEADER
{
	DWORD biSize;//该结构的大小
	DWORD biWidth;//文件宽度
	DWORD biHeight;//文件高度
	WORD biPlanes;//平面数
	WORD biBitCount;//颜色位数
	DWORD biCompression;//压缩类型
	DWORD biSizeImage;//DIB数据区大小
	DWORD biXPixPerMeter;
	DWORD biYPixPerMeter;
	DWORD biClrUsed;//多少颜色索引表
	DWORD biClrImporant;//多少重要颜色
}fileInfo;
#pragma pack()

/*
调色板结构
*/
#pragma pack(1)
typedef struct tagRGBQUAD
{
	BYTE rgbBlue; //蓝色分量亮度
	BYTE rgbGreen;//绿色分量亮度
	BYTE rgbRed;//红色分量亮度
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
}bmpImg;//todo 记得释放


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