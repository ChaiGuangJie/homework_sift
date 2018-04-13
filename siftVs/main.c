#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

//卷积次数
#define CONVTIMES 3
//采样次数
#define SAMPTIMES 3
//
#define SAMPLESIZE 9

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
//#pragma pack(1)
typedef struct tagBITMAPFILEHEADER
{
	BYTE bfType[2];//文件格式
	DWORD bfSize;//文件大小
	WORD bfReserved1;//保留
	WORD bfReserved2;
	DWORD bfOffBits; //DIB数据在文件中的偏移量
}fileHeader;
//#pragma pack()

/*
位图数据信息结构
*/
//#pragma pack(1)
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
//#pragma pack()

/*
调色板结构
*/
//#pragma pack(1)
typedef struct tagRGBQUAD
{
	BYTE rgbBlue; //蓝色分量亮度
	BYTE rgbGreen;//绿色分量亮度
	BYTE rgbRed;//红色分量亮度
	BYTE rgbReserved;
}rgbq;
//#pragma pack()

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
	WORD bitCountOfPixel;
}bmpArray;

typedef enum color
{
	red = 0, green, blue, gray, all
}singleColor;

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

	printf("%d",sizeof(bmp->fInfo));

	fread(bmp->fInfo, sizeof(fileInfo), 1, file);

	if (bmp->fInfo->biBitCount != 24)
	{
		bmp->rgbqCount = (1 << bmp->fInfo->biBitCount);
		bmp->rgbqList = (rgbq*)malloc(sizeof(rgbq)*bmp->rgbqCount);
		fread(bmp->rgbqList, sizeof(rgbq), bmp->rgbqCount, file); //todo
	}
	else
	{
		bmp->rgbqCount = 0;
		bmp->rgbqList = NULL;
	}

	bmp->pixelData = (BYTE*)malloc(bmp->fInfo->biSizeImage);
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
	fwrite(bmp->fHead, sizeof(bmp->fHead), 1, file);
	fwrite(bmp->fInfo, sizeof(bmp->fInfo), 1, file);
	if (bmp->rgbqCount)
	{
		fwrite(bmp->rgbqList, sizeof(rgbq), bmp->rgbqCount, file);
	}
	fwrite(bmp->pixelData, bmp->fInfo->biSizeImage, 1, file);

	fclose(file);
	return 0;
}

bmpArray* bmpArrBuild(const bmpImg* bmp, singleColor color)
{
	BYTE* pixelDataPtr = bmp->pixelData;
	bmpArray* bmpArr = (bmpArray*)malloc(sizeof(bmpArray));
	bmpArr->pixelArray = (BYTE*)malloc(bmp->fInfo->biSizeImage);

	if (color == all || bmp->fInfo->biBitCount != 24) //非rgb 
	{
		strcpy_s(bmpArr->pixelArray, bmp->fInfo->biSizeImage, pixelDataPtr);
		bmpArr->width = bmp->fInfo->biWidth;
		bmpArr->height = bmp->fInfo->biHeight;
		bmpArr->bitCountOfPixel = bmp->fInfo->biBitCount;
	}
	else
	{
		BYTE rgb[4]; //分别存储r g b gray
		for (size_t i = 0; i < bmp->fInfo->biSizeImage; i++)
		{
			rgb[0] = *pixelDataPtr++;
			rgb[1] = *pixelDataPtr++;
			rgb[2] = *pixelDataPtr++;

			rgb[3] = (BYTE)(rgb[0] * 0.299 + rgb[1] * 0.587 + rgb[2] * 0.114); //三色转灰度 todo:用移位操作避免浮点运算 (或者直接1：1：1)

			*(bmpArr->pixelArray) = rgb[color];
			bmpArr->pixelArray++;
		}
		bmpArr->width = bmp->fInfo->biWidth;
		bmpArr->height = bmp->fInfo->biHeight;
		bmpArr->bitCountOfPixel = sizeof(BYTE);
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
		
		bmp->rgbqCount = (1 << bmpArr->bitCountOfPixel) ;
		bmp->rgbqList = (rgbq*)malloc(bmp->rgbqCount * sizeof(rgbq));

		switch (bmpArr->bitCountOfPixel)
		{
		case BIT_1:
			//todo
			break;
		case BIT_4:
			//todo
			break;
		case BIT_8:
			for (size_t i = 0; i < (1 << BIT_8); i++)
			{
				(bmp->rgbqList + i)->rgbBlue = 1;
				(bmp->rgbqList + i)->rgbGreen = 1;
				(bmp->rgbqList + i)->rgbRed = 1;
				(bmp->rgbqList + i)->rgbReserved = 0;
			}
			break;
		default:
			break;
		}
	}
	bmp->pixelData = bmpArr->pixelArray;

	*bmp->fHead->bfType = 'BM';
	bmp->fHead->bfReserved1 = 0;
	bmp->fHead->bfReserved2 = 0;
	bmp->fHead->bfOffBits = sizeof(fileHeader) + sizeof(fileInfo) + sizeof(rgbq) * bmp->rgbqCount;
	bmp->fHead->bfSize = bmpArr->width * bmpArr->height; //todo

	bmp->fInfo->biSize = 40;//todo 这里指定大小干嘛？
	bmp->fInfo->biWidth = bmpArr->width; //todo 保证是4的倍数?不是倍数怎么办？
	bmp->fInfo->biHeight = bmpArr->height;
	bmp->fInfo->biPlanes = 1;
	bmp->fInfo->biBitCount = bmpArr->bitCountOfPixel;
	bmp->fInfo->biCompression = 0;
	bmp->fInfo->biSizeImage = bmpArr->width * bmpArr->height;
	bmp->fInfo->biXPixPerMeter = 0;//todo
	bmp->fInfo->biYPixPerMeter = 0;
	bmp->fInfo->biClrUsed = bmp->rgbqCount;
	bmp->fInfo->biClrImporant = 0;

	return bmp;
}

