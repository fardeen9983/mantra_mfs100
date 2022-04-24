#include "include/mantra_mfs100/mantra_mfs100_plugin.h"
#include "include/mantra_mfs100/MFS100Dll.h"
//#include "include/mantra_mfs100/WSQ_library64.h"

#include "string"
// This must be included before many other Windows headers.
#include <windows.h>

// For getPlatformVersion; remove unless needed for your plugin implementation.
#include <VersionHelpers.h>

#include <flutter/method_channel.h>
#include <flutter/plugin_registrar_windows.h>
#include <flutter/standard_method_codec.h>

#include <map>
#include <memory>
#include <sstream>
#include <vector>

namespace
{
  using namespace std;
  class MantraMfs100Plugin : public flutter::Plugin
  {
  public:
    static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

    MantraMfs100Plugin();

    virtual ~MantraMfs100Plugin();

  private:
    // Called when a method is called on this plugin's channel from Dart.
    void HandleMethodCall(
        const flutter::MethodCall<flutter::EncodableValue> &method_call,
        std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);

    int Wd = 0;
    int Ht = 0;
    int MatchingThreshold = 1400;
    struct CapturedData
    {
      int FingerNumber;
      int Quality;
      int Nfiq;
      unsigned char *FingerImage;
      unsigned char *IsoTemplate;
    };

    int Extract_ISO_19794_2(unsigned char *FinalFrame500DPI, unsigned char *ISO_19794_2)
    {
      int ret = -1;
      unsigned char *Iso_19794_2_Temp;
      Iso_19794_2_Temp = (unsigned char *)malloc(1566);
      int Iso_19794_2_Temp_len = 0;
      ret = MFS100ExtractISOTemplate(FinalFrame500DPI, Iso_19794_2_Temp, &Iso_19794_2_Temp_len);
      if (ret != 0)
      {
        return ret;
      }
      memcpy(ISO_19794_2, Iso_19794_2_Temp, Iso_19794_2_Temp_len);
      free(Iso_19794_2_Temp);
      ISO_19794_2[Iso_19794_2_Temp_len] = 0;
      return Iso_19794_2_Temp_len;
    }

    int Extract_ISO_19794_4(unsigned char *FinalFrame500DPI, int wt, int ht, unsigned char *ISO_19794_4)
    {
      int ret = -1;
      unsigned char *Iso_19794_4_Temp;
      Iso_19794_4_Temp = (unsigned char *)malloc((wt * ht) + 1000);
      int Iso_19794_4_Temp_len = 0;
      ret = MFS100ExtractISOImage(FinalFrame500DPI, Iso_19794_4_Temp, &Iso_19794_4_Temp_len, 0);
      if (ret != 0)
      {
        return ret;
      }
      memcpy(ISO_19794_4, Iso_19794_4_Temp, Iso_19794_4_Temp_len);
      free(Iso_19794_4_Temp);
      ISO_19794_4[Iso_19794_4_Temp_len] = 0;
      return Iso_19794_4_Temp_len;
    }

    int Extract_ANSI_378_2004(unsigned char *FinalFrame500DPI, unsigned char *ANSI_378_2004)
    {
      int ret = -1;
      unsigned char *Ansi_378_2004_Temp;
      Ansi_378_2004_Temp = (unsigned char *)malloc(1566);
      int Ansi_378_2004_Temp_len = 0;
      ret = MFS100ExtractANSITemplate(FinalFrame500DPI, Ansi_378_2004_Temp, &Ansi_378_2004_Temp_len);
      if (ret != 0)
      {
        return ret;
      }
      memcpy(ANSI_378_2004, Ansi_378_2004_Temp, Ansi_378_2004_Temp_len);
      free(Ansi_378_2004_Temp);
      ANSI_378_2004[Ansi_378_2004_Temp_len] = 0;
      return Ansi_378_2004_Temp_len;
    }

    int Extract_WSQ(unsigned char *FinalFrame500DPI, unsigned char **WSQ)
    {
      int ret = -1;
      int Wsq_Temp_len = 0;
      ret = MFS100ExtractWSQImage(FinalFrame500DPI, WSQ, &Wsq_Temp_len, 1);
      if (ret != 0)
      {
        return ret;
      }
      return Wsq_Temp_len;
    }

