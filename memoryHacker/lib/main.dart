import 'package:flutter/material.dart';
import 'package:memoryhacker/pages/control_page/control_page.dart';

/*
TODO:
- Put animations on button and draw a circular line that crosses all the width.
- Validate pinball PE hash.
- Setup retry-to-connect-to-program functionality.
*/

void main() {
  runApp(const MyApp());
}

class MyApp extends StatelessWidget {
  const MyApp({super.key});

  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      title: 'Pinball MemoryHacker',
      theme: ThemeData(
        primarySwatch: Colors.blue,
      ),
      home: const ControlPage(),
    );
  }
}
