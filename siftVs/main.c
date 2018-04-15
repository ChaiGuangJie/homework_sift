#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include"bmpFileManage.h"

//卷积次数
#define CONVTIMES 3
//采样次数
#define SAMPTIMES 3
//
#define SAMPLESIZE 9

//卷积核宽度(奇数)
#define KERNALWIDTH 15

#define PI          3.14159

#define PADDING     1


typedef struct kernal
{
	float* kernalArr;
	DWORD width;
	DWORD height;
}kernal;

//扩充单色图像边缘
bmpArray* preConv(const bmpArray* oldBmpArr, DWORD kWidth)
{
	DWORD width = oldBmpArr->width + kWidth - 1;
	DWORD height = oldBmpArr->height + kWidth - 1;

	bmpArray* newBmpArr = (bmpArray*)malloc(sizeof(bmpArray));
	newBmpArr->width = width;
	newBmpArr->height = height;
	newBmpArr->bitCountOfPixel = oldBmpArr->bitCountOfPixel;

	newBmpArr->realWidth = width * newBmpArr->bitCountOfPixel / 8;

	if (newBmpArr->realWidth % 4 != 0)
	{
		newBmpArr->realWidth += 4 - width % 4;
	}

	newBmpArr->pixelArray = (BYTE*)malloc(newBmpArr->realWidth * height);
	memset(newBmpArr->pixelArray, PADDING, newBmpArr->realWidth * height);

	BYTE* newArrPtr = newBmpArr->pixelArray;
	BYTE* oldArrPtr = oldBmpArr->pixelArray;

	//for (size_t i = 0; i < newBmpArr->realWidth * height; i++)
	//{
	//	*(newBmpArr->pixelArray + i) = (BYTE)PADDING;
	//}

	for (size_t i = 0; i < oldBmpArr->height; i++)
	{
		for (size_t j = 0; j < oldBmpArr->width; j++)
		{
			*(newArrPtr + (kWidth / 2 + i) * newBmpArr->realWidth + (kWidth / 2) + j) = *(oldArrPtr + i * oldBmpArr->realWidth + j);
		}
	}

	/*for (size_t i = kWidth / 2; i < kWidth / 2 + oldBmpArr->height; i++)
	{
		for (size_t j = kWidth / 2; j < kWidth / 2 + oldBmpArr->width; j++)
		{
			*(newBmpArr->pixelArray + i * newBmpArr->realWidth + j) = *(oldArrPtr++);
		}
	}*/
	return newBmpArr;
}

DWORD localCov(const bmpArray* bmp, const BYTE* locOrigin, const kernal* ker)
{
	BYTE* bmpPtr = locOrigin; //todo 
	float* kerPtr = ker->kernalArr;
	float covResult = 0;

	for (size_t i = 1; i < ker->height * ker->width + 1; i++)
	{
		float t = *kerPtr;
		covResult = covResult + (*kerPtr) * (*bmpPtr);
		kerPtr++;
		bmpPtr++;
		if ((ker->height*ker->width != i) && (i % ker->width == 0))
		{
			//方形区域内一行卷积完毕
			bmpPtr = bmpPtr + bmp->width - ker->width+1;
			//bmpPtr += (bmp->width - ker->width);
		}
		//todo 判断是否边界
	}
	//todo 大坑！！！
	if (covResult > 255)
	{
		covResult = 255;
	}
	return (BYTE)covResult;
}

