// 硬件引脚与参数配置
const int touchSensorPin = 4;
const int indicatorLedPin = 2;
const int touchTriggerThreshold = 500;    // 触摸触发临界值，根据实际硬件调整
const unsigned long debounceInterval = 200; // 触摸消抖间隔，单位毫秒

bool ledCurrentStatus = false;
volatile bool touchIntTriggered = false;   // 中断触发标记，volatile保证跨上下文可见
unsigned long lastValidTouchTime = 0;      // 记录上次有效触摸的时间戳，用于消抖

// 触摸中断服务函数：仅置位标记，缩短中断执行时长
void IRAM_ATTR handleTouchInterrupt() {
  touchIntTriggered = true;
}

void setup() {
  Serial.begin(115200);
  delay(1000);

  // 初始化LED引脚并同步初始电平
  pinMode(indicatorLedPin, OUTPUT);
  digitalWrite(indicatorLedPin, ledCurrentStatus);

  // 绑定触摸中断：指定引脚、中断处理函数、触发阈值
  touchAttachInterrupt(touchSensorPin, handleTouchInterrupt, touchTriggerThreshold);
}

void loop() {
  // 检测触摸中断是否触发
  if (touchIntTriggered) {
    unsigned long currentTime = millis();
    // 软件消抖：间隔小于设定值的触发判定为信号抖动，直接忽略
    if (currentTime - lastValidTouchTime > debounceInterval) {
      ledCurrentStatus = !ledCurrentStatus;
      digitalWrite(indicatorLedPin, ledCurrentStatus);
      Serial.print("LED当前状态：");
      Serial.println(ledCurrentStatus ? "开启" : "关闭");
      lastValidTouchTime = currentTime;
    }
    touchIntTriggered = false; // 清除中断标记，等待下一次触发
  }

  // 主循环可扩展其他业务逻辑
  delay(10);
}