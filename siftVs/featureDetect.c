#include"featureDetect.h"


//扩充单色图像边缘
//bmpArray* preConv(const bmpArray* oldBmpArr, DWORD kWidth)
//{
//	DWORD width = oldBmpArr->width + kWidth - 1;
//	DWORD height = oldBmpArr->height + kWidth - 1;
//
//	bmpArray* newBmpArr = (bmpArray*)malloc(sizeof(bmpArray));
//	newBmpArr->width = width;
//	newBmpArr->height = height;
//	newBmpArr->bitCountOfPixel = oldBmpArr->bitCountOfPixel;
//
//	newBmpArr->realWidth = width * newBmpArr->bitCountOfPixel / 8;
//
//	if (newBmpArr->realWidth % 4 != 0)
//	{
//		newBmpArr->realWidth += 4 - width % 4;
//	}
//
//	newBmpArr->pixelArray = (BYTE*)malloc(newBmpArr->realWidth * height);
//	memset(newBmpArr->pixelArray, PADDING, newBmpArr->realWidth * height);
//
//	BYTE* newArrPtr = newBmpArr->pixelArray;
//	BYTE* oldArrPtr = oldBmpArr->pixelArray;
//
//	//for (size_t i = 0; i < newBmpArr->realWidth * height; i++)
//	//{
//	//	*(newBmpArr->pixelArray + i) = (BYTE)PADDING;
//	//}
//
//	for (size_t i = 0; i < oldBmpArr->height; i++)
//	{
//		for (size_t j = 0; j < oldBmpArr->width; j++)
//		{
//			*(newArrPtr + (kWidth / 2 + i) * newBmpArr->realWidth + (kWidth / 2) + j) = *(oldArrPtr + i * oldBmpArr->realWidth + j);
//		}
//	}
//
//	/*for (size_t i = kWidth / 2; i < kWidth / 2 + oldBmpArr->height; i++)
//	{
//	for (size_t j = kWidth / 2; j < kWidth / 2 + oldBmpArr->width; j++)
//	{
//	*(newBmpArr->pixelArray + i * newBmpArr->realWidth + j) = *(oldArrPtr++);
//	}
//	}*/
//	return newBmpArr;
//}

//DWORD localCov(const bmpArray* bmp, const BYTE* locOrigin, const kernal* ker)
//{
//	BYTE* bmpPtr = locOrigin; //todo 
//	float* kerPtr = ker->kernalArr;
//	float covResult = 0;
//
//	for (size_t i = 1; i < ker->height * ker->width + 1; i++)
//	{
//		float t = *kerPtr;
//		covResult = covResult + (*kerPtr) * (*bmpPtr);
//		kerPtr++;
//		bmpPtr++;
//		if ((ker->height*ker->width != i) && (i % ker->width == 0))
//		{
//			//方形区域内一行卷积完毕
//			bmpPtr = bmpPtr + bmp->width - ker->width + 1;
//			//bmpPtr += (bmp->width - ker->width);
//		}
//		//todo 判断是否边界
//	}
//	//todo 大坑！！！
//	if (covResult > 255)
//	{
//		covResult = 255;
//	}
//	return (BYTE)covResult;
//}

