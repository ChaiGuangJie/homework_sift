#pragma once
#include<stdio.h>
#include<malloc.h>
#include<stdlib.h>
#include<math.h>
#include"bmpFileManage.h"


//卷积次数
#define CONVTIMES 4
//采样次数
#define SAMPTIMES 3
//
#define SAMPLESIZE 7

//卷积核宽度(奇数)
#define KERNALWIDTH 15

#define PI          3.14159

#define PADDING     1

#define interp_hist_peak( l, c, r )		(0.5 * ((l)-(r)) / ((l) - 2.0*(c) + (r)))

#define SIFT_ORI_SIG_FCTR				1.5			/* determines gaussian sigma for orientation assignment */
#define SIFT_ORI_HIST_BINS				36			/* default number of bins in histogram for orientation assignment */
#define SIFT_ORI_RADIUS					3.0 * SIFT_ORI_SIG_FCTR	/* determines the radius of the region used in orientation assignment */
#define SIFT_ORI_SMOOTH_PASSES			2			/* number of passes of orientation histogram smoothing */
#define SIFT_ORI_PEAK_RATIO				0.8			/* orientation magnitude relative to max that results in new feature */
#define SIFT_CONTR_THR					0.04		/** default threshold on keypoint contrast |D(x)| */



typedef struct kernal
{
	float* kernalArr;
	DWORD width;
	DWORD height;
}kernal;


typedef struct peakPoint
{
	DWORD h;
	DWORD w;
	//DWORD layer;
	DWORD iSamp;
	DWORD iConv;
	double ori;
	struct peakPoint* next;

}peakPoint;

typedef struct featureList
{
	peakPoint* head;
	peakPoint* current;
	DWORD count;

}featureList;

BYTE localConv(const array2D* arr2D, size_t begin, kernal* k);

kernal* fillAvgKernal(WORD width);

array2D* downSample(const array2D* arr2D, WORD sampleWidowSize);

kernal* fillKernal(DWORD kWidth, float dt);

array2D* prepareConv(array2D* originBmp2D, DWORD kWidth);

featureList* initFeatureList();

void storagePeak(featureList* fList, DWORD h, DWORD w, DWORD iSamp, DWORD iConv);

array2D* conv2D(const array2D* bmp2D, const kernal* ker);

array2D* DOG(const array2D* oldArr2D, const array2D* newArr2D);

BYTE localPeak2D(const array2D arr2D, DWORD hIndex, DWORD wIndex);

BYTE localPeak3D(const array2D* frontLayer, const array2D* currentLayer, const array2D* backLayer, size_t hIndex, size_t wIndex);

void peakExtract(const array2D** DOGPyramid, DWORD maxSampTimes, DWORD maxConvTimes, featureList* flist);

float pixval32f(array2D* img, int r, int c);

int calc_grad_mag_ori(array2D* img, int r, int c, double* mag, double* ori);

int iround(double value);

double* ori_hist(array2D* img, int r, int c, int n, int rad, double sigma);

void smooth_ori_hist(double* hist, int n);

static double dominant_ori(double* hist, int n);

peakPoint* clone_feature(const peakPoint* feat);

void storageFeature(featureList* fList, peakPoint* pk);

void add_good_ori_features(featureList* features, double* hist, int n, double mag_thr, peakPoint* feat);

peakPoint* nextFeature(featureList* fList);

void calc_feature_oris(featureList* features, featureList* newFeatures, array2D** gauss_pyr);

int is_too_edge_like(array2D* dog_img, int r, int c, int curv_thr);