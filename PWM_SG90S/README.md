# PWM_SG90S – Điều khiển Servo SG90 bằng PWM (STM32 TIM2)

## Mục đích
Project này điều khiển servo **SG90 / MG90S** bằng PWM dùng **TIM2 – Channel 1** trên STM32F103.

Servo được điều khiển bằng xung PWM:
- Chu kỳ: **20 ms (50 Hz)**
- Xung:
  - 1 ms  → góc 0°
  - 2 ms  → góc 180°

---

## Cấu hình STM32CubeMX

### TIM2
- Prescaler = **0**
- Counter Mode = UP  
- Period = **19999**  
- Clock = 72 MHz →  
  `T = (Prescaler + 1) × (Period + 1) / Clock`  
  → 20 ms (chuẩn 50 Hz)

### PWM Channel
- TIM2_CH1 (PA0 hoặc PA15 tùy map)
- OC Mode: PWM1
- Pulse: 0

### GPIO
- PA0: Alternate Function Push-Pull
- Clock enable `GPIOA`

---

## Code điều khiển Servo

### Khởi tạo PWM
```c
void Servo_Init() {
    HAL_TIM_PWM_Start(&htim2, TIM_CHANNEL_1);
}
```

### Ghi góc servo
```c
void Servo_Write(uint8_t pwm) {
    uint16_t _ccr = (1000*pwm) / 180 + 1000;
    htim2.Instance->CCR1 = _ccr;
}
```

Giải thích:
- Góc 0°  → CCR = 1000  
- Góc 180° → CCR = 2000  
- Công thức tính xung theo góc

---

## Code trong main.c

```c
Servo_Init();

while(1) {
    Servo_Write(0);
    HAL_Delay(1000);

    Servo_Write(180);
    HAL_Delay(1000);
}
```

---

## Sơ đồ kết nối

| Servo SG90 | STM32 |
|-----------|-------|
| GND       | GND   |
| VCC (5V)  | 5V    |
| Signal    | PA0 (TIM2_CH1) |

⚠️ Servo phải dùng nguồn **5V riêng** nếu dòng yếu → tránh reset MCU.

---

## Debug lỗi thường gặp

| Lỗi | Nguyên nhân | Cách xử lý |
|-----|-------------|------------|
| Servo giật, rung | Nguồn yếu | Dùng nguồn 5V ổn định (≥ 1A) |
| Không quay | Chưa bật PWM | Kiểm tra `HAL_TIM_PWM_Start()` |
| Quay sai góc | Clock lệch | Đặt Prescaler/Period chuẩn 50 Hz |
| Servo nóng | Gửi PWM ngoài 1–2ms | Kiểm tra công thức CCR |

---

## Kết luận
Dùng TIM2 PWM để điều khiển Servo SG90 đơn giản, chính xác.  
Có thể mở rộng:
- Quét servo từ 0° → 180°
- Điều khiển servo theo cảm biến
- Làm robot arm nhiều bậc tự do

