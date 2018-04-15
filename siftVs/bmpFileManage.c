#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"bmpFileManage.h"


bmpImg* readBmp(const char* fileName)
{
	FILE* file;
	bmpImg* bmp = NULL;
	if (0 != fopen_s(&file, fileName, "rb"))
	{
		return bmp;
	}
	bmp = (bmpImg*)malloc(sizeof(bmpImg));

	bmp->fHead = (fileHeader*)malloc(sizeof(fileHeader));

	fread(bmp->fHead, sizeof(fileHeader), 1, file);

	bmp->fInfo = (fileInfo*)malloc(sizeof(fileInfo));

	//printf("%d", sizeof(bmp->fInfo));

	fread(bmp->fInfo, sizeof(fileInfo), 1, file);

	if (bmp->fInfo->biBitCount != 24)
	{
		bmp->rgbqCount = (1 << bmp->fInfo->biBitCount);
		bmp->rgbqList = (rgbq*)malloc(sizeof(rgbq) * bmp->rgbqCount);
		fread(bmp->rgbqList, sizeof(rgbq), bmp->rgbqCount, file); //todo
	}
	else
	{
		bmp->rgbqCount = 0;
		bmp->rgbqList = NULL;
	}
	//bmp->fInfo->biSizeImage = bmp->fHead->bfSize - bmp->fHead->bfOffBits;

	bmp->fInfo->biSizeImage = bmp->fInfo->biWidth * bmp->fInfo->biHeight * bmp->fInfo->biBitCount / 8; //todo 为何读到的biSizeImage为0？
	// bmp->fInfo->biSizeImage 不是正真正DIB的大小！！！

	bmp->pixelData = (BYTE*)malloc(bmp->fHead->bfSize - bmp->fHead->bfOffBits);
	fread(bmp->pixelData, bmp->fInfo->biSizeImage, 1, file);

	fclose(file);
	return bmp;

}

int writeBmp(const bmpImg* bmp, const char* fileName)
{
	FILE* file;
	if (0 != fopen_s(&file, fileName, "wb"))
	{
		return 1;
	}
	fwrite(bmp->fHead, sizeof(fileHeader), 1, file);
	fwrite(bmp->fInfo, sizeof(fileInfo), 1, file);
	if (bmp->rgbqCount)
	{
		fwrite(bmp->rgbqList, sizeof(rgbq), bmp->rgbqCount, file);
	}

	fwrite(bmp->pixelData, bmp->fHead->bfSize - bmp->fHead->bfOffBits, 1, file);

	fclose(file);
	return 0;
}

bmpArray* bmpArrBuild(const bmpImg* bmp, singleColor color)
{
	
	bmpArray* bmpArr = (bmpArray*)malloc(sizeof(bmpArray));

	bmpArr->width = bmp->fInfo->biWidth;
	bmpArr->height = bmp->fInfo->biHeight;

	DWORD realArrWidthBytes = 0;

	if (color == all || bmp->fInfo->biBitCount != 24) //非rgb 
	{
		bmpArr->pixelArray = (BYTE*)malloc(bmp->fHead->bfSize - bmp->fHead->bfOffBits);

		memcpy(bmpArr->pixelArray, bmp->pixelData, bmp->fHead->bfSize - bmp->fHead->bfOffBits);
		
		bmpArr->bitCountOfPixel = bmp->fInfo->biBitCount;

		//todo realWidth
		realArrWidthBytes = bmpArr->width * bmpArr->bitCountOfPixel / 8;

		if (realArrWidthBytes % 4 != 0)
		{
			realArrWidthBytes += 4 - realArrWidthBytes % 4;
		}

		bmpArr->realWidth = realArrWidthBytes;
	}
	else
	{
		bmpArr->bitCountOfPixel = sizeof(BYTE) * 8;

		BYTE rgb[4]; //分别存储r g b gray
		
		//按bmp图像的原始数据存（即图像宽度字节数需对齐4字节）
		DWORD realBmpWidthBytes = bmp->fInfo->biWidth * bmp->fInfo->biBitCount / 8;
		if (realBmpWidthBytes % 4 != 0)
		{
			realBmpWidthBytes += 4 - realBmpWidthBytes % 4;
		}

		realArrWidthBytes = bmpArr->width * bmpArr->bitCountOfPixel / 8;
		if (realArrWidthBytes % 4 != 0)
		{
			realArrWidthBytes += 4 - realArrWidthBytes % 4;
		}

		bmpArr->realWidth = realArrWidthBytes;

		bmpArr->pixelArray = (BYTE*)malloc(realArrWidthBytes * bmpArr->height);
	
		BYTE* pixelDataPtr = NULL;

		//一定要用双层循环
		for (size_t i = 0; i < bmp->fInfo->biHeight; i++)
		{
			for (size_t j = 0; j < bmp->fInfo->biWidth; j++)
			{
				pixelDataPtr = bmp->pixelData + i * realBmpWidthBytes + j * bmp->fInfo->biBitCount / 8;

				rgb[0] = *(pixelDataPtr++);
				rgb[1] = *(pixelDataPtr++);
				rgb[2] = *(pixelDataPtr++);

				//rgb[3] = (BYTE)((rgb[0] + rgb[1] + rgb[2]) / 3);
				rgb[3] = (BYTE)(rgb[0] * 0.299 + rgb[1] * 0.587 + rgb[2] * 0.114); //三色转灰度 todo:用移位操作避免浮点运算 (或者直接1：1：1)
				*(bmpArr->pixelArray + i * realArrWidthBytes + j * bmpArr->bitCountOfPixel / 8) = rgb[color];
			}
		}
		
	}
	return bmpArr;
}

