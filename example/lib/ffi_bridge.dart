import 'dart:ffi';
import 'dart:io';
import 'package:ffi/ffi.dart';

class FFIBridge {
  static bool initialize() {
    //nativeApiLib =    DynamicLibrary.open(Platform.script.resolve("build/windows/runner/Release/MFS100Dll.dll").toFilePath(windows: true));
    nativeApiLib =(DynamicLibrary.open('MFS100Dll.dll')); // android and linux
    final _add = nativeApiLib.lookup<NativeFunction<Int32 Function()>>('MFS100GetSDKVersion');
    add = _add.asFunction<int Function()>();
    // final _cap = nativeApiLib.lookup<NativeFunction<Pointer<Utf8> Function(Pointer<Utf8>)>>('capitalize');
    // _capitalize = _cap.asFunction<Pointer<Utf8> Function(Pointer<Utf8>)>();
    return true;
  }
  static late DynamicLibrary nativeApiLib;
  static late Function add;
  static late Function _capitalize;
  static String capitalize(String str) {
    final _str = str.toNativeUtf8();
    Pointer<Utf8> res = _capitalize(_str);
    calloc.free(_str);
    return res.toDartString();
  }
}