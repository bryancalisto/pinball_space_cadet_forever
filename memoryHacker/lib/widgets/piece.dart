import 'dart:math';

import 'package:flutter/material.dart';

class Point {
  double x;
  double y;

  Point(this.x, this.y);
}

class Piece extends StatefulWidget {
  final List<Point> points;
  final Color color;

  const Piece({super.key, required this.points, required this.color});

  @override
  State<Piece> createState() => _PieceState();
}

class _PieceState extends State<Piece> {
  late Color _color;

  @override
  void initState() {
    _color = widget.color;

    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return ClipPath(
      clipper: PieceClipper(points: widget.points),
      child: Material(
          color: _color,
          child: InkWell(
            hoverColor: Colors.amber.shade200,
            onTap: () {},
          )),
    );
  }
}

class PieceClipper extends CustomClipper<Path> {
  final List<Point> points;

  PieceClipper({required this.points});

  @override
  Path getClip(Size size) {
    Path path = Path();

    path.moveTo(points[0].x * size.width, points[0].y * size.height);

    for (int i = 1; i < points.length; i++) {
      path.lineTo(points[i].x * size.width, points[i].y * size.height);
    }

    path.close();

    return path;
  }

  @override
  bool shouldReclip(covariant CustomClipper<Path> oldClipper) => false;
}
