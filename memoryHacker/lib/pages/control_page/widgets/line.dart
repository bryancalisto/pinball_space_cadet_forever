import 'package:flutter/material.dart';
import 'package:memoryhacker/utils/constants.dart';

class Line extends StatelessWidget {
  final bool hackIsActive;

  const Line({super.key, required this.hackIsActive});

  @override
  Widget build(BuildContext context) {
    return AnimatedContainer(
      duration: animationDuration,
      height: 2,
      color: hackIsActive ? Colors.white : Colors.transparent,
      width: double.infinity,
    );
  }
}
