// ex05/multi_gear_touch_breath.ino
// 触摸式多档位呼吸灯控制器
// 单次触摸循环切换速度档位，共3档循环（1→2→3→1）

const int touchInputPin = 4;       // 触摸检测引脚
const int breathLedPin = 2;        // PWM呼吸灯输出引脚
const int touchTriggerThreshold = 500;  // 触摸触发阈值
const unsigned long debounceWindow = 200; // 触摸消抖时间窗口，单位毫秒

int currentGear = 1;               // 当前速度档位，取值1/2/3
volatile bool touchIntFlag = false; // 触摸中断触发标记
unsigned long lastValidTouchTime = 0; // 上次有效触摸的时间戳，用于消抖

// 各档位对应的呼吸步长与每步延时（档位越高，呼吸速度越快）
// 1档：步长2，延时15ms  慢速呼吸
// 2档：步长5，延时10ms  中速呼吸
// 3档：步长10，延时5ms  快速呼吸
const int gearStep[] = {0, 2, 5, 10};   // 数组下标对应档位，从1开始使用
const int gearDelay[] = {0, 15, 10, 5};

// 触摸中断服务函数：仅置位标记，缩短中断执行时长
void IRAM_ATTR handleTouchInterrupt() {
  touchIntFlag = true;
}

void setup() {
  Serial.begin(115200);
  delay(1000);
  pinMode(breathLedPin, OUTPUT);
  touchAttachInterrupt(touchInputPin, handleTouchInterrupt, touchTriggerThreshold);
  Serial.println("多速呼吸灯系统已启动，当前初始档位：1");
}

void loop() {
  // 检测触摸事件，切换档位
  if (touchIntFlag) {
    if (millis() - lastValidTouchTime > debounceWindow) {
      currentGear++;
      if (currentGear > 3) {
        currentGear = 1;
      }
      Serial.print("档位已切换，当前为：");
      Serial.println(currentGear);
      lastValidTouchTime = millis();
    }
    touchIntFlag = false;
  }

  // 根据当前档位获取呼吸参数
  int currentStep = gearStep[currentGear];
  int currentDelay = gearDelay[currentGear];

  // 亮度逐渐上升
  for (int dutyCycle = 0; dutyCycle <= 255; dutyCycle += currentStep) {
    analogWrite(breathLedPin, dutyCycle);
    delay(currentDelay);
    // 呼吸过程中同步检测触摸，保证档位切换响应及时
    scanTouchEvent();
  }

  // 亮度逐渐下降
  for (int dutyCycle = 255; dutyCycle >= 0; dutyCycle -= currentStep) {
    analogWrite(breathLedPin, dutyCycle);
    delay(currentDelay);
    scanTouchEvent();
  }
}

// 呼吸循环内的触摸检测函数，避免长循环中无法响应触摸
void scanTouchEvent() {
  if (touchIntFlag) {
    if (millis() - lastValidTouchTime > debounceWindow) {
      currentGear++;
      if (currentGear > 3) {
        currentGear = 1;
      }
      Serial.print("档位已切换，当前为：");
      Serial.println(currentGear);
      lastValidTouchTime = millis();
    }
    touchIntFlag = false;
  }
}