bmpImg* bmpImgBuild(const bmpArray* bmpArr)
{
	bmpImg* bmp = (bmpImg*)malloc(sizeof(bmpImg));
	bmp->fHead = (fileHeader*)malloc(sizeof(fileHeader));
	bmp->fInfo = (fileInfo*)malloc(sizeof(fileInfo));

	if (bmpArr->bitCountOfPixel == BIT_24)
	{
		bmp->rgbqCount = 0;
		bmp->rgbqList = NULL;
	}
	else
	{

		bmp->rgbqCount = (1 << bmpArr->bitCountOfPixel);
		bmp->rgbqList = (rgbq*)malloc(bmp->rgbqCount * sizeof(rgbq));
		int t,u = 0;

		switch (bmpArr->bitCountOfPixel)
		{
		case BIT_1:
			//todo
			break;
		case BIT_4:
			//todo
			break;
		case BIT_8:
			u = (1 << BIT_8);
			t = (int)pow(2,BIT_8);
			for (size_t i = 0; i < (size_t)(1 << BIT_8); i++)
			{
				(bmp->rgbqList + i)->rgbBlue = (BYTE)i;
				(bmp->rgbqList + i)->rgbGreen = (BYTE)i;
				(bmp->rgbqList + i)->rgbRed = (BYTE)i;
				(bmp->rgbqList + i)->rgbReserved = 0;
			}
			break;
		default:
			break;
		}
	}

	//DWORD realWidthBytes = bmpArr->width * bmpArr->bitCountOfPixel / 8;

	//if (realWidthBytes % 4 != 0)
	//{
	//	realWidthBytes += 4 - realWidthBytes % 4;
	//}

	bmp->pixelData = (BYTE*)malloc(bmpArr->realWidth * bmpArr->height); 
	//todo check
	memcpy(bmp->pixelData, bmpArr->pixelArray, bmpArr->realWidth * bmpArr->height);

	*bmp->fHead->bfType = 'B';
	*(bmp->fHead->bfType + 1) = 'M';
	bmp->fHead->bfReserved1 = 0;
	bmp->fHead->bfReserved2 = 0;
	bmp->fHead->bfOffBits = sizeof(fileHeader) + sizeof(fileInfo) + sizeof(rgbq) * bmp->rgbqCount;
	bmp->fHead->bfSize = bmp->fHead->bfOffBits + bmpArr->realWidth * bmpArr->height;

	bmp->fInfo->biSize = 40;//todo 这里指定大小干嘛？
	bmp->fInfo->biWidth = bmpArr->width;
	bmp->fInfo->biHeight = bmpArr->height;
	bmp->fInfo->biPlanes = 1;
	bmp->fInfo->biBitCount = bmpArr->bitCountOfPixel;
	bmp->fInfo->biCompression = 0;
	bmp->fInfo->biSizeImage = bmpArr->width * bmpArr->height *bmpArr->bitCountOfPixel / 8;
	bmp->fInfo->biXPixPerMeter = 0; //todo
	bmp->fInfo->biYPixPerMeter = 0;
	bmp->fInfo->biClrUsed = bmp->rgbqCount;
	bmp->fInfo->biClrImporant = bmp->rgbqCount;

	return bmp;
}