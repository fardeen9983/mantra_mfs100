#include <Windows.h>
#include "MFS100Dll.h"
#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <memory.h>

int Wd =0;
int Ht =0;
int MatchingThreshold = 1400;
struct CapturedData
{
	int FingerNumber;
	int Quality;
	int Nfiq;
	unsigned char* FingerImage;
	unsigned char* IsoTemplate;
};

int Extract_ISO_19794_2(unsigned char* FinalFrame500DPI, unsigned char* ISO_19794_2)
{
	int ret = -1;
	unsigned char* Iso_19794_2_Temp;
	Iso_19794_2_Temp=(unsigned char*)malloc(1566);
	int Iso_19794_2_Temp_len =0;
	ret  = MFS100ExtractISOTemplate(FinalFrame500DPI,Iso_19794_2_Temp ,&Iso_19794_2_Temp_len);
	if(ret!=0)
	{
		return ret;
	}
	memcpy(ISO_19794_2,Iso_19794_2_Temp,Iso_19794_2_Temp_len);
	free(Iso_19794_2_Temp);
	ISO_19794_2[Iso_19794_2_Temp_len] = 0;
	return Iso_19794_2_Temp_len;
}


int Extract_ISO_19794_4(unsigned char* FinalFrame500DPI, int wt, int ht, unsigned char* ISO_19794_4)
{
	int ret = -1;
	unsigned char* Iso_19794_4_Temp;
	Iso_19794_4_Temp=(unsigned char*)malloc((wt*ht) + 1000);
	int Iso_19794_4_Temp_len =0;
	ret  = MFS100ExtractISOImage(FinalFrame500DPI,Iso_19794_4_Temp ,&Iso_19794_4_Temp_len,0);
	if(ret!=0)
	{
		return ret;
	}
	memcpy(ISO_19794_4,Iso_19794_4_Temp,Iso_19794_4_Temp_len);
	free(Iso_19794_4_Temp);
	ISO_19794_4[Iso_19794_4_Temp_len] = 0;
	return Iso_19794_4_Temp_len;
}


int Extract_ANSI_378_2004(unsigned char* FinalFrame500DPI, unsigned char* ANSI_378_2004)
{
	int ret = -1;
	unsigned char* Ansi_378_2004_Temp;
	Ansi_378_2004_Temp=(unsigned char*)malloc(1566);
	int Ansi_378_2004_Temp_len =0;
	ret  = MFS100ExtractANSITemplate(FinalFrame500DPI,Ansi_378_2004_Temp ,&Ansi_378_2004_Temp_len);
	if(ret!=0)
	{
		return ret;
	}
	memcpy(ANSI_378_2004,Ansi_378_2004_Temp,Ansi_378_2004_Temp_len);
	free(Ansi_378_2004_Temp);
	ANSI_378_2004[Ansi_378_2004_Temp_len] = 0;
	return Ansi_378_2004_Temp_len;
}

int Extract_WSQ(unsigned char* FinalFrame500DPI, unsigned char** WSQ)
{
	int ret = -1;
	int Wsq_Temp_len =0;
	ret = MFS100ExtractWSQImage(FinalFrame500DPI, WSQ, &Wsq_Temp_len, 1);
	if(ret!=0)
	{
		return ret;
	}
	return Wsq_Temp_len;
}