bmpArray ConvBmp(const bmpArray* bmpArr,bmpArray kernal)
{

}

void downSample(const bmpArray* bmpArr,WORD sampleWidowSize)
{
	//释放bmpArr.pixelArray原有空间，重新申请空间并赋值

}

void fillKernal(bmpArray* kernal)
{

}


int main() 
{
	bmpImg *sourceImg, *tempImg;

	bmpArray *readyImgArr, *tempImgArr, kernal[CONVTIMES];

	bmpArray pyramidImgs[CONVTIMES][SAMPTIMES]; //保存卷积后的图像金字塔

	char* sourceFileName = "1.bmp";

	sourceImg = readBmp(sourceFileName);
	readyImgArr = bmpArrBuild(sourceImg, gray);
	tempImg = bmpImgBuild(readyImgArr);
	writeBmp(tempImg, "gray_1.bmp");
	
	//fillKernal(kernal);
	//tempImgArr = readyImgArr;
	//
	//for (size_t i = 0; i < SAMPTIMES; i++)
	//{
	//	for (size_t j = 0; j < CONVTIMES; j++)
	//	{
	//		pyramidImgs[i][j] = ConvBmp(tempImgArr, kernal[j]);
	//	}
	//	downSample(tempImgArr, SAMPLESIZE);
	//}

	system("pause");
	return 0;
}

//int main()
//{
//	/*存储RGB图像的一行像素点*/
//	BYTE ImgData[3000][3];
//	/*将灰度图的像素存到一个一维数组中*/
//	BYTE ImgData2[3000];
//	int i, j, k;
//	FILE * fpBMP, *fpGray;
//	fileHeader * fh;
//	fileInfo * fi;
//	rgbq * fq;
//
//	if ((fpBMP = fopen("G:/vc6.0/work/21.bmp", "rb")) == NULL)
//	{
//		printf("打开文件失败");
//		exit(0);
//	}
//
//	if ((fpGray = fopen("G:/vc6.0/work/22.bmp", "wb")) == NULL)
//	{
//		printf("创建文件失败");
//		exit(0);
//	}
//
//	fh = (fileHeader *)malloc(sizeof(fileHeader));
//	fi = (fileInfo *)malloc(sizeof(fileInfo));
//	//读取位图头结构和信息头
//	fread(fh, sizeof(fileHeader), 1, fpBMP);
//	fread(fi, sizeof(fileInfo), 1, fpBMP);
//	//修改头信息
//	fi->biBitCount = 8;
//	fi->biSizeImage = ((fi->biWidth * 3 + 3) / 4) * 4 * fi->biHeight;
//	//fi->biClrUsed=256;
//
//	fh->bfOffBits = sizeof(fileHeader) + sizeof(fileInfo) + 256 * sizeof(rgbq);
//	fh->bfSize = fh->bfOffBits + fi->biSizeImage;
//
//	//创建调色版
//	fq = (rgbq *)malloc(256 * sizeof(rgbq));
//	for (i = 0; i<256; i++)
//	{
//		fq[i].rgbBlue = fq[i].rgbGreen = fq[i].rgbRed = i;
//		//fq[i].rgbReserved=0;
//	}
//	//将头信息写入
//	fwrite(fh, sizeof(fileHeader), 1, fpGray);
//	fwrite(fi, sizeof(fileInfo), 1, fpGray);
//	fwrite(fq, sizeof(rgbq), 256, fpGray);
//	//读取RGB图像素并转换为灰度值
//	for (i = 0; i<fi->biHeight; i++)
//	{
//		for (j = 0; j<(fi->biWidth + 3) / 4 * 4; j++)
//		{
//			for (k = 0; k<3; k++)
//				fread(&ImgData[j][k], 1, 1, fpBMP);
//		}
//		for (j = 0; j<(fi->biWidth + 3) / 4 * 4; j++)
//		{
//			ImgData2[j] =(int)((float)ImgData[j][0] * 0.114 +(float)ImgData[j][1] * 0.587 + (float)ImgData[j][2] * 0.299);
//		}
//		//将灰度图信息写入
//		fwrite(ImgData2, j, 1, fpGray);
//	}
//
//	free(fh);
//	free(fi);
//	free(fq);
//	fclose(fpBMP);
//	fclose(fpGray);
//	printf("success\n");
//	return 0;
//}
