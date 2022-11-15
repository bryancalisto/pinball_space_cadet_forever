import 'package:flutter/material.dart';

class HackToggleButton extends StatelessWidget {
  final void Function() onPressed;
  final bool hackIsActive;
  final _color = const Color.fromRGBO(236, 226, 46, 1);

  const HackToggleButton({super.key, required this.onPressed, required this.hackIsActive});

  @override
  Widget build(BuildContext context) {
    final size = MediaQuery.of(context).size;

    return MaterialButton(
      onPressed: onPressed,
      color: hackIsActive ? _color : null,
      hoverColor: _color,
      child: const Padding(
        padding: EdgeInsets.all(7.0),
        child: Text(
          '💀',
          style: TextStyle(fontSize: 25),
        ),
      ),
    );
  }
}
