import 'dart:async';
import 'dart:typed_data';
import 'package:flutter/foundation.dart';
import 'package:flutter/material.dart';
import 'package:flutter_bluetooth_serial/flutter_bluetooth_serial.dart';

void main() {
  runApp(MyApp());
}

class MyApp extends StatefulWidget {
  @override
  _MyAppState createState() => _MyAppState();
}

class _MyAppState extends State<MyApp> {
  List<BluetoothDevice> _devices = [];
  late BluetoothConnection connection;
  String adr = "00:21:08:34:F7:A9"; // my bluetooth device MAC Adres

  @override
  void initState() {
    super.initState();
    _loadDevices();
  }

  Future<void> _loadDevices() async {
    List<BluetoothDevice> devices =
        await FlutterBluetoothSerial.instance.getBondedDevices();

    setState(() {
      _devices = devices;
    });
  }

//----------------------------
  Future<void> sendData(String data) async {
    data = data.trim();
    try {
      List<int> list = data.codeUnits;
      Uint8List bytes = Uint8List.fromList(list);
      connection.output.add(bytes);
      await connection.output.allSent;
      if (kDebugMode) {
        // print('Data sent successfully');
      }
    } catch (e) {
      //print(e.toString());
    }
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text("Dàn phơi thông minh"),
        ),
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              // const SizedBox(
              //   width: 400,
              //   child: TextField(
              //       keyboardType: TextInputType.text,
              //       autofillHints: Iterable.empty(),
              //       style: TextStyle(),
              //       decoration: InputDecoration(
              //           hintText: "Địa chỉ MAC", border: OutlineInputBorder())),
              // ),
              // const SizedBox(
              //   height: 10,
              // ),
              // const SizedBox(
              //   width: 400,
              //   child: TextField(
              //       keyboardType: TextInputType.text,
              //       autofillHints: Iterable.empty(),
              //       style: TextStyle(),
              //       decoration: InputDecoration(
              //           hintText: "Mật khẩu", border: OutlineInputBorder())),
              // ),
              Text(
                "Nhiệt độ : 32.6°C",
                style: TextStyle(color: Colors.blueAccent, fontSize: 50),
              ),
              Text(
                "Độ ẩm : 68%",
                style: TextStyle(color: Colors.blueAccent, fontSize: 50),
              ),
              SizedBox(
                height: 30.0,
              ),

              ElevatedButton(
                child: Text(" Bật "),
                onPressed: () {
                  print(123);
                },
              ),

              SizedBox(
                height: 10.0,
              ),
              ElevatedButton(
                child: Text(" Tắt "),
                onPressed: () {
                  sendData("off");
                },
              ),
            ],
          ),
        ),
      ),
      debugShowCheckedModeBanner: false,
    );
  }

  Future connect(String address) async {
    try {
      connection = await BluetoothConnection.toAddress(address);
      sendData('111');
      //durum="Connected to the device";
      connection.input!.listen((Uint8List data) {
        //Data entry point
        // durum=ascii.decode(data);
      });
    } catch (exception) {
      // durum="Cannot connect, exception occured";
    }
  }

// --------------**************data gonder
//Future send(Uint8List data) async {
//connection.output.add(data);
//await connection.output.allSent;
}