void PrintErrorMsg(int errorno)
{
    switch(errorno)
    {
		
		case MFS100_E_NO_DEVICE:
			printf("MFS100 not Found\n"); break;
		case MFS100_E_INVALIDPARAM:
			printf("Invalid Function parameter\n"); break;
		case MFS100_E_NULLPARAM:
			printf("Null Parameters\n"); break;
		case MFS100_E_MEMORY:
			printf("Not Enough Memory\n"); break;
		case MFS100_E_CORRUPT_SERIAL:
			printf("Serial no is corrupted\n"); break;
		case MFS100_E_UNKNOWN_SENSOR:
			printf("Unknown Sensor\n"); break;
		case MFS100_E_NOT_INITIALIZED:
			printf("MFS100 not initialized\n"); break;
		case MFS100_E_SYNC_PROBLEM:
			printf("MFS100 Sync Problem\n"); break;
		case MFS100_E_TIMEOUT:
			printf("Timeout from Function\n"); break;
		case MFS100_E_NOT_GOOD_IMAGE:
			printf("Input Image is not Good\n"); break;
		case MFS100_E_BLANKIMAGE:
			printf("Invalid image or unsupported image format\n"); break;
		case MFS100_E_EXTRACTOR_INIT_FAILED:
			printf("Extractor Library cannot Initialize\n"); break;
		case MFS100_E_FILE_IO:
			printf("Error occured while opening/reading file\n"); break;
		case MFS100_E_BAD_LICENSE:
			printf("Provided license is not valid, or no license was found\n"); break;
		case MFS100_E_BAD_FORMAT:
			printf("Unsupported Format\n"); break;
		case MFS100_E_BAD_VALUE:
			printf("Invalid Value Provided\n"); break;
		case MFS100_E_BAD_TEMPLATE:
			printf("Invalide template or unsupported template format\n"); break;
		case MFS100_E_READ_ONLY:
			printf("value cannot be modified\n"); break;
		case MFS100_E_NOT_DEFINED:
			printf("value is not defined\n"); break;
		case MFS100_E_NULL_TEMPLATE:
			printf("template is null\n"); break;
		case MFS100_E_NOT_IMPLEMENTED:
			printf("Function Not Implemented\n"); break;
		case MFS100_E_BAD_QUALITY:
			printf("Bad Quality Finger\n"); break;
		case MFS100_E_CAPTURING_STOPPED:
			printf("Capturing stopped\n"); break;
		case MFS100_E_LATENT_FINGER:
			printf("Latent Finger on device, clean and then initialize\n"); break;
		case MFS100_E_ALREADY_START_STOP:
			printf("MFS100 already started or already stopped\n"); break;
		case MFS100_E_LOAD_FIRMWARE_FAILED:
			printf("Firmware cannot load on device, try again or check security setting at udev\n"); break;
		case MFS100_E_OTHER:
			printf("Other unspecified Error\n"); break;
    }
}

void UnInitScaner()
{
	int ret = MFS100Uninit();
	printf("Uninit : %d", ret);
}
bool InitScanner()
{
	int DevicePresent=0;
	DevicePresent = MFS100DeviceConnected();
	if(DevicePresent!=0)
	{
		printf("Please connect device");
		return false;
	}
	char* make = (char*)malloc(10);
	char* model = (char*)malloc(10);
	int ret = MFS100GetInfo(make, model);
	if(ret!=0)
	{
		printf("Unable to recognize device");
		return false;
	}
	printf("Make: %s, Model: %s\n", make, model);


	unsigned char* serialno = (unsigned char*)malloc(11);
	ret = MFS100Init(serialno);
	if(ret!=0)
	{
		printf("Init Failed. Error: %d", ret);
		return false;
	}

	serialno[7] = 0;
	printf("Init Success. \nSerial Number: %s \n",serialno);

	Wd = MFS100GetWidth();
	Ht = MFS100GetHeight();

	printf("Image Width: %d \nImage Height: %d \n",Wd,Ht);
	
	free(serialno);
	return true;

}


