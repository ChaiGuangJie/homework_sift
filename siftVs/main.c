#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>

//�������
#define CONVTIMES 3
//��������
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
λͼͷ�ṹ
*/
//#pragma pack(1)
typedef struct tagBITMAPFILEHEADER
{
	BYTE bfType[2];//�ļ���ʽ
	DWORD bfSize;//�ļ���С
	WORD bfReserved1;//����
	WORD bfReserved2;
	DWORD bfOffBits; //DIB�������ļ��е�ƫ����
}fileHeader;
//#pragma pack()

/*
λͼ������Ϣ�ṹ
*/
//#pragma pack(1)
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
//#pragma pack()

/*
��ɫ��ṹ
*/
//#pragma pack(1)
typedef struct tagRGBQUAD
{
	BYTE rgbBlue; //��ɫ��������
	BYTE rgbGreen;//��ɫ��������
	BYTE rgbRed;//��ɫ��������
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
}bmpImg;//todo �ǵ��ͷ�


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

	if (color == all || bmp->fInfo->biBitCount != 24) //��rgb 
	{
		strcpy_s(bmpArr->pixelArray, bmp->fInfo->biSizeImage, pixelDataPtr);
		bmpArr->width = bmp->fInfo->biWidth;
		bmpArr->height = bmp->fInfo->biHeight;
		bmpArr->bitCountOfPixel = bmp->fInfo->biBitCount;
	}
	else
	{
		BYTE rgb[4]; //�ֱ�洢r g b gray
		for (size_t i = 0; i < bmp->fInfo->biSizeImage; i++)
		{
			rgb[0] = *pixelDataPtr++;
			rgb[1] = *pixelDataPtr++;
			rgb[2] = *pixelDataPtr++;

			rgb[3] = (BYTE)(rgb[0] * 0.299 + rgb[1] * 0.587 + rgb[2] * 0.114); //��ɫת�Ҷ� todo:����λ�������⸡������ (����ֱ��1��1��1)

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

	bmp->fInfo->biSize = 40;//todo ����ָ����С���
	bmp->fInfo->biWidth = bmpArr->width; //todo ��֤��4�ı���?���Ǳ�����ô�죿
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
	//�ͷ�bmpArr.pixelArrayԭ�пռ䣬��������ռ䲢��ֵ

}

void fillKernal(bmpArray* kernal)
{

}


int main() 
{
	bmpImg *sourceImg, *tempImg;

	bmpArray *readyImgArr, *tempImgArr, kernal[CONVTIMES];

	bmpArray pyramidImgs[CONVTIMES][SAMPTIMES]; //���������ͼ�������

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
//	/*�洢RGBͼ���һ�����ص�*/
//	BYTE ImgData[3000][3];
//	/*���Ҷ�ͼ�����ش浽һ��һά������*/
//	BYTE ImgData2[3000];
//	int i, j, k;
//	FILE * fpBMP, *fpGray;
//	fileHeader * fh;
//	fileInfo * fi;
//	rgbq * fq;
//
//	if ((fpBMP = fopen("G:/vc6.0/work/21.bmp", "rb")) == NULL)
//	{
//		printf("���ļ�ʧ��");
//		exit(0);
//	}
//
//	if ((fpGray = fopen("G:/vc6.0/work/22.bmp", "wb")) == NULL)
//	{
//		printf("�����ļ�ʧ��");
//		exit(0);
//	}
//
//	fh = (fileHeader *)malloc(sizeof(fileHeader));
//	fi = (fileInfo *)malloc(sizeof(fileInfo));
//	//��ȡλͼͷ�ṹ����Ϣͷ
//	fread(fh, sizeof(fileHeader), 1, fpBMP);
//	fread(fi, sizeof(fileInfo), 1, fpBMP);
//	//�޸�ͷ��Ϣ
//	fi->biBitCount = 8;
//	fi->biSizeImage = ((fi->biWidth * 3 + 3) / 4) * 4 * fi->biHeight;
//	//fi->biClrUsed=256;
//
//	fh->bfOffBits = sizeof(fileHeader) + sizeof(fileInfo) + 256 * sizeof(rgbq);
//	fh->bfSize = fh->bfOffBits + fi->biSizeImage;
//
//	//������ɫ��
//	fq = (rgbq *)malloc(256 * sizeof(rgbq));
//	for (i = 0; i<256; i++)
//	{
//		fq[i].rgbBlue = fq[i].rgbGreen = fq[i].rgbRed = i;
//		//fq[i].rgbReserved=0;
//	}
//	//��ͷ��Ϣд��
//	fwrite(fh, sizeof(fileHeader), 1, fpGray);
//	fwrite(fi, sizeof(fileInfo), 1, fpGray);
//	fwrite(fq, sizeof(rgbq), 256, fpGray);
//	//��ȡRGBͼ���ز�ת��Ϊ�Ҷ�ֵ
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
//		//���Ҷ�ͼ��Ϣд��
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
