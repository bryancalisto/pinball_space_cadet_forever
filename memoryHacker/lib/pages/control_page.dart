import 'package:flutter/material.dart';
import 'package:memoryhacker/utils/native_bridge.dart';
import 'package:memoryhacker/widgets/background.dart';
import 'package:memoryhacker/widgets/piece.dart';

class ControlPage extends StatefulWidget {
  const ControlPage({super.key});

  @override
  State<ControlPage> createState() => _ControlPageState();
}

class _ControlPageState extends State<ControlPage> {
  late bool _hackIsActive;
  bool debug = true;

  void _checkAndUpdateHackStatus() {
    if (debug) {
      setState(() {
        _hackIsActive = false;
      });
      return;
    }
    final result = nativeIsHackActive();

    if (![0, 1].contains(result)) {
      // There was an error
      print('There was a error: $result');

      setState(() {
        _hackIsActive = false;
      });

      return;
    }

    setState(() {
      _hackIsActive = result == 1;
    });
  }

  @override
  void initState() {
    _checkAndUpdateHackStatus();
    super.initState();
  }

  void toggleHack() {
    if (debug) {
      setState(() {
        _hackIsActive = !_hackIsActive;
      });
      return;
    }
    setState(() {
      final result = nativeToggleHack(!_hackIsActive);

      if (result != 0) {
        print('There was a error: $result');
        return;
      }

      _hackIsActive = !_hackIsActive;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Stack(
        children: [
          const Background(),
          Center(
            child: ElevatedButton(
              onPressed: toggleHack,
              child: const Icon(Icons.abc),
            ),
          )
        ],
      ),
    );
  }
}