void AsyncCapture()
{
	unsigned char* FinalFrame500DPI;
	FinalFrame500DPI = (unsigned char*)malloc(Ht*Wd);

	unsigned char* WSQImage = (unsigned char*)malloc((Wd * Ht) + 1500);
	int WSQImageSize = 0;

	unsigned char* Iso_19794_2_Template1;
	Iso_19794_2_Template1=(unsigned char*)malloc(1566);
	int Iso_19794_2_len = 0;

	unsigned char* ISOImage;
	ISOImage=(unsigned char*)malloc((Wd*Ht)+1000);
	int Iso_19794_4_len = 0;

	unsigned char* Ansi_378_2004_Template1;
	Ansi_378_2004_Template1=(unsigned char*)malloc(1566);
	int Ansi_378_2004_len = 0;

	unsigned char* Iso_19794_2_Template2;
	Iso_19794_2_Template2=(unsigned char*)malloc(1566);

	unsigned char* Ansi_378_2004_Template2;
	Ansi_378_2004_Template2=(unsigned char*)malloc(1566);

	try{
		printf("Place Finger and press enter to Start Scan..\n");
		getchar();

		int ret = MFS100StartXcan();
		if(ret != 0)
		{
			printf("Start Capture Error: %d \n",ret);
			goto finaly;
		}
		int i=0;int Quality = 0;int NFIQ = 0;int Threshold = 254;
		while (Quality >= 0 && Quality < 60)
		{
			ret = MFS100GetFrame(FinalFrame500DPI);
			if(ret != 0)
			{
				printf("Get frame error: %d \n",ret);
				goto finaly;
			}
			ret = MFS100GetQuality(FinalFrame500DPI,&Quality,&Threshold,&NFIQ);
			if(ret != 0)
			{
				printf("Get quality error: %d \n",ret);
				goto finaly;
			}
			printf("ret: %d, Quality: %d, Threshold: %d, NFIQ: %d\n",ret,Quality,Threshold,NFIQ);
			Sleep(100);
		}

		ret = MFS100StopXcan();
		if(ret!=0)
		{
			printf("StopScan Return: %d\n",ret);
			goto finaly;
		}	
		printf("StopScan Return: %d\n",ret);

		//Extract WSQ Image
		
		WSQImageSize = Extract_WSQ(FinalFrame500DPI,&WSQImage);
		if(WSQImageSize <= 0)
		{
			printf("Extract_WSQ error: %d \n",WSQImageSize);
			goto finaly;
		}
		else
		{
			printf("Extract_WSQ success with length: %d \n",WSQImageSize);
		}
		
		//Extract ISO19794-4 Image
		
		Iso_19794_4_len = 	Extract_ISO_19794_4(FinalFrame500DPI,Wd,Ht,ISOImage);
		if(Iso_19794_4_len <= 0)
		{
			printf("Extract_ISO_19794_4 error: %d \n",Iso_19794_4_len);
			goto finaly;
		}
		else
		{
			printf("Extract_ISO_19794_4 success with length: %d \n",Iso_19794_4_len);
		}

		//Extract ISO19794-2 Template
		
		Iso_19794_2_len = Extract_ISO_19794_2(FinalFrame500DPI,Iso_19794_2_Template1);
		if(Iso_19794_2_len <= 0)
		{
			printf("Extract_ISO_19794_2 error: %d \n",Iso_19794_2_len);
			goto finaly;
		}
		else
		{
			printf("Extract_ISO_19794_2 success with length: %d \n",Iso_19794_2_len);
		}
		
		//Extract ANSI Template
		
		Ansi_378_2004_len = Extract_ANSI_378_2004(FinalFrame500DPI,Ansi_378_2004_Template1);
		if(Ansi_378_2004_len <= 0)
		{
			printf("Extract_ANSI_378_2004 error: %d \n",Ansi_378_2004_len);
			goto finaly;
		}
		else
		{
			printf("Extract_ANSI_378_2004 success with length: %d \n",Ansi_378_2004_len);
		}
		//realloc(Ansi_378_2004_Template,*ANSITemplateLength);

		printf("ISO and ANSI Templates Extracted, Enter to capture another Finger\n");
		getchar();

		ret = MFS100StartXcan();
		if(ret != 0)
		{
			printf("Start Capture Error: %d \n",ret);
			goto finaly;
		}
		i=0;Quality = 0;NFIQ = 0;Threshold = 0;
		while (Quality >= 0 && Quality < 60)
		{
			ret = MFS100GetFrame(FinalFrame500DPI);
			if(ret != 0)
			{
				printf("Get frame error: %d \n",ret);
				goto finaly;
			}
			ret = 	MFS100GetQuality(FinalFrame500DPI,&Quality,&Threshold,&NFIQ);
			if(ret != 0)
			{
				printf("Get quality error: %d \n",ret);
				goto finaly;
			}
			printf("Quality: %d, Threshold: %d, NFIQ: %d\n",Quality,Threshold,NFIQ);
			Sleep(100);
		}

		//Img_Write_From_Bitmap_Array(FinalFrame500DPI, "fina2.bmp", Ht, Wd);

		ret = MFS100StopXcan();
		if(ret != 0)
		{
				printf("StopScan Return: %d\n",ret);
				goto finaly;
		}
		
		ret = Extract_ISO_19794_2(FinalFrame500DPI,Iso_19794_2_Template2);
		if(ret <= 0)
		{
			printf("Extract_ISO_19794_2 error: %d \n",ret);
			goto finaly;
		}
		else
		{
			printf("Extract_ISO_19794_2 success with length: %d \n",ret);
		}

		//Extract ANSI Template
		
		ret = Extract_ANSI_378_2004(FinalFrame500DPI,Ansi_378_2004_Template2);
		if(ret <= 0)
		{
			printf("Extract_ANSI_378_2004 error: %d \n",ret);
			goto finaly;
		}
		else
		{
			printf("Extract_ANSI_378_2004 success with length: %d \n",ret);
		}

		//Match ISO Template
		int score=0;
		ret = MFS100MatchISO(Iso_19794_2_Template1,Iso_19794_2_Template2,180,&score);
		if(ret <= 0)
		{
			printf("MFS100MatchISO error: %d \n",ret);
			goto finaly;
		}
		else
		{
			printf("ISO Template Matches with Score = %d\n",score);
		}
		
		//Match ANSI Template
		ret = MFS100MatchANSI(Ansi_378_2004_Template1,Ansi_378_2004_Template2,180,&score);
		if(ret <= 0)
		{
			printf("MFS100MatchANSI error: %d \n",ret);
			goto finaly;
		}
		else
		{
			printf("ANSI Template Matches with Score = %d\n",score);
		}
		

		printf("Enter to Uninit Sensor and Exit\n");
		getchar();

	}
	catch(char *excp){
		printf("Unhandled Exception: %s \n",excp);
	}
finaly:
	free(FinalFrame500DPI);
	free(Iso_19794_2_Template1);
	free(Ansi_378_2004_Template1);
	free(Iso_19794_2_Template2);
	free(Ansi_378_2004_Template2);
	free(ISOImage);
}


