# README_UART2.md
## UART – Nhận dữ liệu bằng Interrupt (USART1 RX)

### 1. Mục tiêu
Demo sử dụng **USART1** của STM32F103 để **nhận dữ liệu qua UART bằng ngắt (Interrupt)** với hàm `HAL_UART_Receive_IT()`.

- Khi có 1 byte nhận được, **ISR (Interrupt Service Routine)** sẽ được gọi qua callback:
  ```c
  void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart)
  ```

---

## 2. Cấu hình UART (USART1)

Các tham số trong CubeMX:

```c
huart1.Instance        = USART1;
huart1.Init.BaudRate   = 115200;
huart1.Init.WordLength = UART_WORDLENGTH_8B;
huart1.Init.StopBits   = UART_STOPBITS_1;
huart1.Init.Parity     = UART_PARITY_NONE;
huart1.Init.Mode       = UART_MODE_TX_RX;
huart1.Init.HwFlowCtl  = UART_HWCONTROL_NONE;
huart1.Init.OverSampling = UART_OVERSAMPLING_16;
```

Chân UART trên STM32F103:
- **TX**: PA9  
- **RX**: PA10  

---

## 3. Bật ngắt UART trong NVIC

Trong STM32CubeMX:
- Vào tab **NVIC** → bật:
  - `USART1 global interrupt` (Enable)

Cube sẽ tự sinh code cấu hình NVIC trong `stm32f1xx_it.c` và `syscalls`.

---

## 4. Biến nhận dữ liệu & hàm callback

### Biến global:

```c
uint8_t mData;
```

### Callback UART RX:

```c
void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
    if (huart->Instance == USART1) {
        // TODO: Xử lý dữ liệu mData ở đây (ví dụ: echo lại, parse command, ...)

        // Đăng ký lại interrupt để nhận tiếp
        HAL_UART_Receive_IT(&huart1, &mData, 1);
    }
}
```

> Lưu ý: phải **gọi lại** `HAL_UART_Receive_IT()` trong callback để tiếp tục nhận byte tiếp theo.

---

## 5. Khởi tạo nhận ngắt trong main()

Trong `main.c` sau khi gọi `MX_USART1_UART_Init()`:

```c
MX_USART1_UART_Init();

// Bắt đầu nhận 1 byte bằng interrupt
HAL_UART_Receive_IT(&huart1, &mData, 1);
```

---

## 6. Vòng lặp chính

```c
while (1)
{
    // Không cần làm gì, mọi việc xử lý RX nằm trong callback
}
```

---

## 7. Test với USB-UART

1. Kết nối:
   - STM32 PA10 (RX) ← TX của USB-UART
   - GND ↔ GND
2. Mở Serial Terminal (115200 8N1).
3. Gửi từng ký tự bất kỳ.
4. Mỗi khi 1 byte được gửi, `mData` sẽ chứa byte đó, và code trong `HAL_UART_RxCpltCallback()` sẽ chạy.

---

## 8. Gợi ý mở rộng

- Thêm xử lý:
  - Echo lại dữ liệu:
    ```c
    HAL_UART_Transmit(&huart1, &mData, 1, 100);
    ```
  - Lưu chuỗi vào buffer, detect ký tự xuống dòng `\n` để parse command.
- Dùng **DMA** cho nhận chuỗi dài hơn.

---

## 9. Tóm tắt luồng hoạt động

1. `HAL_UART_Receive_IT()` được gọi lần đầu trong `main()`.
2. Khi nhận xong 1 byte:
   - Hardware UART → sinh ngắt.
   - HAL gọi `HAL_UART_RxCpltCallback()`.
3. Trong callback:
   - Xử lý byte vừa nhận.
   - Gọi lại `HAL_UART_Receive_IT()` để chờ byte kế tiếp.

Như vậy UART RX sẽ hoạt động **liên tục, không block**, phù hợp cho các ứng dụng nhận dữ liệu nền.
