#include "include/mantra_mfs100/mantra_mfs100_plugin.h"
#include "include/mantra_mfs100/MFS100Dll.h"

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

namespace {

class MantraMfs100Plugin : public flutter::Plugin {
 public:
  static void RegisterWithRegistrar(flutter::PluginRegistrarWindows *registrar);

  MantraMfs100Plugin();

  virtual ~MantraMfs100Plugin();

  
 private:
  std::string GetPlatformVersion();
  std::int64_t GetSdkVersion();
  // Called when a method is called on this plugin's channel from Dart.
  void HandleMethodCall(
      const flutter::MethodCall<flutter::EncodableValue> &method_call,
      std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result);
};

// static
void MantraMfs100Plugin::RegisterWithRegistrar(
    flutter::PluginRegistrarWindows *registrar) {
  auto channel =
      std::make_unique<flutter::MethodChannel<flutter::EncodableValue>>(
          registrar->messenger(), "mantra_mfs100",
          &flutter::StandardMethodCodec::GetInstance());

  auto plugin = std::make_unique<MantraMfs100Plugin>();

  channel->SetMethodCallHandler(
      [plugin_pointer = plugin.get()](const auto &call, auto result) {
        plugin_pointer->HandleMethodCall(call, std::move(result));
      });

  registrar->AddPlugin(std::move(plugin));
}

MantraMfs100Plugin::MantraMfs100Plugin() {}

MantraMfs100Plugin::~MantraMfs100Plugin() {}

std::string MantraMfs100Plugin::GetPlatformVersion() {
  std::ostringstream version_stream;
    version_stream << "Windows ";
    if (IsWindows10OrGreater()) {
      version_stream << "10+";
    } else if (IsWindows8OrGreater()) {
      version_stream << "8";
    } else if (IsWindows7OrGreater()) {
      version_stream << "7";
    }
    return version_stream.str();
}

std::int64_t MantraMfs100Plugin::GetSdkVersion() {
    return MFS100GetSDKVersion();
}
void MantraMfs100Plugin::HandleMethodCall(
    const flutter::MethodCall<flutter::EncodableValue> &method_call,
    std::unique_ptr<flutter::MethodResult<flutter::EncodableValue>> result) {
  if (method_call.method_name().compare("getPlatformVersion") == 0) {
   
    result->Success(flutter::EncodableValue(GetPlatformVersion()));
  } else if(method_call.method_name().compare("getSdkVersion") == 0) {
    result->Success(flutter::EncodableValue(GetSdkVersion()));
  } else {
  result->NotImplemented();
  }
}

}  // namespace

void MantraMfs100PluginRegisterWithRegistrar(
    FlutterDesktopPluginRegistrarRef registrar) {
  MantraMfs100Plugin::RegisterWithRegistrar(
      flutter::PluginRegistrarManager::GetInstance()
          ->GetRegistrar<flutter::PluginRegistrarWindows>(registrar));
}
