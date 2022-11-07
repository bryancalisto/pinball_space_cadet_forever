import 'package:flutter/material.dart';
import 'package:memoryhacker/utils/native_bridge.dart';
import 'package:memoryhacker/pages/control_page/widgets/hack_toggle_button.dart';

class ControlPage extends StatefulWidget {
  const ControlPage({super.key});

  @override
  State<ControlPage> createState() => _ControlPageState();
}

class _ControlPageState extends State<ControlPage> {
  late bool _hackIsActive;
  String _msg = 'TEST';
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
      body: Container(
        color: const Color.fromRGBO(37, 32, 32, 1),
        child: Center(
          child: Column(
            mainAxisAlignment: MainAxisAlignment.center,
            children: [
              Text(
                _msg,
                style: TextStyle(
                  fontSize: MediaQuery.of(context).size.height * 0.03,
                  color: Colors.white,
                ),
              ),
              HackToggleButton(onPressed: toggleHack, hackIsActive: _hackIsActive),
            ],
          ),
        ),
      ),
    );
  }
}
