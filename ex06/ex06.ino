// ex06/police_alternate_strobe.ino
// 警灯交替渐变效果 - 双LED反相亮度渐变
// 第一路灯亮度上升时第二路灯同步下降，循环往复形成交替频闪效果
// 硬件引脚：D2(GPIO2) + D4(GPIO4)

const int ledFirstPin = 2;    // 第一路灯输出引脚，对应板载D2
const int ledSecondPin = 4;   // 第二路灯输出引脚，对应板载D4，支持PWM输出

const int brightnessStep = 2;       // 每次亮度变化步长
const int stepDelayMs = 15;         // 每步停留时长，单位毫秒

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(ledFirstPin, OUTPUT);
  pinMode(ledSecondPin, OUTPUT);
  Serial.println("警灯交替渐变系统已启动");
}

void loop() {
  // 第一阶段：第一路灯从暗到亮，第二路灯同步从亮到暗
  for (int brightness = 0; brightness <= 255; brightness += brightnessStep) {
    analogWrite(ledFirstPin, brightness);
    analogWrite(ledSecondPin, 255 - brightness);
    delay(stepDelayMs);
  }

  // 第二阶段：第一路灯从亮到暗，第二路灯同步从暗到亮
  for (int brightness = 255; brightness >= 0; brightness -= brightnessStep) {
    analogWrite(ledFirstPin, brightness);
    analogWrite(ledSecondPin, 255 - brightness);
    delay(stepDelayMs);
  }
}