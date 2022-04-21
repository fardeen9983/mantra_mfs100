import 'dart:async';

import 'package:flutter/services.dart';

class MantraMfs100 {
  static const MethodChannel _channel = MethodChannel('mantra_mfs100');

  static Future<String?> get platformVersion async {
    final String? version = await _channel.invokeMethod('getPlatformVersion');
    return version;
  }
}
