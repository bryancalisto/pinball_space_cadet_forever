import 'package:flutter/material.dart';
import 'package:memoryhacker/widgets/piece.dart';

class Background extends StatefulWidget {
  const Background({super.key});

  @override
  State<Background> createState() => _BackgroundState();
}

class _BackgroundState extends State<Background> {
  double _pieces = 10;

  List<Point> points1 = [
    Point(0, 0),
    Point(1, 0),
    Point(0.5, 0.5),
  ];

  List<Point> points2 = [
    Point(0, 0),
    Point(0, 1),
    Point(0.5, 0.5),
  ];

  @override
  Widget build(BuildContext context) {
    return Container(
      color: Colors.amber,
      child: Stack(
        children: [
          Piece(
            points: points1,
            size: MediaQuery.of(context).size,
            color: Colors.red,
          ),
          Piece(
            points: points2,
            size: MediaQuery.of(context).size,
            color: Colors.blue,
          ),
        ],
      ),
    );
  }
}