bmpArray* convBmp(const bmpArray* bmpArr,const kernal* ker)
{
	bmpArray* newBmpArr = (bmpArray*)malloc(sizeof(bmpArray));


	newBmpArr->width = bmpArr->width;
	newBmpArr->height = bmpArr->height;
	newBmpArr->bitCountOfPixel = bmpArr->bitCountOfPixel;

	newBmpArr->realWidth = newBmpArr->width * newBmpArr->bitCountOfPixel / 8; //todo realWidth重构
	if (newBmpArr->realWidth % 4 != 0)
	{
		newBmpArr->realWidth += 4 - newBmpArr->realWidth % 4;
	}

	newBmpArr->pixelArray = (BYTE*)malloc(bmpArr->realWidth * newBmpArr->height);
	
	//todo 先构造新图像
	bmpArray* preBmpArr = preConv(bmpArr, ker->width);

	writeBmp(bmpImgBuild(preBmpArr),"testPre.bmp");

	for (size_t i = 0; i < newBmpArr->height; i++)
	{
		for (size_t j = 0; j < newBmpArr->width; j++)
		{
			*(newBmpArr->pixelArray + i * newBmpArr->realWidth + j * newBmpArr->bitCountOfPixel / 8) =
				localCov(preBmpArr, preBmpArr->pixelArray + i * preBmpArr->realWidth + j * preBmpArr->bitCountOfPixel / 8, ker);
		}
	}

	/*for (size_t j = 0; j < newBmpArr->height * newBmpArr->width; j++)
	{
		*(newBmpArr->pixelArray + j) = localCov(preBmpArr, preBmpArr->pixelArray + j, ker);
	}*/
	return newBmpArr;
}



//求卷积后的局域极值点


//用三次函数拟合后求真正的极值点


//去除边界上的和小于阈值的极值点

//todo 下采样后求得的极值点如何恢复到原图像中？

void downSample(const bmpArray* bmpArr,WORD sampleWidowSize)
{
	//释放bmpArr.pixelArray原有空间，重新申请空间并赋值

}

//目前仅限高斯核 宽限定为奇数？
kernal* fillKernal(DWORD kWidth,float dt)
{
	if (kWidth % 2 != 1)
	{
		return NULL;
	}
	kernal* ker = (kernal*)malloc(sizeof(kernal));
	ker->width = kWidth;
	ker->height = kWidth;
	ker->kernalArr = (float*)malloc(sizeof(float) * ker->width * ker->height);
	float* kerPtr = ker->kernalArr;

	int base = -(int)(kWidth / 2);
	int x, y;
	float nomal = 0;

	for (size_t i = 0; i < ker->height; i++)
	{
		for (size_t j = 0; j < ker->width; j++)
		{
			y = base + i;
			x = base + j;
			*kerPtr = (1.0 / (2 * PI * dt * dt)) * exp((x * x + y * y) / (-2.0*dt*dt));
			nomal += *(kerPtr);
			kerPtr++;
			//printf("%f", *((ker->kernalArr) + i + j));
		}
	}
	//todo 权重归一化
	for (size_t i = 0; i < ker->height*ker->width; i++)
	{
		*(ker->kernalArr + i) = *(ker->kernalArr + i) / nomal;
	}
	return ker;
}


int main() 
{
	bmpImg *sourceImg, *tempImg;

	bmpArray *readyImgArr, *tempImgArr;
	kernal* kers[CONVTIMES];

	bmpArray* pyramidImgs[CONVTIMES][SAMPTIMES]; //保存卷积后的图像金字塔

	char* sourceFileName = "1.bmp";

	sourceImg = readBmp(sourceFileName);
	//writeBmp(sourceImg, "t_copy_2.bmp");
	//sourceImg = readBmp(sourceFileName);
	readyImgArr = bmpArrBuild(sourceImg, green);
	tempImg = bmpImgBuild(readyImgArr);

	writeBmp(tempImg, "gray_1.bmp");
	//
	kers[0] = fillKernal(KERNALWIDTH, 3);
	tempImgArr = readyImgArr;

	/*char pixelArr[10] = {'q','q','q','q','q','q','q','q','q','q'};
	pixelArr[5] = 127;
	bmpArray testBmpArr = { pixelArr,3,3,8 };
	tempImgArr = &testBmpArr;*/

	pyramidImgs[0][0] = convBmp(tempImgArr, kers[0]);
	
	writeBmp(bmpImgBuild(pyramidImgs[0][0]),"conv_test.bmp");
	
	
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