//bmpArray* convBmp(const bmpArray* bmpArr, const kernal* ker)
//{
//	bmpArray* newBmpArr = (bmpArray*)malloc(sizeof(bmpArray));
//
//
//	newBmpArr->width = bmpArr->width;
//	newBmpArr->height = bmpArr->height;
//	newBmpArr->bitCountOfPixel = bmpArr->bitCountOfPixel;
//
//	newBmpArr->realWidth = newBmpArr->width * newBmpArr->bitCountOfPixel / 8; //todo realWidth重构
//	if (newBmpArr->realWidth % 4 != 0)
//	{
//		newBmpArr->realWidth += 4 - newBmpArr->realWidth % 4;
//	}
//
//	newBmpArr->pixelArray = (BYTE*)malloc(bmpArr->realWidth * newBmpArr->height);
//
//	//todo 先构造新图像
//	bmpArray* preBmpArr = preConv(bmpArr, ker->width);
//
//	writeBmp(bmpImgBuild(preBmpArr), "testPre.bmp");
//
//	for (size_t i = 0; i < newBmpArr->height; i++)
//	{
//		for (size_t j = 0; j < newBmpArr->width; j++)
//		{
//			*(newBmpArr->pixelArray + i * newBmpArr->realWidth + j * newBmpArr->bitCountOfPixel / 8) =
//				localCov(preBmpArr, preBmpArr->pixelArray + i * preBmpArr->realWidth + j * preBmpArr->bitCountOfPixel / 8, ker);
//		}
//	}
//
//	/*for (size_t j = 0; j < newBmpArr->height * newBmpArr->width; j++)
//	{
//	*(newBmpArr->pixelArray + j) = localCov(preBmpArr, preBmpArr->pixelArray + j, ker);
//	}*/
//	return newBmpArr;
//}



//求卷积后的局域极值点


//用三次函数拟合后求真正的极值点


//去除边界上的和小于阈值的极值点
//todo 下采样后求得的极值点如何恢复到原图像中？

peakPoint* currentPtr = NULL;