    void PrintErrorMsg(int errorno)
    {
      switch (errorno)
      {

      case MFS100_E_NO_DEVICE:
        printf("MFS100 not Found\n");
        break;
      case MFS100_E_INVALIDPARAM:
        printf("Invalid Function parameter\n");
        break;
      case MFS100_E_NULLPARAM:
        printf("Null Parameters\n");
        break;
      case MFS100_E_MEMORY:
        printf("Not Enough Memory\n");
        break;
      case MFS100_E_CORRUPT_SERIAL:
        printf("Serial no is corrupted\n");
        break;
      case MFS100_E_UNKNOWN_SENSOR:
        printf("Unknown Sensor\n");
        break;
      case MFS100_E_NOT_INITIALIZED:
        printf("MFS100 not initialized\n");
        break;
      case MFS100_E_SYNC_PROBLEM:
        printf("MFS100 Sync Problem\n");
        break;
      case MFS100_E_TIMEOUT:
        printf("Timeout from Function\n");
        break;
      case MFS100_E_NOT_GOOD_IMAGE:
        printf("Input Image is not Good\n");
        break;
      case MFS100_E_BLANKIMAGE:
        printf("Invalid image or unsupported image format\n");
        break;
      case MFS100_E_EXTRACTOR_INIT_FAILED:
        printf("Extractor Library cannot Initialize\n");
        break;
      case MFS100_E_FILE_IO:
        printf("Error occured while opening/reading file\n");
        break;
      case MFS100_E_BAD_LICENSE:
        printf("Provided license is not valid, or no license was found\n");
        break;
      case MFS100_E_BAD_FORMAT:
        printf("Unsupported Format\n");
        break;
      case MFS100_E_BAD_VALUE:
        printf("Invalid Value Provided\n");
        break;
      case MFS100_E_BAD_TEMPLATE:
        printf("Invalide template or unsupported template format\n");
        break;
      case MFS100_E_READ_ONLY:
        printf("value cannot be modified\n");
        break;
      case MFS100_E_NOT_DEFINED:
        printf("value is not defined\n");
        break;
      case MFS100_E_NULL_TEMPLATE:
        printf("template is null\n");
        break;
      case MFS100_E_NOT_IMPLEMENTED:
        printf("Function Not Implemented\n");
        break;
      case MFS100_E_BAD_QUALITY:
        printf("Bad Quality Finger\n");
        break;
      case MFS100_E_CAPTURING_STOPPED:
        printf("Capturing stopped\n");
        break;
      case MFS100_E_LATENT_FINGER:
        printf("Latent Finger on device, clean and then initialize\n");
        break;
      case MFS100_E_ALREADY_START_STOP:
        printf("MFS100 already started or already stopped\n");
        break;
      case MFS100_E_LOAD_FIRMWARE_FAILED:
        printf("Firmware cannot load on device, try again or check security setting at udev\n");
        break;
      case MFS100_E_OTHER:
        printf("Other unspecified Error\n");
        break;
      }
    }

    void UnInitScaner()
    {
      int ret = MFS100Uninit();
      printf("Uninit : %d", ret);
    }

    bool InitScanner()
    {
      int DevicePresent = 0;
      DevicePresent = MFS100DeviceConnected();
      if (DevicePresent != 0)
      {
        printf("Please connect device");
        return false;
      }
      char *make = (char *)malloc(10);
      char *model = (char *)malloc(10);
      int ret = MFS100GetInfo(make, model);
      if (ret != 0)
      {
        printf("Unable to recognize device");
        return false;
      }
      printf("Make: %s, Model: %s\n", make, model);

      unsigned char *serialno = (unsigned char *)malloc(11);
      ret = MFS100Init(serialno);
      if (ret != 0)
      {
        printf("Init Failed. Error: %d", ret);
        return false;
      }

      serialno[7] = 0;
      printf("Init Success. \nSerial Number: %s \n", serialno);

      Wd = MFS100GetWidth();
      Ht = MFS100GetHeight();

      printf("Image Width: %d \nImage Height: %d \n", Wd, Ht);

      free(serialno);
      return true;
    }

