import 'dart:async';
import 'dart:convert';
import 'dart:typed_data';

import 'package:flutter/services.dart';

class MantraMfs100 {
  static const MethodChannel _channel = MethodChannel('mantra_mfs100');

  static Future<bool> get isInitialized async {
    final bool? result = await _channel.invokeMethod('isInitialized');
    print(result);
    return result ?? false;
  }

  static Future<Uint8List?> startScan() async {
    final data = await _channel.invokeListMethod("startScan");
    Uint8List? result;
    if(data!=null) {
       result = Uint8List.fromList(data.cast<int>());
       String s  = String.fromCharCodes(result);
       print(base64Encode(result));
    }
    return result;
  }
}
