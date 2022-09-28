import 'package:flutter/material.dart';
import 'package:memoryhacker/utils/native_bridge.dart';

class ControlPage extends StatefulWidget {
  const ControlPage({super.key});

  @override
  State<ControlPage> createState() => _ControlPageState();
}

class _ControlPageState extends State<ControlPage> {
  late bool hackIsActive;

  void _checkAndUpdateHackStatus() {
    final result = nativeIsHackActive();

    if (![0, 1].contains(result)) {
      // There was an error
      print('There was a error: $result');

      setState(() {
        hackIsActive = false;
      });

      return;
    }

    setState(() {
      hackIsActive = result == 1;
    });

    return;
  }

  @override
  void initState() {
    _checkAndUpdateHackStatus();
    super.initState();
  }

  void toggleHack() {
    setState(() {
      final result = nativeToggleHack(!hackIsActive);

      if (result != 0) {
        print('There was a error: $result');
        return;
      }

      hackIsActive = !hackIsActive;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Center(
        child: ElevatedButton(
          onPressed: toggleHack,
          child: Text(
            hackIsActive ? 'Deactivate' : 'Activate',
          ),
        ),
      ),
    );
  }
}
