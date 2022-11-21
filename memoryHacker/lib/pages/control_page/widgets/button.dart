import 'package:flutter/material.dart';

class Button extends StatelessWidget {
  final void Function() onPressed;
  final bool hackIsActive;
  final _color = const Color.fromRGBO(236, 226, 46, 1);

  const Button({super.key, required this.onPressed, required this.hackIsActive});

  @override
  Widget build(BuildContext context) {
    return InkWell(
      onTap: onPressed,
      child: AnimatedContainer(
        decoration: BoxDecoration(
          borderRadius: BorderRadius.circular(50),
          color: hackIsActive ? _color : Colors.transparent,
        ),
        duration: const Duration(milliseconds: 400),
        padding: const EdgeInsets.all(6),
        child: const Text(
          '💀',
          style: TextStyle(fontSize: 25),
        ),
      ),
    );
  }
}
