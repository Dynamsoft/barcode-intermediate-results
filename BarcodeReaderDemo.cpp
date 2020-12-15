#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string>

#include "opencv2/core.hpp"
#include "opencv2/imgproc.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/core/utility.hpp"
#include "opencv2/imgcodecs.hpp"

#if defined(_WIN64) || defined(_WIN32)
#include <windows.h>
#include <conio.h>
#define snprintf sprintf_s 
#else
#include <sys/time.h>
#endif

#include "DynamsoftBarcodeReader.h"
#include "DynamsoftCommon.h"
using namespace dynamsoft::dbr;
using namespace cv;
using namespace std;

typedef struct BarcodeFormatSet
{
	int barcodeFormatIds;
	int barcodeFormatIds_2;
}BarcodeFormatSet;

unsigned long GetTimestamp()
{
#if defined(_WIN64) || defined(_WIN32)
	return GetTickCount();
#else
	struct timeval timing;
	gettimeofday(&timing, NULL);
	return timing.tv_sec * 1000  + timing.tv_usec / 1000;
#endif
}

const int getBarcodeFormat(int index, BarcodeFormatSet* barcodeFormatSet)
{
	int ret = 0;
	switch (index)
	{
	case 1:
		barcodeFormatSet->barcodeFormatIds = BF_ALL;
		barcodeFormatSet->barcodeFormatIds_2 = BF2_POSTALCODE | BF2_DOTCODE;
		break;
	case 2:
		barcodeFormatSet->barcodeFormatIds = BF_ONED;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 3:
		barcodeFormatSet->barcodeFormatIds = BF_QR_CODE;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 4:
		barcodeFormatSet->barcodeFormatIds = BF_CODE_39;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 5:
		barcodeFormatSet->barcodeFormatIds = BF_CODE_128;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 6:
		barcodeFormatSet->barcodeFormatIds = BF_CODE_93;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 7:
		barcodeFormatSet->barcodeFormatIds = BF_CODABAR;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 8:
		barcodeFormatSet->barcodeFormatIds = BF_ITF;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 9:
		barcodeFormatSet->barcodeFormatIds = BF_INDUSTRIAL_25;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 10:
		barcodeFormatSet->barcodeFormatIds = BF_EAN_13;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 11:
		barcodeFormatSet->barcodeFormatIds = BF_EAN_8;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 12:
		barcodeFormatSet->barcodeFormatIds = BF_UPC_A;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 13:
		barcodeFormatSet->barcodeFormatIds = BF_UPC_E;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 14:
		barcodeFormatSet->barcodeFormatIds = BF_PDF417;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 15:
		barcodeFormatSet->barcodeFormatIds = BF_DATAMATRIX;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 16:
		barcodeFormatSet->barcodeFormatIds = BF_AZTEC;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 17:
		barcodeFormatSet->barcodeFormatIds = BF_CODE_39_EXTENDED;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 18:
		barcodeFormatSet->barcodeFormatIds = BF_MAXICODE;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 19:
		barcodeFormatSet->barcodeFormatIds = BF_GS1_DATABAR;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 20:
		barcodeFormatSet->barcodeFormatIds = BF_PATCHCODE;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 21:
		barcodeFormatSet->barcodeFormatIds = BF_GS1_COMPOSITE;
		barcodeFormatSet->barcodeFormatIds_2 = 0;
		break;
	case 22:
		barcodeFormatSet->barcodeFormatIds = 0;
		barcodeFormatSet->barcodeFormatIds_2 = BF2_POSTALCODE;
		break;
	case 23:
		barcodeFormatSet->barcodeFormatIds = 0;
		barcodeFormatSet->barcodeFormatIds_2 = BF2_DOTCODE;
		break;
	default:
		ret = -1;
		break;
	}
	return ret;
}

bool getImagePath(char* pImagePath)
{
	char buffer[512] = { 0 };
	bool bExit = false;
	size_t length = 0;
	FILE* fp = NULL;
	while (1)
	{
		printf("\r\n>> Step 1: Input your image file's full path:\r\n");
		fgets(buffer, 512, stdin);
		strtok(buffer, "\n");

		length = strlen(buffer);
		if (length > 0)
		{
			if (strlen(buffer) == 1 && (buffer[0] == 'q' || buffer[0] == 'Q'))
			{
				bExit = true;
				break;
			}

			memset(pImagePath, 0, 512);
			if (buffer[0] == '\'')
			{
				if(buffer[length - 3] == '\'')
					memcpy(pImagePath, buffer + 1, length - 4);
				else if(buffer[length - 2] == '\'')
					memcpy(pImagePath, buffer + 1, length - 3);
			}
			else
				memcpy(pImagePath, buffer, length);

			fp = fopen(pImagePath, "rb");
			if (fp != NULL)
			{
				fclose(fp);
				break;
			}
		}
		printf("Please input a valid path.\r\n");
	}
	return bExit;
}

