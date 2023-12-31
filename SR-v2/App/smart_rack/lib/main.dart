import 'dart:convert';

import 'package:flutter/material.dart';
import 'package:web_socket_channel/io.dart';
import 'package:web_socket_channel/status.dart' as status;
import 'temphumi.dart';

const String esp_url = 'ws://192.168.99.100:81';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({Key? key}) : super(key: key);

  // This widget is the root of your application.
  @override
  Widget build(BuildContext context) {
    return const MaterialApp(
      title: 'DHT11 eps-websocket-flutter',
      home: MyHomePage(title: 'Giàn phơi thông minh'),
      debugShowCheckedModeBanner: false,
    );
  }
}

class MyHomePage extends StatefulWidget {
  const MyHomePage({Key? key, required this.title}) : super(key: key);

  final String title;

  @override
  State<MyHomePage> createState() => _MyHomePageState();
}

class _MyHomePageState extends State<MyHomePage> {
  bool isLoaded = false;
  String msg = '';
  TempHumi dht = TempHumi(0, 0, 0);
  String rain_noti = "Không";
  final channel = IOWebSocketChannel.connect(esp_url);
  @override
  void initState() {
    super.initState();

    channel.stream.listen(
      (message) {
        channel.sink.add('Flutter received $message');
        if (message == "connected") {
          print('Received from MCU: $message');
          setState(() {
            msg = message;
          });
        } else {
          print('Received from MCU: $message');
          // {'tempC':'30.50','humi':'64.00'}
          Map<String, dynamic> json = jsonDecode(message);
          setState(() {
            dht = TempHumi.fromJson(json);
            isLoaded = true;
            if (dht.rain == 1) {
              rain_noti = "Không";
            } else {
              rain_noti = "Có";
            }
          });
        }
        //channel.sink.close(status.goingAway);
      },
      onDone: () {
        //if WebSocket is disconnected
        print("Web socket is closed");
        setState(() {
          msg = 'disconnected';
          isLoaded = false;
        });
      },
      onError: (error) {
        print(error.toString());
      },
    );
  }

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: const Text("Giàn phơi thông minh"),
        ),
        body: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Text(
                "Mưa : ${rain_noti}",
                style: TextStyle(color: Colors.blueAccent, fontSize: 50),
              ),
              Text(
                "Nhiệt độ : ${dht.tempC.toStringAsFixed(1)}°C",
                style: TextStyle(color: Colors.blueAccent, fontSize: 50),
              ),
              Text(
                "Độ ẩm : ${dht.humi.toStringAsFixed(1)}%",
                style: TextStyle(color: Colors.blueAccent, fontSize: 50),
              ),
              SizedBox(
                height: 30.0,
              ),
              ElevatedButton(
                child: Text(" Bật "),
                onPressed: () {
                  channel.sink.add("on");
                },
              ),
              SizedBox(
                height: 10.0,
              ),
              ElevatedButton(
                child: Text(" Tắt "),
                onPressed: () {
                  channel.sink.add("off");
                },
              ),
            ],
          ),
        ),
      ),
      debugShowCheckedModeBanner: false,
    );
  }
}
