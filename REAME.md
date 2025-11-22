# TIMER_INTERRUPT.md
## STM32 – Sử dụng Timer Interrupt (TIM1) để Toggle LED

### 1. Mục tiêu
Tạo ngắt định kỳ bằng TIM1 và bật/tắt LED tại chân PA1 trong hàm callback của timer.

---

## 2. Cấu hình Timer (TIM1)

- Timer: TIM1  
- Clock Source: Internal Clock  
- Prescaler: 63999  
- Period: 499  

Cách tính:
```
F_CPU = 64 MHz  
Prescaler = 63999 → Timer clock = 1 kHz (1 ms)  
Period = 499 → 500 ms mỗi lần ngắt  
```

LED sẽ nháy mỗi 0.5 giây.

---

## 3. Bật interrupt cho TIM1 trong NVIC

Trong CubeMX bật:

```
TIM1 Update Interrupt → ENABLE
```

---

## 4. Hàm callback xử lý timer interrupt

```c
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM1)
    {
        HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_1);
    }
}
```

---

## 5. Khởi động timer interrupt trong main.c

```c
MX_TIM1_Init();
HAL_TIM_Base_Start_IT(&htim1);
```

---

## 6. Cấu hình chân LED (PA1)

```c
GPIO_InitTypeDef GPIO_InitStruct = {0};
GPIO_InitStruct.Pin = GPIO_PIN_1;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);
```
