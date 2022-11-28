import 'dart:convert';
import 'dart:io';
import 'package:crypto/crypto.dart';

Future<String> getFileMd5Hash(String filename) async {
  final bytes = await File(filename).readAsBytes();
  return md5.convert(bytes).toString();
}
