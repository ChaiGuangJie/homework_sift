#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>

typedef unsigned long DWORD;
typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned short WORD;

typedef   struct   tagBITMAPFILEHEADER
{
	WORD bfType;   //   位图文件的类型，必须为BM 
	DWORD bfSize;   //   位图文件的大小，以字节为单位   
	WORD bfReserved1;   //   位图文件保留字，必须为0 
	WORD bfReserved2;   //   位图文件保留字，必须为0 
	DWORD   bfOffBits;   //   位图数据的起始位置，以相对于位图 
						 //   文件头的偏移量表示，以字节为单位 
}BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER
{
	DWORD biSize;
	long biWidth;
	long biHeight;
	WORD biPlanes;
	WORD biBitCount;
	DWORD biCompression;
	DWORD biSizeImage;
	long biXPelsPerMeter;
	long biYPelsPerMeter;
	DWORD biClrUsed;
	DWORD biClrImportant;
} BITMAPINFOHEADER;

typedef struct bmpArray
{
	BYTE *pixelArray;
	DWORD width;
	DWORD height;
	WORD bitCountOfPixel;
}bmpArray;

typedef struct singleColorBmp
{
	BITMAPINFOHEADER bmpHead;
	char *pixelArray; //todo 改名

}singleColorBmp;

int ReadBmp(const char *szFileName);
int GetDIBColor(int X, int Y, BYTE *r, BYTE *g, BYTE *b);
singleColorBmp ConvertToSingleColor(BITMAPINFOHEADER rgb, BYTE *Buffer);
int writerBmpToFile(singleColorBmp bmp, const char *szFileName);

BITMAPINFOHEADER bih;
BYTE *Buffer = NULL;
long LineByteWidth;
//todo 应定义一个变量保存所有动态申请的内存块，最后统一释放。
int main(void)
{
	int x, y;
	BYTE r, g, b;
	int n;
	char szfilename[255] = "1.bmp";
	if (ReadBmp(szfilename) == 0)
	{
		printf("failure to read file %s", szfilename);
		return 1;
	}
	printf("Width: %ld\n", bih.biWidth);
	printf("Height: %ld\n", bih.biHeight);
	printf("BitCount: %d\n\n", (int)bih.biBitCount);

	//singleColorBmp sBmp = ConvertToSingleColor(bih,Buffer);
	//printf("%d", sizeof(sBmp.bmpHead));
	//writerBmpToFile(sBmp, "gray_1.bmp");
	//printf("灰度图像第一个像素值为%d", *sBmp.pixelArray);

	while (1)
	{
		printf("input the X:");
		scanf_s("%d", &x);
		if (x < 0)
			break;
		printf("input the Y:");
		scanf_s("%d", &y);
		if (GetDIBColor(x, y, &r, &g, &b) == 1)
			printf("(%d, %d): r:%d, g:%d, b:%d\n", x, y, (int)r, (int)g, (int)b);
		else
			printf("input error.\n");
	}
	free(Buffer);
	return 0;
}

DWORD localCov(bmpArray bmp, BYTE* locOrigin,const bmpArray kernal)
{
	BYTE* bmpPtr = locOrigin;
	BYTE* kerPtr = kernal.pixelArray;
	DWORD covResult = 0;

	for (size_t i = 0; i < kernal.height * kernal.width; i++)
	{
		covResult += (*kerPtr) * (*bmpPtr);
		kerPtr++;
		if ((i!=0)&& (i % kernal.width == 0)) 
		{
			//方形区域内一行卷积完毕
			bmpPtr = locOrigin + bmp.width;
		}
		//todo 判断是否边界
	}
	return (DWORD)(covResult / kernal.width*kernal.height);
}

bmpArray convBmp(bmpArray bmp, bmpArray kernal)
{
	bmpArray newBmp =
	{
		.pixelArray = NULL,
		.width = 0,
		.height = 0,
		.bitCountOfPixel = 8
	};
	//todo先计算新图像的宽高
	newBmp.width = bmp.width - kernal.width + 1;
	newBmp.height = bmp.height - kernal.height + 1;
	newBmp.pixelArray = (BYTE*)malloc(sizeof(BYTE)*newBmp.width*newBmp.height);
	//
	//DWORD row, col = 0;
	//for (size_t i = 0; i < bmp.width * bmp.width; i++)
	//{
	//	//i为原图像每个像素
	//	//将i转化为row col
	//	//row = i / bmp.width;
	//	//col = i % bmp.width;
	//}
	for (size_t j = 0; j < newBmp.height*newBmp.width; j++)
	{
		*(newBmp.pixelArray + j) = localCov(bmp, bmp.pixelArray + j, kernal); //只要计算好newBmp的图像大小，就不会越界访问？
	}

	return newBmp;
}

bmpArray downSample(bmpArray bmp,int sampSize)
{

}

//求卷积后的局域极值点


//用三次函数拟合后求真正的极值点


//去除边界上的和小于阈值的极值点

//todo 下采样后求得的极值点如何恢复到原图像中？

//将bmp图像写入文件
int writerBmpToFile(singleColorBmp bmp, const char *szFileName)
{
	FILE *file;
	if (0 == fopen_s(&file, szFileName, "wb"))
	{
		fwrite(&bmp.bmpHead, sizeof(bmp.bmpHead),1,file);

		size_t arrSize = bmp.bmpHead.biBitCount / 8 * bmp.bmpHead.biHeight*bmp.bmpHead.biWidth;
		fwrite(bmp.pixelArray, arrSize, 1, file);
		
		fclose(file);
		return 0;
	}
	else
	{
		printf("写入文件出错！");
		return 1;
	}
}

