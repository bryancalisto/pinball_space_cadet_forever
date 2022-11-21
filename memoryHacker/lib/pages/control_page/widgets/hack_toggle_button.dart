import 'package:flutter/material.dart';
import 'package:memoryhacker/utils/constants.dart';

class HackToggleButton extends StatelessWidget {
  final void Function() onPressed;
  final bool hackIsActive;
  final _color = const Color.fromRGBO(236, 226, 46, 1);

  const HackToggleButton({super.key, required this.onPressed, required this.hackIsActive});

  @override
  Widget build(BuildContext context) {
    return InkWell(
      onTap: onPressed,
      child: AnimatedContainer(
        decoration: BoxDecoration(
          borderRadius: BorderRadius.circular(50),
          color: hackIsActive ? _color : Colors.transparent,
        ),
        duration: animationDuration,
        padding: const EdgeInsets.all(6),
        child: const Text(
          '💀',
          style: TextStyle(fontSize: 25),
        ),
      ),
    );
  }
}
