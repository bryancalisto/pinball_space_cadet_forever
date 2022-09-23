import 'package:flutter/material.dart';
import 'package:memoryhacker/pages/control_page.dart';

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Pinball Space Cadet Memory hacker',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: const ControlPage(),
    );
  }
}
