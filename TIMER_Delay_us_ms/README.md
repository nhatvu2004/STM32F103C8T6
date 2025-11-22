# TIMER_DELAY_US_MS – Tạo Delay microsecond & millisecond bằng TIM2 (STM32F103)

## Mục đích
Dự án này tạo hàm **delay_us()** và **delay_ms()** chính xác bằng cách sử dụng **TIM2** ở chế độ **Base Timer**.  
Timer chạy ở tần số 1 MHz → mỗi count = 1 microsecond.

---

## Cấu hình STM32CubeMX

### TIM2
- Prescaler = **63**  
  Vì clock APB1 (TIM2) = 64 MHz → (64 MHz / (63 + 1)) = **1 MHz**
- Counter Mode = UP  
- Period = **65535**
- Clock Division = DIV1

→ Timer đếm 1 µs mỗi tick.

### GPIO
- PA1 dùng test LED (không bắt buộc)

### Clock hệ thống
- SYSCLK = PLL từ HSI (8 MHz / 2 × 16 = **64 MHz**)

---

## Hàm delay_us()
```c
void delay_us(uint16_t time_us){
    HAL_TIM_Base_Start(&htim2);
    htim2.Instance->CNT = 0;       // reset counter

    while(htim2.Instance->CNT < time_us);

    HAL_TIM_Base_Stop(&htim2);
}
```

✔ Timer đếm microsecond  
✔ Không dùng HAL_Delay nên chính xác hơn  
✔ Dùng trong giao tiếp cần timing nhỏ (DHT11, ultrasonic…)

---

## Hàm delay_ms()
```c
void delay_ms(uint16_t time_ms){
    for(uint16_t i = 0; i < time_ms; i++)
        delay_us(1000);
}
```

---

## Test trong main()
```c
while (1) {
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 1);
    delay_ms(1000);

    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_1, 0);
    delay_ms(1000);
}
```

LED PA1 sẽ nháy 1 Hz (1 giây sáng – 1 giây tắt).

---

## Ưu điểm so với HAL_Delay()
| HAL_Delay | delay_us / delay_ms |
|-----------|---------------------|
| Độ phân giải thấp (1 ms) | Chính xác tới microsecond |
| Dùng SysTick | Dùng TIM2 riêng biệt |
| Không phù hợp giao tiếp tốc độ cao | Phù hợp cho sensor timing |

---

## Ứng dụng
- Đọc cảm biến **DHT11/DHT22**
- Giao tiếp **1-Wire**
- Điều khiển **SRF05/HC-SR04**
- Delay chính xác trong driver tùy chỉnh

---

## Lưu ý
- delay_us() **busy-wait**, CPU bị block.  
- Không nên delay_us() quá lớn (vài ms), thay bằng delay_ms().
- Nếu Timer clock không phải 64MHz → cần tính lại Prescaler.

---

## Kết luận
Sử dụng TIM2 để tạo delay µs chính xác giúp xử lý nhiều loại sensor và giao tiếp timing-critical trên STM32F1.