bool setBarcodeFormat(BarcodeFormatSet* barcodeFormat)
{
	char buffer[512] = { 0 };
	bool bExit = false;
	size_t length = 0;
	int index = 0;
	while (1)
	{
		printf("\r\n>> Step 2: Choose a number for the format(s) of your barcode image:\r\n");
		printf("   1: All\r\n");
		printf("   2: OneD\r\n");
		printf("   3: QR Code\r\n");
		printf("   4: Code 39\r\n");
		printf("   5: Code 128\r\n");
		printf("   6: Code 93\r\n");
		printf("   7: Codabar\r\n");
		printf("   8: Interleaved 2 of 5\r\n");
		printf("   9: Industrial 2 of 5\r\n");
		printf("   10: EAN-13\r\n");
		printf("   11: EAN-8\r\n");
		printf("   12: UPC-A\r\n");
		printf("   13: UPC-E\r\n");
		printf("   14: PDF417\r\n");
		printf("   15: DATAMATRIX\r\n");
		printf("   16: AZTEC\r\n");
		printf("   17: Code 39 Extended\r\n");
		printf("   18: Maxicode\r\n");
		printf("   19: GS1 Databar\r\n");
		printf("   20: PatchCode\r\n");
		printf("   21: GS1 Composite\r\n");
		printf("   22: Postal  Code\r\n");
		printf("   23: DotCode\r\n");

		fgets(buffer, 512, stdin);
		strtok(buffer, "\n");

		length = strlen(buffer);
		if (length > 0)
		{

			if (strlen(buffer) == 1 && (buffer[0] == 'q' || buffer[0] == 'Q'))
			{
				bExit = true;
				break;
			}

			index = atoi(buffer);
			int ret = getBarcodeFormat(index, barcodeFormat);
			if (ret != -1)
				break;
		}

		if (bExit)
			break;

		printf("Please choose a valid number. \r\n");

	}
	return bExit;
}

void showImage(string windowName, Mat &img, double hScale, double wScale, int imgWidth, int imgHeight)
{
	if (hScale >= wScale && hScale > 1)
	{
		Mat newImage;
		resize(img, newImage, Size(int(imgWidth / hScale), int(imgHeight / hScale)));
		imshow(windowName, newImage);
		imwrite(windowName + ".jpg", newImage);
	}
	else if (hScale <= wScale && wScale > 1)
	{
		Mat newImage;
		resize(img, newImage, Size(int(imgWidth / wScale), int(imgHeight / wScale)));
		imshow(windowName, newImage);
		imwrite(windowName + ".jpg", newImage);
	}
	else 
	{
		imshow(windowName, img);
		imwrite(windowName + ".jpg", img);
	}

}

void destroyWindow()
{
	waitKey(0);
	destroyAllWindows();
}