void _stdcall AutoCaptureCallback(unsigned char* RawImage)
{
	// do stuff here for show preview if required
	printf("%s\n","MFS100AutoCapture Callback Fires for preview...");
}


MFS100AutoCaptureCallback mfs100autocapturecallback = &AutoCaptureCallback;

void SyncCapture()
{
	int ret = -1;
	unsigned char* FinalFrame500DPI;
	FinalFrame500DPI = (unsigned char*)malloc(Ht*Wd);
	unsigned char *BitmapImageData;
	BitmapImageData = (unsigned char*)malloc((Ht*Wd) + 1079);

	unsigned char *Iso_19794_2_Template;
	Iso_19794_2_Template = (unsigned char*)malloc(1566);
	unsigned char *Ansi_378_2004_Template;
	Ansi_378_2004_Template = (unsigned char*)malloc(1566);
	int ISO_TemplateLength = 0;
	int ANSI_TemplateLength = 0;
	int Quality = 0;
	int NFIQ = 0;

	try
	{
		printf("%s\n","MFS100AutoCapture Starting, Place Finger to Capture within 10 secs");

		ret = MFS100AutoCapture(mfs100autocapturecallback, 10000, FinalFrame500DPI, BitmapImageData, Iso_19794_2_Template, &ISO_TemplateLength, Ansi_378_2004_Template, &ANSI_TemplateLength, &Quality, &NFIQ,0);

		if(ret!=0)
		{
			printf("Sync Capture error: %d \n",ret);
			goto finaly;
		}

		printf("Capture Success\n");
		printf("Quality: %d \n",Quality);
		printf("NFIQ: %d \n",NFIQ);
		printf("Iso 19794_2 Template Length: %d \n",ISO_TemplateLength);
		printf("Ansi 378_2004 Template Lenght: %d \n",ANSI_TemplateLength);
		
	}
	catch(char *excp){
		printf("Unhandled Exception: %s \n",excp);
	}
finaly:
	free(BitmapImageData);
	free(FinalFrame500DPI);
	free(Iso_19794_2_Template);
	free(Ansi_378_2004_Template);
}


