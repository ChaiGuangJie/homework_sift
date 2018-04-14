//#include<stdio.h>
//#include<malloc.h>
//#include<stdlib.h>
//#include<math.h>
//#include<string.h>
#include"bmpFileManage.h"

//卷积次数
#define CONVTIMES 3
//采样次数
#define SAMPTIMES 3
//
#define SAMPLESIZE 9



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
	writeBmp(tempImg, "gray_1_bili.bmp");
	
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