int main(int argc, const char* argv[])
{
	const char* templateName = NULL;
	BarcodeFormatSet barcodeFormat = { 0,0 };
	char buffer[512] = { 0 };
	char imageFile[512] = {0};
	// char *imageFile = "D:/code/opencv-cpp-barcode/images/shop-floor-efficiency-02sm.jpg";
	int index = 0;
	int ret = 0;
	unsigned long timeBegin = 0;
	unsigned long timeEnd = 0;

	size_t length = 0;
	FILE* fp = NULL;
	bool bExit = false;
	char errorMSG[256];
	PublicRuntimeSettings runtimeSettings;

	int maxWidth = 800, maxHeight = 800, padding = 5;

	printf("*************************************************\r\n");
	printf("Welcome to Dynamsoft Barcode Reader Demo\r\n");
	printf("*************************************************\r\n");
	printf("Hints: Please input 'Q' or 'q' to quit the application.\r\n");

	CBarcodeReader reader;
	// Get a free trial license from https://www.dynamsoft.com/customer/license/trialLicense
	ret = reader.InitLicense("LICENSE-KEY");

	while (1)
	{
		double hScale = 1.0, wScale = 1.0;

		bExit = getImagePath(imageFile);
		if (bExit)
			break;

		bExit = setBarcodeFormat(&barcodeFormat);
		if (bExit)
			break;

		reader.GetRuntimeSettings(&runtimeSettings);
		runtimeSettings.barcodeFormatIds = barcodeFormat.barcodeFormatIds;
		runtimeSettings.barcodeFormatIds_2 = barcodeFormat.barcodeFormatIds_2;
		// https://www.dynamsoft.com/barcode-reader/parameters/scenario-settings/intermediate-result.html
		runtimeSettings.intermediateResultSavingMode = IRSM_MEMORY;
 		runtimeSettings.intermediateResultTypes = IRT_BINARIZED_IMAGE | IRT_TRANSFORMED_GRAYSCALE_IMAGE | IRT_TYPED_BARCODE_ZONE;
		runtimeSettings.scaleDownThreshold = 10000;
		ret = reader.UpdateRuntimeSettings(&runtimeSettings, errorMSG, 256);
		if (ret != DBR_OK)
		{
			printf("Error code: %d. Error message: %s\n", ret, errorMSG);
			return -1;
		}

		// Read an image
		Mat img = imread(imageFile);
		int imgHeight = img.rows, imgWidth = img.cols;
		int thickness = 2;
		Scalar color(0, 255, 0);

		if (imgHeight > maxHeight) 
		{
			hScale = imgHeight * 1.0 / maxHeight;
			thickness = 6;
		}
			

		if (imgWidth > maxWidth)
		{
			wScale = imgWidth * 1.0 / maxWidth;
			thickness = 6;
		}

		// Decode barcodes
		timeBegin = GetTimestamp();
		// ret = reader.DecodeFile(imageFile, templateName);
		ret = reader.DecodeBuffer(img.data, imgWidth, imgHeight, img.step.p[0], IPF_RGB_888, templateName);
		timeEnd = GetTimestamp();

		unsigned long time = timeEnd - timeBegin;
		printf("\nDecode barcodes in %d ms.\n\n", time);

		if (ret != DBR_OK && ret != DBRERR_MAXICODE_LICENSE_INVALID && ret != DBRERR_AZTEC_LICENSE_INVALID && ret != DBRERR_LICENSE_EXPIRED && ret != DBRERR_QR_LICENSE_INVALID && ret != DBRERR_GS1_COMPOSITE_LICENSE_INVALID &&
			ret != DBRERR_1D_LICENSE_INVALID && ret != DBRERR_PDF417_LICENSE_INVALID && ret != DBRERR_DATAMATRIX_LICENSE_INVALID && ret != DBRERR_GS1_DATABAR_LICENSE_INVALID && ret != DBRERR_PATCHCODE_LICENSE_INVALID &&
			ret != DBRERR_POSTALCODE_LICENSE_INVALID && ret != DBRERR_DOTCODE_LICENSE_INVALID && ret != DBRERR_DPM_LICENSE_INVALID && ret != DBRERR_IRT_LICENSE_INVALID)
		{
			printf("Failed to read barcode: %s\r\n", CBarcodeReader::GetErrorString(ret));
			continue;
		}

		// Intermediate results
		// https://www.dynamsoft.com/barcode-reader/parameters/scenario-settings/intermediate-result.html
		IntermediateResultArray* intermediateResults = NULL;
		reader.GetIntermediateResults(&intermediateResults);
		for (int i = 0; i < intermediateResults->resultsCount; i++)
		{
			if(intermediateResults->results[i]->dataType == IMRDT_IMAGE)
			{
				int type = 0;
				ImageData* imageData = (ImageData*)intermediateResults->results[i]->results[0];
				switch(imageData->format)
				{
				case IPF_BINARY:
				case IPF_GRAYSCALED:
				case IPF_BINARYINVERTED:
					type = CV_8UC1;
					break;
				case IPF_RGB_888:
					type = CV_8UC3;
					break;
				default:
					break;
				}
				
				if (imageData->format == IPF_BINARY|| imageData->format == IPF_BINARYINVERTED)
				{
					int count = 0;
					int pos = 0;
					int whiteValue = imageData->format == IPF_BINARY?1:0;
					
					unsigned char* binData = new unsigned char[imageData->bytesLength<<3];
					while (pos < imageData->bytesLength)
					{
						for (int bit = 7; bit >= 0; --bit)
						{
							binData[count] = ((imageData->bytes[pos] >> bit) & 0x01) == whiteValue ? 255 : 0;
							count++;
						}
						pos++;
					}
					
					Mat img(imageData->height, imageData->width, type, binData, imageData->stride<<3);
					showImage("Binary Image", img, hScale, wScale, imgWidth, imgHeight);
					delete[] binData;
				}
				else 
				{
					Mat img(imageData->height, imageData->width, type, imageData->bytes, imageData->stride);
					showImage("Grayscale Image", img, hScale, wScale, imgWidth, imgHeight);
				} 
			}
			else if (intermediateResults->results[i]->dataType == IMRDT_LOCALIZATIONRESULT) 
			{
				LocalizationResult *pLocalizationResult = (LocalizationResult *)intermediateResults->results[i]->results[0];
				int x1 = pLocalizationResult->x1;
				int y1 = pLocalizationResult->y1;
				int minX = x1, minY = y1;
				int maxX = x1, maxY = y1;
				int x2 = pLocalizationResult->x2;
				int y2 = pLocalizationResult->y2;
				minX = minX < x2 ? minX : x2;
				minY = minY < y2 ? minY : y2;
				maxX = maxX > x2 ? maxX : x2;
				maxY = maxY > y2 ? maxY : y2;
				int x3 = pLocalizationResult->x3;
				int y3 = pLocalizationResult->y3;
				minX = minX < x3 ? minX : x3;
				minY = minY < y3 ? minY : y3;
				maxX = maxX > x3 ? maxX : x3;
				maxY = maxY > y3 ? maxY : y3;
				int x4 = pLocalizationResult->x4;
				int y4 = pLocalizationResult->y4;
				minX = minX < x4 ? minX : x4;
				minY = minY < y4 ? minY : y4;
				maxX = maxX > x4 ? maxX : x4;
				maxY = maxY > y4 ? maxY : y4;

				line( img, Point(x1, y1), Point(x2, y2), cv::Scalar(255, 0, 0), thickness);
				line( img, Point(x2, y2), Point(x3, y3), cv::Scalar(255, 0, 0), thickness);
				line( img, Point(x3, y3), Point(x4, y4), cv::Scalar(255, 0, 0), thickness);
				line( img, Point(x4, y4), Point(x1, y1), cv::Scalar(255, 0, 0), thickness);
				// rectangle(img, Point(minX - padding, minY - padding), Point(maxX + padding, maxY + padding), cv::Scalar(255, 0, 0), thickness);
			}
		}
		showImage("Localization", img, hScale, wScale, imgWidth, imgHeight);
		CBarcodeReader::FreeIntermediateResults(&intermediateResults);
		
		// Get final results
		TextResultArray *finalResults = NULL;
		reader.GetAllTextResults(&finalResults);

		if (finalResults->resultsCount == 0)
		{
			printf("No barcode found\n\n");
			CBarcodeReader::FreeTextResults(&finalResults);

			destroyWindow();
			continue;
		}

		printf("Total barcode(s) found: %d. \n\n", finalResults->resultsCount);
		
		for (int index = 0; index < finalResults->resultsCount; index++)
		{
			printf("Barcode %d:\r\n", index + 1);
			if (finalResults->results[index]->barcodeFormat != 0)
			{
				printf("    Type: %s\r\n", finalResults->results[index]->barcodeFormatString);
			}
			else
			{
				printf("    Type: %s\r\n", finalResults->results[index]->barcodeFormatString_2);
			}

			printf("    Value: %s\r\n", finalResults->results[index]->barcodeText);

			TextResult *barcode = finalResults->results[index];
			int x1 = barcode->localizationResult->x1;
			int y1 = barcode->localizationResult->y1;
			int minX = x1, minY = y1;
			int x2 = barcode->localizationResult->x2;
			int y2 = barcode->localizationResult->y2;
			minX = minX < x2 ? minX : x2;
			minY = minY < y2 ? minY : y2;
			int x3 = barcode->localizationResult->x3;
			int y3 = barcode->localizationResult->y3;
			minX = minX < x3 ? minX : x3;
			minY = minY < y3 ? minY : y3;
			int x4 = barcode->localizationResult->x4;
			int y4 = barcode->localizationResult->y4;
			minX = minX < x4 ? minX : x4;
			minY = minY < y4 ? minY : y4;

			line( img, Point(x1, y1), Point(x2, y2), color, thickness);
			line( img, Point(x2, y2), Point(x3, y3), color, thickness);
			line( img, Point(x3, y3), Point(x4, y4), color, thickness);
			line( img, Point(x4, y4), Point(x1, y1), color, thickness);

			putText(img, barcode->barcodeText, Point(minX, minY - 10), FONT_HERSHEY_COMPLEX, 1, Scalar(0, 0, 255),
            1, LINE_AA);
		}

		CBarcodeReader::FreeTextResults(&finalResults);
		
		showImage("Final", img, hScale, wScale, imgWidth, imgHeight);
		destroyWindow();
	}

	return 0;
}