    vector<uint8_t> AsyncCapture()
    {
      unsigned char *FinalFrame500DPI;
      FinalFrame500DPI = (unsigned char *)malloc(Ht * Wd);

      unsigned char *WSQImage = (unsigned char *)malloc((Wd * Ht) + 1500);
      int WSQImageSize = 0;

      // unsigned char *Iso_19794_2_Template1;
      // Iso_19794_2_Template1 = (unsigned char *)malloc(1566);
      // int Iso_19794_2_len = 0;

      // unsigned char *ISOImage;
      // ISOImage = (unsigned char *)malloc((Wd * Ht) + 1000);
      // int Iso_19794_4_len = 0;

      // unsigned char *Ansi_378_2004_Template1;
      // Ansi_378_2004_Template1 = (unsigned char *)malloc(1566);
      // int Ansi_378_2004_len = 0;

      // unsigned char *Iso_19794_2_Template2;
      // Iso_19794_2_Template2 = (unsigned char *)malloc(1566);

      // unsigned char *Ansi_378_2004_Template2;
      // Ansi_378_2004_Template2 = (unsigned char *)malloc(1566);

      try
      {
        int ret = MFS100StartXcan();
        if (ret != 0)
        {
          printf("Start Capture Error: %d \n", ret);
          goto finaly;
        }
        // int i = 0;
        int Quality = 0;
        int NFIQ = 0;
        int Threshold = 254;
        while (Quality >= 0 && Quality < 50)
        {
          ret = MFS100GetFrame(FinalFrame500DPI);
          if (ret != 0)
          {
            printf("Get frame error: %d \n", ret);
            goto finaly;
          }
          ret = MFS100GetQuality(FinalFrame500DPI, &Quality, &Threshold, &NFIQ);
          if (ret != 0)
          {
            printf("Get quality error: %d \n", ret);
            goto finaly;
          }
          printf("ret: %d, Quality: %d, Threshold: %d, NFIQ: %d\n", ret, Quality, Threshold, NFIQ);
          Sleep(100);
        }

        ret = MFS100StopXcan();
        if (ret != 0)
        {
          printf("StopScan Return: %d\n", ret);
          goto finaly;
        }
        printf("StopScan Return: %d\n", ret);

        // Extract WSQ Image

        WSQImageSize = Extract_WSQ(FinalFrame500DPI, &WSQImage);
        if (WSQImageSize <= 0)
        {
          printf("Extract_WSQ error: %d \n", WSQImageSize);
          goto finaly;
        }
        else
        {
          //RegisterWSQ();
          printf("Extract_WSQ success with length: %d \n", WSQImageSize);
          //const char *imageData = "C:\\Users\\khanf\\Downloads\\MFS100_Windows_9.0.2.5_Cpp\\MFS100_Windows_9.0.2.5_Cpp\\Sample\\x64\\Debug\\test.png";
          //SaveWSQByteArrayToImageFile(WSQImage, WSQImageSize, imageData, 4);
          vector<uint8_t> vecInt;
          vecInt.reserve(ret);

          for (int i = 0; i < WSQImageSize; i++)
          {
            vecInt.push_back((uint8_t)WSQImage[i]);
          }
          return vecInt;
        }

        // return vector<uint8_t>();

        // Extract ISO19794-4 Image
        // Iso_19794_4_len = Extract_ISO_19794_4(FinalFrame500DPI, Wd, Ht, ISOImage);
        // if (Iso_19794_4_len <= 0)
        // {
        //   printf("Extract_ISO_19794_4 error: %d \n", Iso_19794_4_len);
        //   goto finaly;
        // }
        // else
        // {
        //   printf("Extract_ISO_19794_4 success with length: %d \n", Iso_19794_4_len);
        // }
        // // Extract ISO19794-2 Template
        // Iso_19794_2_len = Extract_ISO_19794_2(FinalFrame500DPI, Iso_19794_2_Template1);
        // if (Iso_19794_2_len <= 0)
        // {
        //   printf("Extract_ISO_19794_2 error: %d \n", Iso_19794_2_len);
        //   goto finaly;
        // }
        // else
        // {
        //   printf("Extract_ISO_19794_2 success with length: %d \n", Iso_19794_2_len);
        // }
        // // Extract ANSI Template
        // Ansi_378_2004_len = Extract_ANSI_378_2004(FinalFrame500DPI, Ansi_378_2004_Template1);
        // if (Ansi_378_2004_len <= 0)
        // {
        //   printf("Extract_ANSI_378_2004 error: %d \n", Ansi_378_2004_len);
        //   goto finaly;
        // }
        // else
        // {
        //   printf("Extract_ANSI_378_2004 success with length: %d \n", Ansi_378_2004_len);
        // }
        // realloc(Ansi_378_2004_Template,*ANSITemplateLength);

        //   printf("ISO and ANSI Templates Extracted, Enter to capture another Finger\n");
        //   getchar();
        //
        //   ret = MFS100StartXcan();
        //   if (ret != 0)
        //   {
        //     printf("Start Capture Error: %d \n", ret);
        //     goto finaly;
        //   }
        //   i = 0;
        //   Quality = 0;
        //   NFIQ = 0;
        //   Threshold = 0;
        //   while (Quality >= 0 && Quality < 60)
        //   {
        //     ret = MFS100GetFrame(FinalFrame500DPI);
        //     if (ret != 0)
        //     {
        //       printf("Get frame error: %d \n", ret);
        //       goto finaly;
        //     }
        //     ret = MFS100GetQuality(FinalFrame500DPI, &Quality, &Threshold, &NFIQ);
        //     if (ret != 0)
        //     {
        //       printf("Get quality error: %d \n", ret);
        //       goto finaly;
        //     }
        //     printf("Quality: %d, Threshold: %d, NFIQ: %d\n", Quality, Threshold, NFIQ);
        //     Sleep(100);
        //   }
        //
        //   // Img_Write_From_Bitmap_Array(FinalFrame500DPI, "fina2.bmp", Ht, Wd);
        //
        //   ret = MFS100StopXcan();
        //   if (ret != 0)
        //   {
        //     printf("StopScan Return: %d\n", ret);
        //     goto finaly;
        //   }
        //
        //   ret = Extract_ISO_19794_2(FinalFrame500DPI, Iso_19794_2_Template2);
        //   if (ret <= 0)
        //   {
        //     printf("Extract_ISO_19794_2 error: %d \n", ret);
        //     goto finaly;
        //   }
        //   else
        //   {
        //     printf("Extract_ISO_19794_2 success with length: %d \n", ret);
        //   }
        //
        //   // Extract ANSI Template
        //
        //   ret = Extract_ANSI_378_2004(FinalFrame500DPI, Ansi_378_2004_Template2);
        //   if (ret <= 0)
        //   {
        //     printf("Extract_ANSI_378_2004 error: %d \n", ret);
        //     goto finaly;
        //   }
        //   else
        //   {
        //     printf("Extract_ANSI_378_2004 success with length: %d \n", ret);
        //   }
        //
        //   // Match ISO Template
        //   int score = 0;
        //   ret = MFS100MatchISO(Iso_19794_2_Template1, Iso_19794_2_Template2, 180, &score);
        //   if (ret <= 0)
        //   {
        //     printf("MFS100MatchISO error: %d \n", ret);
        //     goto finaly;
        //   }
        //   else
        //   {
        //     printf("ISO Template Matches with Score = %d\n", score);
        //   }
        //
        //   // Match ANSI Template
        //   ret = MFS100MatchANSI(Ansi_378_2004_Template1, Ansi_378_2004_Template2, 180, &score);
        //   if (ret <= 0)
        //   {
        //     printf("MFS100MatchANSI error: %d \n", ret);
        //     goto finaly;
        //   }
        //   else
        //   {
        //     printf("ANSI Template Matches with Score = %d\n", score);
        //   }
        //
        //   printf("Enter to Uninit Sensor and Exit\n");
        //   getchar();
      }
      catch (char *excp)
      {
        printf("Unhandled Exception: %s \n", excp);
      }
    finaly:
      free(FinalFrame500DPI);
      return vector<uint8_t>();

      // free(Iso_19794_2_Template1);
      // free(Ansi_378_2004_Template1);
      // free(Iso_19794_2_Template2);
      // free(Ansi_378_2004_Template2);
      // free(ISOImage);
    }