void CaptureWithDeduplicate()
{
	int NoOfFinger = 0;
	int ret = 0;
	printf("Enter number of finger (1 to 10) for capture\n");
	scanf ("%d",&NoOfFinger);
	
	if(NoOfFinger<=0 || NoOfFinger>10)
	{
		printf("Invalid finger count\n");
		return;
	}
	struct CapturedData FingerData[10];

	for(int i = 0;i<NoOfFinger;i++)
	{
		printf("Put finger number %d and then press enter\n", i+1);
		getchar();

		unsigned char* FinalFrame500DPI;
		FinalFrame500DPI = (unsigned char*)malloc(Ht*Wd);
		unsigned char *BitmapImageData;
		BitmapImageData = (unsigned char*)malloc((Ht*Wd) + 1079);

		unsigned char *Iso_19794_2_Template;
		Iso_19794_2_Template = (unsigned char*)malloc(1566);
		unsigned char *Ansi_378_2004_Template;
		Ansi_378_2004_Template = (unsigned char*)malloc(1566);
		int ISO_TemplateLength = 0;
		int ANSI_TemplateLength = 0;
		int Quality = 0;
		int NFIQ = 0;

		try
		{
			ret = 0;
			ret = MFS100AutoCapture(mfs100autocapturecallback, 10000, FinalFrame500DPI, BitmapImageData, Iso_19794_2_Template, &ISO_TemplateLength, Ansi_378_2004_Template, &ANSI_TemplateLength, &Quality, &NFIQ,0);
			if(ret!=0)
			{
				printf("Capture error: %d \n",ret);
				printf("Finger number %d capture fail\n",i+1);
				goto finaly;
			}
			else
			{
				if(i>0)
				{
					for(int j = 0;j<i;j++)
					{
						int score=0;
						ret = MFS100MatchISO(Iso_19794_2_Template,FingerData[j].IsoTemplate,180,&score);
						if(ret!=0)
						{
							printf("Deduplicate check error: %d \n",ret);
							goto finaly;
						}
						if(score>=MatchingThreshold)
						{
							ret = -1;
							printf("Duplicate finger found with matching score %d with finger number %d\n",score, FingerData[j].FingerNumber);
							goto finaly;
						}
					}
				}
				FingerData[i].FingerImage = (unsigned char*)malloc((Ht*Wd) + 1079);
				FingerData[i].IsoTemplate = (unsigned char*)malloc(ISO_TemplateLength);
				FingerData[i].FingerNumber = i+1;
				FingerData[i].Quality = Quality;
				FingerData[i].Nfiq = NFIQ;
				memcpy(FingerData[i].FingerImage,BitmapImageData,(Ht*Wd) + 1079);
				memcpy(FingerData[i].IsoTemplate,Iso_19794_2_Template,ISO_TemplateLength);
				FingerData[i].IsoTemplate[ISO_TemplateLength]=0;
				printf("Finger number %d capture Success\n",i+1);
			}
			
		}
		catch(char *excp){
			printf("Unhandled Exception: %s \n",excp);
		}
		finaly:
			free(BitmapImageData);
			free(FinalFrame500DPI);
			free(Iso_19794_2_Template);
			free(Ansi_378_2004_Template);

			if(ret!=0)
			{
				break;
			}
	}
	if(ret==0)
	{
		printf("Do you want to print all data>? (y|n)\n");
		int yn = getchar();

		if(yn == 89|| yn == 121)
		{
			for(int i = 0;i<NoOfFinger;i++)
			{
				printf("Finger Number: %d, Quality: %d, Nfiq: %d\n",FingerData[i].FingerNumber, FingerData[i].Quality, FingerData[i].Nfiq);
				// you can utilize other variables as per your busines logic
			}
		}
	}
}