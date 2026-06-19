// 第二次修改：增加代码注释说明
// lab02 实验2 基础IO LED闪烁
const int ledPin = 2; // ESP32板载LED引脚

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  Serial.println("实验2运行中");
}

void loop() {
  digitalWrite(ledPin, HIGH);
  Serial.println("LED亮");
  delay(1000); // 亮1秒
  digitalWrite(ledPin, LOW);
  Serial.println("LED灭");
  delay(1000); // 灭1秒
}
