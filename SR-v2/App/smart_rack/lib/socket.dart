import 'package:json_annotation/json_annotation.dart';

part 'socket.h.dart';

@JsonSerializable(explicitToJson: true)
class TempHumi {
  TempHumi(this.rain, this.tempC, this.humi);
  int rain;
  double tempC;
  double humi;
  factory TempHumi.fromJson(Map<String, dynamic> json) =>
      _$TempHumiFromJson(json);
  Map<String, dynamic> toJson() => _$TempHumiToJson(this);
}
