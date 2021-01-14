import {Point} from './point.js';

export class Wave {
  constructor(color) {
    this.color = color;
  }

  resize(stageWidth, stageHeight) {
    this.stageWidth = stageWidth;
    this.stageHeight = stageHeight;

    /* 중간을 각각 넓이, 높이를 2로 나눈 값으로 지정 */
    this.centerX = stageWidth / 2;
    this.centerY = stageHeight / 2;

    this.init();
  }

  init() {
    /* 가운데 하나의 점 만들기 */
    this.point = new Point(this.centerX, this.centerY);
  }

  draw(ctx) {
    /*
    그리기의 경로를 시작하는 메소드
    https://developer.mozilla.org/ko/docs/Web/HTML/Canvas/Tutorial/Drawing_shapes
    https://developer.mozilla.org/en-US/docs/Web/API/CanvasRenderingContext2D/beginPath
    */
    ctx.beginPath();
    /* 
    호(arc)를 그리는 메소드를 이용하여 원을 그림 
    2 * Math.PI = 반지름 * 2 = 지름
    */
    ctx.arc(this.point.x, this.point.y, 30, 0, 2 * Math.PI);
    /* 색상 RED & 채우기 */
    ctx.fillStyle = '#ff0000';
    ctx.fill();
    ctx.closePath();

    /* 공의 위치 변경하기 */
    this.point.update();
  }
}