BYTE localConv(const array2D* arr2D, size_t begin, kernal* k)
{
	BYTE* arr2DPtr = arr2D->head + begin; //todo 
	float* kerPtr = k->kernalArr;
	float covResult = 0;

	for (size_t i = 1; i < k->height * k->width + 1; i++)
	{
		covResult = covResult + (*kerPtr) * (*arr2DPtr);
		kerPtr++;
		arr2DPtr++;
		if ((k->height*k->width != i) && (i % k->width == 0))
		{
			//方形区域内一行卷积完毕
			arr2DPtr = arr2DPtr + arr2D->w - k->width + 1;
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


kernal* fillAvgKernal(WORD width)
{
	kernal* k = (kernal*)malloc(sizeof(kernal));
	k->width = width;
	k->height = width;
	k->kernalArr = (float*)malloc(sizeof(float) * k->height * k->width);
	float* kerPtr = k->kernalArr;
	for (size_t i = 0; i < k->height*k->width; i++)
	{
		float f = 1.0 / (k->height*k->width);
		*kerPtr++ = f;
	}
	return k;
}

array2D* downSample(const array2D* arr2D, WORD sampleWidowSize)
{
	//
	array2D* newArr2D = (array2D*)malloc(sizeof(array2D));
	newArr2D->h = arr2D->h / sampleWidowSize;
	newArr2D->w = arr2D->w / sampleWidowSize;
	newArr2D->head = (BYTE*)malloc(newArr2D->h*newArr2D->w);

	kernal* k = fillAvgKernal(sampleWidowSize);

	for (size_t i = 0; i < newArr2D->h; i++)
	{
		for (size_t j = 0; j < newArr2D->w; j++)
		{
			//测试
			*(newArr2D->head + i * newArr2D->w + j) =
				localConv(arr2D, i* arr2D->w*sampleWidowSize + j * sampleWidowSize, k);
		}
	}

	return newArr2D;
}

//目前仅限高斯核 宽限定为奇数？
kernal* fillKernal(DWORD kWidth, float dt)
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
			//todo 这里能++？
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

array2D* prepareConv(array2D* originBmp2D, DWORD kWidth)
{
	array2D* prepBmp2D = (array2D*)malloc(sizeof(array2D));
	prepBmp2D->h = originBmp2D->h + kWidth - 1;
	prepBmp2D->w = originBmp2D->w + kWidth - 1;
	prepBmp2D->head = (BYTE*)malloc(prepBmp2D->h * prepBmp2D->w);

	memset(prepBmp2D->head, PADDING, prepBmp2D->h*prepBmp2D->w);

	for (size_t i = 0; i < originBmp2D->h; i++)
	{
		for (size_t j = 0; j < originBmp2D->w; j++)
		{
			*(prepBmp2D->head + (kWidth / 2 + i) * prepBmp2D->w + (kWidth / 2) + j) =
				*(originBmp2D->head + i * originBmp2D->w + j);
		}
	}
	return prepBmp2D;
}

featureList* initFeatureList()
{
	featureList* fList = (featureList*)malloc(sizeof(featureList));
	fList->head = NULL;
	fList->head = NULL;
	fList->count = 0;
	return fList;
}

void storagePeak(featureList* fList, DWORD h, DWORD w, DWORD iSamp, DWORD iConv)
{

	peakPoint* p = (peakPoint*)malloc(sizeof(peakPoint));
	p->next = NULL;
	p->h = h;
	p->w = w;
	p->iSamp = iSamp;
	p->iConv = iConv;

	if (fList->head == NULL)
	{
		fList->head = p;
		fList->current = p;
		fList->count = 1;
	}
	else
	{
		fList->current->next = p;
		fList->current = p;
		fList->count++;
	}
	return 0;
}

array2D* conv2D(const array2D* bmp2D, const kernal* ker)
{
	array2D* outBmp2D = (array2D*)malloc(sizeof(array2D));
	outBmp2D->h = bmp2D->h;
	outBmp2D->w = bmp2D->w;
	outBmp2D->head = (BYTE*)malloc(outBmp2D->w * outBmp2D->h);

	//todo 先构造新图像
	array2D* prepBmp2D = prepareConv(bmp2D, ker->width);
	//测试
	writeBmp(bmpImgBuild(arr2DToImgArr(prepBmp2D)), "testPre.bmp");

	for (size_t i = 0; i < outBmp2D->h; i++)
	{
		for (size_t j = 0; j < outBmp2D->w; j++)
		{
			*(outBmp2D->head + i * outBmp2D->w + j) =
				localConv(prepBmp2D, i * prepBmp2D->w + j, ker);
		}
	}
	return outBmp2D;
}

array2D* DOG(const array2D* oldArr2D, const array2D* newArr2D)
{
	array2D* dog2D = (array2D*)malloc(sizeof(array2D));
	dog2D->w = oldArr2D->w;
	dog2D->h = oldArr2D->h;
	dog2D->head = (BYTE*)malloc(oldArr2D->w * oldArr2D->h);
	int dog = 0;
	for (size_t i = 0; i < oldArr2D->h; i++)
	{
		for (size_t j = 0; j < oldArr2D->w; j++)
		{
			dog = *(oldArr2D->head + i * oldArr2D->w + j) - *(newArr2D->head + i * newArr2D->w + j);
			if (dog < 0)dog = 0;
			*(dog2D->head + i * dog2D->w + j) = (BYTE)dog;//todo
		}
	}
	return dog2D;
}



//找3*3区域的最大值
BYTE localPeak2D(const array2D arr2D, DWORD hIndex, DWORD wIndex)
{
	//todo 限制输入
	if (hIndex < 1 || (hIndex > arr2D.h - 1) || wIndex < 1 || (wIndex > arr2D.w - 1))
		return 0;

	BYTE* currentPtr = arr2D.head + (hIndex - 1) * wIndex + wIndex - 1;
	BYTE max = 0;
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 3; j++)
		{
			if (*(currentPtr + i * arr2D.w + j) > max)
			{
				max = *(currentPtr + i * arr2D.w + j);
			}
		}
	}
	return max;
}
//找3*3*3区域的极值
BYTE localPeak3D(const array2D* frontLayer, const array2D* currentLayer, const array2D* backLayer, size_t hIndex, size_t wIndex)
{
	BYTE current = *(currentLayer->head + hIndex * currentLayer->w + wIndex);


	if (current >= localPeak2D(*frontLayer, hIndex, wIndex)
		&& current >= localPeak2D(*currentLayer, hIndex, wIndex)
		&& current >= localPeak2D(*backLayer, hIndex, wIndex))
	{
		return 1;
	}
	else
	{
		return 0;
	}
	//1 是极值
	//0 非极值
}

int is_too_edge_like(array2D* dog_img, int r, int c, float curv_thr)
{
	double d, dxx, dyy, dxy, tr, det;

	/* principal curvatures are computed using the trace and det of Hessian */
	d = pixval32f(dog_img, r, c);
	dxx = pixval32f(dog_img, r, c + 1) + pixval32f(dog_img, r, c - 1) - 2 * d;
	dyy = pixval32f(dog_img, r + 1, c) + pixval32f(dog_img, r - 1, c) - 2 * d;
	dxy = (pixval32f(dog_img, r + 1, c + 1) - pixval32f(dog_img, r + 1, c - 1) -
		pixval32f(dog_img, r - 1, c + 1) + pixval32f(dog_img, r - 1, c - 1)) / 4.0;
	tr = dxx + dyy;
	det = dxx * dyy - dxy * dxy;

	/* negative determinant -> curvatures have different signs; reject feature */
	if (det <= 0)
		return 1;

	if (tr * tr / det < (curv_thr + 1.0)*(curv_thr + 1.0) / curv_thr)
		return 0;
	return 1;
}

void peakExtract(const array2D** DOGPyramid, DWORD maxSampTimes, DWORD maxConvTimes, featureList* flist)
{
	for (size_t j = 0; j < maxSampTimes; j++)
	{
		array2D** currentSample = DOGPyramid + j * maxSampTimes;

		for (size_t i = 1; i < maxConvTimes - 2; i++) //dog已-1所以此处 -2
		{
			array2D* lastConv = *(currentSample + i - 1);
			array2D* currentConv = *(currentSample + i);
			array2D* nextConv = *(currentSample + i + 1);

			for (size_t h = 1; h < currentConv->h - 1; h++)
			{
				for (size_t w = 1; w < currentConv->w - 1; w++)
				{
					if (localPeak3D(lastConv, currentConv, nextConv, h, w))
					{
						//todo 极值 存储
						if (!is_too_edge_like(currentConv, h, w, SIFT_CONTR_THR))
							storagePeak(flist, h, w, j, i);
					}
				}
			}
		}
	}

}
float pixval32f(array2D* img, int r, int c)
{
	return (float)(*(img->head + img->w * r + c));
}

int calc_grad_mag_ori(array2D* img, int r, int c, double* mag,
	double* ori)
{
	double dx, dy;

	if (r > 0 && r < img->h - 1 && c > 0 && c < img->w - 1)
	{
		dx = pixval32f(img, r, c + 1) - pixval32f(img, r, c - 1);
		dy = pixval32f(img, r - 1, c) - pixval32f(img, r + 1, c);
		*mag = sqrt(dx*dx + dy * dy);
		*ori = atan2(dy, dx);
		return 1;
	}

	else
		return 0;
}

int iround(double value)
{
	return (int)(value + (value >= 0 ? 0.5 : -0.5));
}

double* ori_hist(array2D* img, int r, int c, int n, int rad,
	double sigma)
{
	double* hist;
	double mag, ori, w, exp_denom, PI2 = PI * 2.0;
	int bin, i, j;

	hist = calloc(n, sizeof(double));
	exp_denom = 2.0 * sigma * sigma;
	for (i = -rad; i <= rad; i++)
		for (j = -rad; j <= rad; j++)
			if (calc_grad_mag_ori(img, r + i, c + j, &mag, &ori))
			{
				w = exp(-(i*i + j * j) / exp_denom);
				bin = iround(n * (ori + PI) / PI2);
				bin = (bin < n) ? bin : 0;
				hist[bin] += w * mag;
			}

	return hist;
}

void smooth_ori_hist(double* hist, int n)
{
	double prev, tmp, h0 = hist[0];
	int i;

	prev = hist[n - 1];
	for (i = 0; i < n; i++)
	{
		tmp = hist[i];
		hist[i] = 0.25 * prev + 0.5 * hist[i] +
			0.25 * ((i + 1 == n) ? h0 : hist[i + 1]);
		prev = tmp;
	}
}
static double dominant_ori(double* hist, int n)
{
	double omax;

	omax = hist[0];
	for (int i = 1; i < n; i++)
		if (hist[i] > omax)
		{
			omax = hist[i];
		}
	return omax;
}

peakPoint* clone_feature(const peakPoint* feat)
{
	peakPoint* new_feat = (peakPoint*)malloc(sizeof(peakPoint));

	memcpy(new_feat, feat, sizeof(peakPoint));

	new_feat->next = NULL;

	return new_feat;
}

void storageFeature(featureList* fList, peakPoint* pk)
{
	peakPoint* p = (peakPoint*)malloc(sizeof(peakPoint));
	p->next = NULL;
	p->h = pk->h;
	p->w = pk->w;
	p->iSamp = pk->iSamp;
	p->iConv = pk->iConv;
	p->ori = pk->ori;

	if (fList->head == NULL)
	{
		fList->head = p;
		fList->current = p;
		fList->count = 1;
	}
	else
	{
		fList->current->next = p;
		fList->current = p;
		fList->count++;
	}
	return 0;
}

void add_good_ori_features(featureList* features, double* hist, int n,
	double mag_thr, peakPoint* feat)
{
	peakPoint* new_feat;
	double bin, PI2 = PI * 2.0;
	int l, r, i;

	for (i = 0; i < n; i++)
	{
		l = (i == 0) ? n - 1 : i - 1;
		r = (i + 1) % n;

		if (hist[i] > hist[l] && hist[i] > hist[r] && hist[i] >= mag_thr)
		{
			bin = i + interp_hist_peak(hist[l], hist[i], hist[r]);
			bin = (bin < 0) ? n + bin : (bin >= n) ? bin - n : bin;
			new_feat = clone_feature(feat);
			new_feat->ori = ((PI2 * bin) / n) - PI;
			//seqPush(features, new_feat);
			storageFeature(features, new_feat);
			free(new_feat);
		}
	}
}


peakPoint* nextFeature(featureList* fList)
{
	if (currentPtr == NULL) {
		currentPtr = fList->head;
	}
	peakPoint* pk = currentPtr;//clone_feature(currentPtr);
	currentPtr = currentPtr->next;

	return pk;
}

void calc_feature_oris(featureList* features,featureList* newFeatures, array2D** gauss_pyr)
{
	peakPoint* feat;
	//DetectionData* ddata;
	double* hist;
	double omax;
	int i, j, n = features->count;

	for (i = 0; i < n; i++)
	{

		feat = nextFeature(features);
		double sigma = pow(sqrt(2), feat->iConv);
		hist = ori_hist(*(gauss_pyr + feat->iSamp * SAMPTIMES + feat->iConv),
			feat->h, feat->w, SIFT_ORI_HIST_BINS,
			iround(SIFT_ORI_RADIUS * (feat->iSamp+1)),
			sigma);
		for (j = 0; j < SIFT_ORI_SMOOTH_PASSES; j++)
			smooth_ori_hist(hist, SIFT_ORI_HIST_BINS);
		omax = dominant_ori(hist, SIFT_ORI_HIST_BINS);
		add_good_ori_features(newFeatures, hist, SIFT_ORI_HIST_BINS,
			omax * SIFT_ORI_PEAK_RATIO, feat);
		//free(ddata);
		free(feat);
		free(hist);
	}
	printf("%d", i);
	//currentPtr = NULL;;
}