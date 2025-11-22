# TIMER_DHT11 – Đọc cảm biến DHT11 bằng Timer us (STM32F103)

## Mục đích
- Đọc **nhiệt độ** và **độ ẩm** từ cảm biến **DHT11** bằng cách bit-banging trên chân **PA2**.
- Sử dụng **TIM2** tạo hàm `delay_us()` chính xác (1 µs).
- Thực hành đổi mode GPIO runtime: **Output ↔ Input** trên cùng một chân.

---

## Cấu hình chính (STM32CubeMX)

### Clock
- HSI → PLL (HSI/2 × 16) ≈ **64 MHz**.
- APB1 Prescaler = 2 → Clock cho TIM2 = 64 MHz (timer được nhân 2 trên APB1).

### TIM2 – dùng cho delay_us
- Prescaler = **63** → 64 MHz / (63 + 1) = **1 MHz** → 1 tick = 1 µs.
- Period = 65535.
- Mode: **Base Timer**, ClockSource Internal.

### TIM1
- Được cấu hình nhưng **chưa dùng** trong code (có thể dùng cho mục đích khác sau).

### GPIO
- PA2: dùng cho **data pin** của DHT11:
  - Khi Start: config **Output Push-Pull**.
  - Khi đọc dữ liệu: config **Input**.
- PA1: Input (chưa dùng).

---

## Hàm delay

```c
void delay_us(uint16_t time_us) {
    HAL_TIM_Base_Start(&htim2);
    htim2.Instance->CNT = 0;
    while (htim2.Instance->CNT < time_us) {}
    HAL_TIM_Base_Stop(&htim2);
}

void delay_ms(uint16_t time_ms) {
    for (uint16_t i = 0; i < time_ms; i++)
        delay_us(1000);
}
```

---

## Quy trình giao tiếp DHT11

### 1️⃣ Đổi mode chân PA2

```c
void setToOutput() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

void setToInput() {
    GPIO_InitTypeDef GPIO_InitStruct = {0};
    GPIO_InitStruct.Pin = GPIO_PIN_2;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}
```

### 2️⃣ Gửi tín hiệu Start

```c
void DHT11_Start() {
    setToOutput();
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1);
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);
    HAL_Delay(20);                 // kéo xuống ~18ms
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1);
    setToInput();

    // chờ DHT11 phản hồi: 80µs thấp + 80µs cao
    while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == 1) {}
    while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == 0) {}
    while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == 1) {}
    // sau đó DHT11 bắt đầu gửi 40 bit
}
```

### 3️⃣ Đọc 1 byte (8 bit)

```c
uint8_t DHT11_Read_byte() {
    uint8_t res = 0;
    for (uint8_t i = 0; i < 8; i++) {
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == 0) {}  // đợi lên 1
        delay_us(50);
        if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == 1) {
            res = (res << 1) | 1;     // bit 1
        } else {
            res = (res << 1) & ~1;    // bit 0
        }
        while (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_2) == 1) {}  // đợi về 0
    }
    return res;
}
```

### 4️⃣ Đọc 5 byte dữ liệu

```c
uint8_t int_Tem, int_Hum, float_Tem, float_Hum, CheckSum;

void DHT11_Handle() {
    DHT11_Start();
    int_Hum   = DHT11_Read_byte();
    float_Hum = DHT11_Read_byte();
    int_Tem   = DHT11_Read_byte();
    float_Tem = DHT11_Read_byte();
    CheckSum  = DHT11_Read_byte();
}
```

> ⚠ Trong code hiện tại **chưa kiểm tra checksum**. Chuẩn là:
> ```c
> if ((int_Hum + float_Hum + int_Tem + float_Tem) == CheckSum) { /* dữ liệu hợp lệ */ }
> ```

---

## Vòng lặp chính

```c
while (1) {
    DHT11_Handle();
    HAL_Delay(1000);    // đọc mỗi 1 giây
}
```

---

## Gợi ý sử dụng thêm
- In giá trị ra UART để debug:
  ```c
  printf("H = %d.%d %% | T = %d.%d C\r\n",
         int_Hum, float_Hum, int_Tem, float_Tem);
  ```
- Kết hợp với LCD/OLED để hiển thị trực tiếp.

---

## Kết luận
Project này tổng hợp:
- Timer-based delay µs (TIM2)
- Thay đổi mode GPIO runtime
- Đọc DHT11 bằng giao thức 1-wire đơn giản  

Là nền tảng tốt để bạn làm các bài **IoT – đo nhiệt độ, độ ẩm, hiển thị & gửi lên server**.