    void _stdcall AutoCaptureCallback(unsigned char *RawImage)
    {
      // do stuff here for show preview if required
      printf("%s\n", "MFS100AutoCapture Callback Fires for preview...");
    }

    // TODO MFS100AutoCaptureCallback mfs100autocapturecallback = &AutoCaptureCallback;
  };

  // static
  void MantraMfs100Plugin::RegisterWithRegistrar(
      flutter::PluginRegistrarWindows *registrar)
  {
    auto channel =
        std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
            registrar->messenger(), "mantra_mfs100",
            &flutter::StandardMethodCodec::GetInstance());

    auto plugin = std::make_unique<MantraMfs100Plugin>();

    channel->SetMethodCallHandler(
        [plugin_pointer = plugin.get()](const auto &call, auto result)
        {
          plugin_pointer->HandleMethodCall(call, std::move(result));
        });

    registrar->AddPlugin(std::move(plugin));
  }

  MantraMfs100Plugin::MantraMfs100Plugin() {}

  MantraMfs100Plugin::~MantraMfs100Plugin() {}

  void MantraMfs100Plugin::HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result)
  {
    if (method_call.method_name().compare("isInitialized") == 0)
    {
      result->Success(flutter::EncodableValue(InitScanner()));
    }
    else if (method_call.method_name().compare("startScan") == 0)
    {

      result->Success(flutter::EncodableValue(AsyncCapture()));
    }

    else
    {
      result->NotImplemented();
    }
  }

} // namespace

void MantraMfs100PluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar)
{
  MantraMfs100Plugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
