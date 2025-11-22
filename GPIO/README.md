# GPIO – STM32F103C8T6

## Mục đích
- Thực hành GPIO input/output.
- Đọc nút nhấn PA1 và điều khiển LED PA3.
- Làm quen quy trình tạo project STM32CubeMX.

## Cấu hình STM32CubeMX
- Clock: HSE ON, PLL ×9 → 72 MHz.
- PA3: Output Push-Pull, No Pull.
- PA1: Input, No Pull (cần điện trở kéo ngoài).
- Bật clock GPIOA.

## Code chính
```
if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_1) == 0)
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 1);
else
    HAL_GPIO_WritePin(GPIOA, GPIO_PIN_3, 0);
HAL_Delay(100);
```
