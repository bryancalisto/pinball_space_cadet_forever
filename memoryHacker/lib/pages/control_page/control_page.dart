import 'package:flutter/material.dart';
import 'package:memoryhacker/utils/constants.dart';
import 'package:memoryhacker/utils/native_bridge.dart';
import 'package:memoryhacker/pages/control_page/widgets/hack_toggle_button.dart';

class ControlPage extends StatefulWidget {
  const ControlPage({super.key});

  @override
  State<ControlPage> createState() => _ControlPageState();
}

class _ControlPageState extends State<ControlPage> {
  late bool _hackIsActive;
  String _msg = '';

  void _resetMsg() {
    _msg = '';
  }

  void _checkAndUpdateHackStatus() {
    final result = nativeIsHackActive();

    if (![0, 1].contains(result)) {
      setState(() {
        _hackIsActive = false;
        _msg = msgMap[result] ?? '';
      });

      return;
    }

    setState(() {
      _resetMsg();
      _hackIsActive = result == 1;
    });
  }

  @override
  void initState() {
    _checkAndUpdateHackStatus();
    super.initState();
  }

  void toggleHack() {
    final result = nativeToggleHack(!_hackIsActive);

    if (result != 0) {
      setState(() {
        _msg = msgMap[result] ?? '';
      });
      return;
    }

    setState(() {
      _resetMsg();
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
              HackToggleButton(onPressed: toggleHack, hackIsActive: _hackIsActive),
              _msg.isNotEmpty
                  ? Text(
                      _msg,
                      style: const TextStyle(
                        fontSize: 13,
                        color: Colors.white,
                      ),
                    )
                  : Container(),
            ],
          ),
        ),
      ),
    );
  }
}
