#include"featureDetect.h"
#include"bmpFileManage.h"



int main() 
{
	bmpImg *sourceImg, *tempImg;

	bmpArray *readyImgArr, *tempImgArr;
	kernal* kers[CONVTIMES];

	array2D* pyramidImgs[SAMPTIMES][CONVTIMES]; //保存卷积后的图像金字塔
	array2D* DOGPyramidImgs[SAMPTIMES][CONVTIMES - 1]; //卷积结果少一个

	featureList* featureList1=initFeatureList(), *featureList2 = initFeatureList();

	char* sourceFileName = "1.bmp";

	sourceImg = readBmp(sourceFileName);
	//writeBmp(sourceImg, "t_copy_2.bmp");
	//sourceImg = readBmp(sourceFileName);
	readyImgArr = bmpArrBuild(sourceImg, gray);
	tempImg = bmpImgBuild(readyImgArr);

	writeBmp(tempImg, "gray_1.bmp");
	//
	array2D* afterSamp2D = NULL;
	char fileName[30];
	for (size_t i = 0; i < SAMPTIMES; i++)
	{
		afterSamp2D = downSample(imgArrToArr2D(readyImgArr), i + 1); //todo free
		for (size_t j = 0; j < CONVTIMES; j++)
		{
			pyramidImgs[i][j] = conv2D(afterSamp2D, fillKernal(KERNALWIDTH, pow(sqrt(2),(j + 1))));//todo 释放kernal

			sprintf_s(fileName, 30, "pyramid_%d_%d.bmp", i, j);

			writeBmp(bmpImgBuild(arr2DToImgArr(pyramidImgs[i][j])), fileName);

			if (j > 0)
			{
				DOGPyramidImgs[i][j-1] = DOG(pyramidImgs[i][j-1], pyramidImgs[i][j]);

				sprintf_s(fileName, 30, "DOGPyramid_%d_%d.bmp", i, j-1);

				writeBmp(bmpImgBuild(arr2DToImgArr(DOGPyramidImgs[i][j-1])), fileName);
			}

		}

		free(afterSamp2D->head);
		free(afterSamp2D);
	}

	peakExtract(DOGPyramidImgs,SAMPTIMES,CONVTIMES,featureList1);

	//有问题
	calc_feature_oris(featureList1, DOGPyramidImgs);



	system("pause");
	return 0;
}