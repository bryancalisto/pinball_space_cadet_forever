import 'package:flutter/material.dart';
import 'package:memoryhacker/pages/control_page/control_page.dart';

/*
TODO:
- Change icon.
- Put animations on button.
- Implement info messages.
- Validate all cases are working with plugin.
- Validate pinball PE hash.
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
