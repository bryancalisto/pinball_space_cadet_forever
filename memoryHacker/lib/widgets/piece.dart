import 'package:flutter/material.dart';

class Point {
  double x;
  double y;

  Point(this.x, this.y);
}

class Piece extends StatefulWidget {
  final List<Point> points;
  final Size size;
  final Color color;

  Piece({required this.points, required this.size, required this.color});

  @override
  State<Piece> createState() => _PieceState();
}

class _PieceState extends State<Piece> {
  late Color _color;
  late Size _size;
  bool _active = false;

  @override
  void initState() {
    _color = widget.color;
    _size = widget.size;

    super.initState();
  }

  @override
  Widget build(BuildContext context) {
    return InkWell(
      onTap: () {
        _color = _active ? Colors.black : _color;
      },
      onHover: (hovering) {
        print(hovering);
        setState(() {
          _color = !hovering ? Colors.black : _color;
          print(_color.toString());
        });
      },
      child: SizedBox(
        width: widget.size.width,
        height: widget.size.height,
        child: CustomPaint(
          painter: PiecePainter(points: widget.points, color: _color),
        ),
      ),
    );
  }
}

// TODO: Change this to a CustomClipper to define the shape of the piece for events (clicks, hover, etc)
class PiecePainter extends CustomPainter {
  final List<Point> points;
  final Color color;

  PiecePainter({required this.points, required this.color});

  @override
  void paint(Canvas canvas, Size size) {
    Paint paint = Paint()
      ..color = color
      ..style = PaintingStyle.fill;

    Path path = Path();

    path.moveTo(points[0].x * size.width, points[0].y * size.height);

    for (int i = 1; i < points.length; i++) {
      path.lineTo(points[i].x * size.width, points[i].y * size.height);
    }

    path.close();

    canvas.drawPath(path, paint);
  }

  @override
  bool shouldRepaint(CustomPainter oldDelegate) => true;
}
