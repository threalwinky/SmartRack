// GENERATED CODE - DO NOT MODIFY BY HAND

part of 'socket.dart';

// **************************************************************************
// JsonSerializableGenerator
// **************************************************************************

TempHumi _$TempHumiFromJson(Map<String, dynamic> json) => TempHumi(
      (json['rain'] as num).toInt(),
      (json['tempC'] as num).toDouble(),
      (json['humi'] as num).toDouble(),
    );

Map<String, dynamic> _$TempHumiToJson(TempHumi instance) => <String, dynamic>{
      'rain': instance.rain,
      'tempC': instance.tempC,
      'humi': instance.humi,
    };