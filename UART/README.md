# README_UART.md
## UART – Truyền dữ liệu với USART1 (TX)

### 1. Mục tiêu
Gửi chuỗi ký tự liên tục qua UART1 của STM32F103 (TX = PA9) mỗi 1 giây.

---

## 2. Cấu hình UART
Các tham số trong CubeMX:

```
Baudrate: 115200
Word Length: 8 bits
Stop Bits: 1
Parity: None
Mode: TX/RX
Hardware Flow Control: None
Oversampling: 16
```

---

## 3. Chân UART trên STM32F103
| Tín hiệu | Chân MCU |
|---------|-----------|
| TX      | PA9       |
| RX      | PA10      |

---

## 4. Gửi dữ liệu bằng HAL UART

```c
char *dulieu2 = "a";

HAL_UART_Transmit(&huart1, (uint8_t*)dulieu2, strlen(dulieu2), 500);
HAL_Delay(1000);
```

Hàm `HAL_UART_Transmit()` sẽ gửi dữ liệu dạng blocking.

---

## 5. Cách kiểm tra UART
- Dùng USB-to-UART (CP2102, CH340, FT232…)  
- Nối dây:
```
STM32 PA9 (TX) → RX adapter
STM32 GND → GND adapter
```
- Mở Serial Monitor / PuTTY / RealTerm  
- Chọn **Baudrate 115200**  
- Bạn sẽ thấy ký tự **a** xuất ra mỗi giây.

---

## 6. Khởi tạo UART trong CubeMX (tự động sinh)

```c
huart1.Instance = USART1;
huart1.Init.BaudRate = 115200;
huart1.Init.WordLength = UART_WORDLENGTH_8B;
huart1.Init.StopBits = UART_STOPBITS_1;
huart1.Init.Parity = UART_PARITY_NONE;
huart1.Init.Mode = UART_MODE_TX_RX;
huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
huart1.Init.OverSampling = UART_OVERSAMPLING_16;
```

---

## 7. Lưu ý
- TX phải được đặt đúng Mode: **Alternate Function Push-Pull (AF_PP)**  
- Nếu không thấy dữ liệu → kiểm tra Baudrate, dây nối, và GND chung.  
- Nếu muốn nhận UART → dùng `HAL_UART_Receive()` hoặc interrupt/DMA.  

