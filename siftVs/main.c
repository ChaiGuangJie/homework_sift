//#include<stdio.h>
//#include<malloc.h>
//#include<stdlib.h>
//#include<math.h>
//#include<string.h>
#include"bmpFileManage.h"

//�������
#define CONVTIMES 3
//��������
#define SAMPTIMES 3
//
#define SAMPLESIZE 9



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
