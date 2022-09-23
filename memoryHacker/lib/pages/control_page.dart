import 'package:flutter/material.dart';

class ControlPage extends StatefulWidget {
  const ControlPage({super.key});

  @override
  State<ControlPage> createState() => _ControlPageState();
}

class _ControlPageState extends State<ControlPage> {
  bool hackIsActive = false;

  void toggleHack() {
    setState(() {
      hackIsActive = !hackIsActive;
    });
  }

  @override
  Widget build(BuildContext context) {
    return Scaffold(
      body: Center(
        child: ElevatedButton(
          onPressed: toggleHack,
          child: Text(hackIsActive ? 'Deactivate' : 'Activate'),
        ),
      ),
    );
  }
}
