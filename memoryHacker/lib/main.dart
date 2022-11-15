import 'package:flutter/material.dart';
import 'package:memoryhacker/pages/control_page/control_page.dart';

/*
TODO:
- Put animations on button.
- Implement info messages.
- Validate all cases are working with plugin.
- Validate pinball PE hash.
- Solve bug with stale 'isHackActive'. You can reproduce it by enabling the hack, closing pinball and reopening it. After
reopen, it should keep trying to enable the hack, even though the has is already enabled. Check '_checkAndUpdateHackStatus',
'toggleHack' and how '_hackIsActive' changes.
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