// DWORD biSize,
// long biWidth,
// long biHeight,
// WORD biPlanes,
// WORD biBitCount,
// DWORD biCompression,
// DWORD biSizeImage,
// long biXPelsPerMeter,
// long biYPelsPerMeter,
// DWORD biClrUsed,
// DWORD biClrImportant
void printImgInfo(BITMAPINFOHEADER bmpHead)
{
	printf("imgInfo");
	printf("biSize:%ld\n", bmpHead.biSize);
	printf("biWidth:%ld\n", bmpHead.biWidth);
	printf("biHeight:%ld\n", bmpHead.biHeight);
	printf("biPlanes:%d\n", bmpHead.biPlanes);
	printf("biBitCount:%d\n", bmpHead.biBitCount);
	printf("biCompression:%ld\n", bmpHead.biCompression);
	printf("biSizeImage:%ld\n", bmpHead.biSizeImage);
	printf("biXPelsPerMeter:%ld\n", bmpHead.biXPelsPerMeter);
	printf("biYPelsPerMeter:%ld\n", bmpHead.biYPelsPerMeter);
	printf("biClrUsed:%ld\n", bmpHead.biClrUsed);
	printf("biClrImportant:%ld\n", bmpHead.biClrImportant);
}

int ReadBmp(const char *szFileName)
{
	FILE *file;
	errno_t err;
	WORD bfh[7];
	long dpixeladd;
	
	if ((err = fopen_s(&file, szFileName, "r"))!=0)
	{
		return 0;
	}
	printf("%s\n", szFileName);
	fread(&bfh, sizeof(WORD), 7, file);
	if (bfh[0] != (WORD)(((WORD)'B') | ('M' << 8)))
	{
		fclose(file);
		return 0;
	}
	fread(&bih, sizeof(BITMAPINFOHEADER), 1, file);
	if (bih.biBitCount < 24)
	{
		fclose(file);
		return 0;
	}
	dpixeladd = bih.biBitCount / 8;
	LineByteWidth = bih.biWidth * (dpixeladd);
	if ((LineByteWidth % 4) != 0)
		LineByteWidth += 4 - (LineByteWidth % 4);

	printImgInfo(bih);

	if ((Buffer = (BYTE *)malloc(sizeof(BYTE) * LineByteWidth * bih.biHeight)) != NULL)
	{
		fread(Buffer, LineByteWidth * bih.biHeight, 1, file);
		fclose(file);
		return 1;
	}

	fclose(file);
	return 0;
}

int GetDIBColor(int X, int Y, BYTE *r, BYTE *g, BYTE *b)
{
	int dpixeladd;
	BYTE *ptr;
	if (X < 0 || X >= bih.biWidth || Y < 0 || Y >= bih.biHeight)
	{
		return 0;
	}
	dpixeladd = bih.biBitCount / 8;
	ptr = Buffer + X * dpixeladd + (bih.biHeight - 1 - Y) * LineByteWidth;
	*b = *ptr; 
	*g = *(ptr + 1); //todo 有问题 应该根据dpixel决定加几次
	*r = *(ptr + 2);
	return 1;
}
//将RGB图像转变为灰度图像保存
singleColorBmp ConvertToSingleColor(BITMAPINFOHEADER rgb, BYTE *Buffer)
{
	singleColorBmp bmpArr =
	{
		.bmpHead = NULL,
		.pixelArray = NULL
	};

	if ((sizeof(rgb) == NULL) || (Buffer == NULL))
	{
		return bmpArr;
	}

	BITMAPINFOHEADER head = rgb;

	head.biBitCount = sizeof(BYTE) * 8; //灰度图像用8位
	int dpixeladd = bih.biBitCount / 8;
	int LineByteWidth = bih.biWidth * (dpixeladd);
	if ((LineByteWidth % 4) != 0)
		LineByteWidth += 4 - (LineByteWidth % 4);

	/**
	BITMAPINFOHEADER head =
	{
		.biSize = rgb.biSize,
		.biWidth = rgb.biWidth,
		.biHeight = rgb.biHeight,
		.biPlanes = rgb.biPlanes,
		.biBitCount = sizeof(BYTE),         //灰度图像用8位
		.biCompression = rgb.biCompression,
		.biSizeImage = rgb.biSizeImage,
		.biXPelsPerMeter = rgb.biXPelsPerMeter,
		.biYPelsPerMeter = rgb.biYPelsPerMeter,
		.biClrUsed = rgb.biClrUsed,
		.biClrImportant = rgb.biClrImportant 
	};
	**/
	

	if ((bmpArr.pixelArray = (BYTE *)malloc(sizeof(BYTE) * LineByteWidth * head.biHeight)) != NULL)
	{
		bmpArr.bmpHead = head;

		//todo 填充数组
		BYTE *bufferPtr = Buffer;
		BYTE *pixArrPtr = bmpArr.pixelArray;
		int dpixelCount = bih.biBitCount / 8;
		if (dpixelCount==3) //todo 优化不为3的情况
		{
			BYTE r, g, b;
			for (size_t i = 0; i < rgb.biWidth*rgb.biHeight; i++)
			{
				b = *bufferPtr++;
				g = *bufferPtr++;
				r = *bufferPtr++;

				*pixArrPtr = (BYTE)(r * 0.299 + g * 0.587 + b * 0.114); //三色转灰度 todo:用移位操作避免浮点运算
				*pixArrPtr++;
			}
		}
		
	}

	return bmpArr;
}