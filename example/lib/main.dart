import 'dart:async';
import 'dart:typed_data';

import 'package:flutter/material.dart';
import 'package:flutter/services.dart';
import 'package:mantra_mfs100/mantra_mfs100.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatefulWidget {
  const MyApp({Key? key}) : super(key: key);

  @override
  State<MyApp> createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  String _platformVersion = 'Unknown';
  Uint8List uList = Uint8List(0);

  // Image? _image;
  @override
  void initState() {
    super.initState();
    //FFIBridge.initialize();
    initPlatformState();
  }

  // Platform messages are asynchronous, so we initialize in an async method.
  Future<void> initPlatformState() async {
    String platformVersion;
    Uint8List uList = Uint8List(0);
    // Image? image;
    // Platform messages may fail, so we use a try/catch PlatformException.
    // We also handle the message potentially returning null.
    try {
      platformVersion =
          //   await MantraMfs100.platformVersion ?? 'Unknown platform version';
//          ((await MantraMfs100.sdkVersion) ?? -1).toString();
          (await MantraMfs100.isInitialized).toString();

      // final codec = await instantiateImageCodec(uList);
      // final frameInfo = await codec.getNextFrame();
      // image = frameInfo.image as Image;
    } on PlatformException {
      platformVersion = 'Failed to get platform version.';
    }

    // If the widget was removed from the tree while the asynchronous platform
    // message was in flight, we want to discard the reply rather than calling
    // setState to update our non-existent appearance.
    if (!mounted) return;

    setState(() {
      _platformVersion = platformVersion;
      this.uList = uList;
      // _image = image;
    });
  }

  Map<String, List<double>> predefinedFilters = {
    'Identity': [
      //R  G   B    A  Const
      1, 0, 0, 0, 0, //
      0, 1, 0, 0, 0, //
      0, 0, 1, 0, 0, //
      0, 0, 0, 1, 0, //
    ],
    'Grey Scale': [
      //R  G   B    A  Const
      0.33, 0.59,0.11, 0,0,//
      0.33,0.59,0.11, 0,0,//
      0.33, 0.59,0.11, 0,0,//
      0, 0, 0, 1, 0, //
    ],
    'Inverse': [
      //R  G   B    A  Const
      -1, 0, 0, 0, 255, //
      0, -1, 0, 0, 255, //
      0, 0, -1, 0, 255, //
      0, 0, 0, 1, 0, //
    ],
    'Sepia': [
      //R  G   B    A  Const
      0.393, 0.769, 0.189, 0,0, //
      0.349,0.686,0.168,   0,0, //
      0.272,0.534,0.131,0,0, //
      0, 0, 0, 1, 0, //
    ],
  };

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text('Plugin example app'),
        ),
        body: Center(
          child: Column(
            children: [
              Text('Init Status: $_platformVersion\n'),
              SizedBox(
                height: 50,
              ),
              TextButton(
                  onPressed: () async {
                    uList = await MantraMfs100.startScan() ?? Uint8List(0);
                    setState(() {});
                  },
                  child: Text("Start Scanning")),
              SizedBox(
                height: 50,
              ),
              TextButton(onPressed: () {}, child: Text("Stop Scanning")),
              // uList.length > 0
              //     ? Container(
              //         width: 150,
              //         height: 150,
              //         decoration: BoxDecoration(
              //             image: DecorationImage(
              //                 fit: BoxFit.contain, image: MemoryImage(uList))),
              //       )
              //     : Container(),
              ColorFiltered(colorFilter: ColorFilter.matrix(predefinedFilters['Inverse']?.map<double>((entry) => entry)
                  .toList()?? List.empty()),
              child: Image.asset("assets/test.png"))
            ],
          ),
        ),
      ),
    );
  }
}
