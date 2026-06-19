// 增加10秒自动模式切换功能
// 新增SOS信号状态机逻辑
// lab02 升级版：millis()实现1Hz闪烁 + SOS信号自动切换（全非阻塞）
const int ledPin = 2; // ESP32板载LED引脚

// -------------------------- 1Hz闪烁参数 --------------------------
const long BLINK_INTERVAL = 1000; // 1Hz闪烁间隔（1000ms）
unsigned long lastBlinkTime = 0;
int ledState = LOW;

// -------------------------- SOS信号参数 --------------------------
const int SHORT_ON = 200;    // 短亮200ms
const int SHORT_OFF = 200;   // 短灭200ms
const int LONG_ON = 600;     // 长亮600ms
const int LONG_OFF = 200;    // 长灭200ms
const int CHAR_GAP = 500;    // 字母间隔500ms
const int WORD_GAP = 2000;   // SOS结束后长停顿2秒

unsigned long lastEventTime = 0;
int sosStage = 0; // SOS当前阶段（0-19）

// -------------------------- 模式切换参数 --------------------------
const long MODE_DURATION = 10000; // 每个模式运行10秒
int currentMode = 0; // 0=1Hz闪烁模式，1=SOS模式
unsigned long modeStartTime = 0;

void setup() {
  Serial.begin(115200);
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);
  modeStartTime = millis();
  Serial.println("lab02升级版启动：1Hz闪烁模式（millis()非阻塞）");
}

void loop() {
  unsigned long now = millis();

  // 1. 自动切换模式（每10秒切换一次）
  if (now - modeStartTime >= MODE_DURATION) {
    modeStartTime = now;
    currentMode = !currentMode;
    sosStage = 0; // 切换时重置SOS阶段
    if (currentMode == 0) {
      Serial.println("切换到：1Hz闪烁模式");
    } else {
      Serial.println("切换到：SOS求救信号模式");
    }
  }

  // 2. 执行当前模式逻辑
  if (currentMode == 0) {
    // 1Hz闪烁逻辑（纯millis()实现，无delay）
    if (now - lastBlinkTime >= BLINK_INTERVAL) {
      lastBlinkTime = now;
      ledState = !ledState;
      digitalWrite(ledPin, ledState);
      
      if (ledState == HIGH) {
        Serial.print(now);
        Serial.println("ms：LED点亮");
      } else {
        Serial.print(now);
        Serial.println("ms：LED熄灭");
      }
    }
  } else {
    // SOS信号逻辑（纯millis()实现，无delay）
    if (now - lastEventTime >= getSosDelay(sosStage)) {
      lastEventTime = now;
      runSosStage(sosStage);
      sosStage++;
      
      if (sosStage >= 20) {
        sosStage = 0;
        Serial.println("一轮SOS信号播放完成");
      }
    }
  }
}

// 执行SOS每个阶段的LED操作
void runSosStage(int stage) {
  switch (stage) {
    // 第一个S：3次短闪
    case 0: digitalWrite(ledPin, HIGH); break;
    case 1: digitalWrite(ledPin, LOW);  break;
    case 2: digitalWrite(ledPin, HIGH); break;
    case 3: digitalWrite(ledPin, LOW);  break;
    case 4: digitalWrite(ledPin, HIGH); break;
    case 5: digitalWrite(ledPin, LOW);  break;
    case 6: break;
    
    // O：3次长闪
    case 7: digitalWrite(ledPin, HIGH); break;
    case 8: digitalWrite(ledPin, LOW);  break;
    case 9: digitalWrite(ledPin, HIGH); break;
    case 10: digitalWrite(ledPin, LOW); break;
    case 11: digitalWrite(ledPin, HIGH); break;
    case 12: digitalWrite(ledPin, LOW); break;
    case 13: break;
    
    // 第二个S：3次短闪
    case 14: digitalWrite(ledPin, HIGH); break;
    case 15: digitalWrite(ledPin, LOW);  break;
    case 16: digitalWrite(ledPin, HIGH); break;
    case 17: digitalWrite(ledPin, LOW);  break;
    case 18: digitalWrite(ledPin, HIGH); break;
    case 19: digitalWrite(ledPin, LOW);  break;
  }
}

// 获取SOS每个阶段的等待时长
long getSosDelay(int stage) {
  switch (stage) {
    case 0: case 2: case 4: case 14: case 16: case 18:
      return SHORT_ON;
    case 1: case 3: case 5: case 15: case 17: case 19:
      return SHORT_OFF;
    case 7: case 9: case 11:
      return LONG_ON;
    case 8: case 10: case 12:
      return LONG_OFF;
    case 6: case 13:
      return CHAR_GAP;
    default:
      return WORD_GAP;
  }
}