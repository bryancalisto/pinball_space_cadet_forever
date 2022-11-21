import 'package:flutter/material.dart';
import 'package:flutter_conditional_rendering/conditional.dart';
import 'package:memoryhacker/pages/control_page/widgets/line.dart';
import 'package:memoryhacker/utils/constants.dart';
import 'package:memoryhacker/utils/native_bridge.dart';
import 'package:memoryhacker/pages/control_page/widgets/button.dart';

class ControlPage extends StatefulWidget {
  const ControlPage({super.key});

  @override
  State<ControlPage> createState() => _ControlPageState();
}

class _ControlPageState extends State<ControlPage> {
  late bool _hackIsActive;
  String _msg = '';

  void _checkAndUpdateHackStatus() {
    final result = nativeIsHackActive();

    setState(() {
      _hackIsActive = result == 1 ? true : false;
      _msg = msgMap[result] ?? '';
    });
  }

  @override
  void initState() {
    _checkAndUpdateHackStatus();
    super.initState();
  }

  void toggleHack() {
    final result = nativeToggleHack(!_hackIsActive);

    setState(() {
      if (result == Codes.ok || result == Codes.noOperation) {
        _hackIsActive = !_hackIsActive;
      } else if (pinballAppIsNotPresentCodes.contains(result)) {
        _hackIsActive = false;
      }

      _msg = msgMap[result] ?? '';
    });
  }

  @override
  Widget build(BuildContext context) {
    final button = Button(onPressed: toggleHack, hackIsActive: _hackIsActive);

    return Scaffold(
      body: Container(
        color: const Color.fromRGBO(37, 32, 32, 1),
        child: Center(
          child: Stack(
            alignment: Alignment.center,
            children: [
              Line(hackIsActive: _hackIsActive),
              Column(
                mainAxisAlignment: MainAxisAlignment.center,
                children: Conditional.list(
                  context: context,
                  conditionBuilder: (BuildContext context) {
                    return _msg.isEmpty;
                  },
                  widgetBuilder: (BuildContext context) {
                    return [button];
                  },
                  fallbackBuilder: (BuildContext context) {
                    return [
                      button,
                      const SizedBox(height: 10),
                      Text(
                        _msg,
                        style: const TextStyle(
                          fontSize: 13,
                          color: Colors.white,
                        ),
                      )
                    ];
                  },
                ),
              ),
            ],
          ),
        ),
      ),
    );
  }
}
