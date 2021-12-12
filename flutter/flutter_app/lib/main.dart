import 'package:flutter/material.dart';

void main() => runApp(MyApp());

class MyApp extends StatelessWidget {   // 상태변화가 없는 위젯 (<-> StatefulWidget)
  @override
  Widget build(BuildContext context) {
    return MaterialApp(
      home: Scaffold(
        appBar: AppBar(
          title: Center(child: Text('First Practice'))
        ),
        //body: Container(child: Center(child: Text('Hello'))),
        body: Center(
          child: Image(
              image: NetworkImage(
              "https://img1.daumcdn.net/thumb/R1280x0/?scode=mtistory2&fname=https%3A%2F%2Fblog.kakaocdn.net%2Fdn%2FGax1Y%2FbtqV2WV6RAI%2Fv0qmrXOyRgC5OIS8laOCjK%2Fimg.png"
          )),
        ),
      ),
    );
  }
}