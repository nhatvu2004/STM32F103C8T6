# README_UART_BAITAP

## Mục tiêu
Ví dụ này minh họa **UART Receive Interrupt (RX IT)** trên STM32F1.  
Chương trình nhận 1 byte qua UART1, nếu byte nhận được là `'a'` thì bật LED PA0, ngược lại tắt LED.

---

## 1. Cấu hình UART trong STM32CubeMX
- USART1  
  - Baudrate: **115200**  
  - Word Length: **8 bits**  
  - Stop bit: **1**  
  - Parity: **None**  
  - Mode: **TX & RX**  
  - Hardware Flow Control: **None**

---

## 2. Callback xử lý ngắt UART
```c
uint8_t mData;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
    if(mData == 'a'){
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 1);
    } else {
        HAL_GPIO_WritePin(GPIOA, GPIO_PIN_0, 0);
    }

    HAL_UART_Receive_IT(&huart1, &mData, 1); // nhận byte tiếp theo
}
```

---

## 3. Khởi tạo UART nhận ngắt trong main()
```c
HAL_UART_Receive_IT(&huart1, &mData, 1);
```

---

## 4. Cấu hình LED PA0
- GPIOA Pin 0 → Output Push Pull  
- Speed: Low

---

## 5. Nguyên lý hoạt động
1. STM32 chờ 1 byte gửi đến UART1.  
2. Khi có dữ liệu → UART tạo ngắt → chạy `HAL_UART_RxCpltCallback`.  
3. Nếu ký tự `'a'` → LED sáng.  
4. Ngược lại → LED tắt.  
5. Kích hoạt lại UART Receive IT để nhận byte tiếp theo.

---

## 6. Test với Serial Terminal
Dùng:  
- RealTerm / TeraTerm / PuTTY  
- Gửi:  
  - `a` → LED PA0 bật  
  - ký tự khác → LED tắt

---

## 7. Lưu ý
- Hàm callback **phải rất ngắn gọn**, tránh delay.  
- Luôn gọi lại `HAL_UART_Receive_IT()` để không bị mất dữ liệu.

---

## 8. File tham khảo
`main.c` chứa đầy đủ ví dụ đã cung cấp.

---

## Done!
Bạn có thể mở rộng thêm:
- Điều khiển nhiều LED bằng nhiều ký tự  
- Tạo giao thức mini  
- Nhận chuỗi (string) bằng buffer + interrupt

