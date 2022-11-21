import 'package:flutter/material.dart';

class Line extends StatelessWidget {
  final bool hackIsActive;

  const Line({super.key, required this.hackIsActive});

  @override
  Widget build(BuildContext context) {
    return AnimatedContainer(
      duration: const Duration(milliseconds: 450),
      height: 2,
      color: hackIsActive ? Colors.white : Colors.transparent,
      width: double.infinity,
    );
  }
}
