# LCD_GPIO – STM32F103C8T6

## Mục đích
- Điều khiển **LCD 16x2 giao tiếp song song 8-bit** bằng GPIO.
- Tự viết các hàm:
  - Gửi lệnh (command)
  - Gửi dữ liệu (data)
  - In chuỗi ký tự
- Hiểu cơ chế: **RS, E, D0–D7** của LCD.

---

## Sơ đồ chân sử dụng
- **RS** → PB0  
- **E** → PB10  
- **D0–D7** → PA0 → PA7  
- **PB1** dùng làm đèn nền (tùy bạn điều khiển)

---

## Cấu hình trong STM32CubeMX

### GPIOA (D0–D7)
- PA0–PA7 → Output Push-Pull, Low Speed

### GPIOB
- PB0 → RS (Output)
- PB10 → E  (Output)
- PB1 → điều khiển LED nền (Output)

### Clock
- HSE ON, PLL ×9 → **72 MHz**

---

## Chức năng chương trình

### Khởi tạo LCD
Lệnh 8-bit mode, 2 hàng, font 5×7:

```c
LCD_Write_cmd(0x38);
LCD_Write_cmd(0x0C);  // Display ON, Cursor OFF
LCD_Write_cmd(0x06);  // Entry mode: left → right
```

### Gửi 1 byte (command hoặc data)
```c
LCD_Write_Byte(uint8_t data, uint8_t mode)
```
- `mode = 1` → gửi **data**
- `mode = 0` → gửi **command**

Mỗi bit được đưa ra PA0–PA7 rồi kích xung E:

```c
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
HAL_Delay(1);
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 1);
HAL_Delay(1);
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_10, 0);
```

### In chuỗi
```c
LCD_Print("Hello World");
```

### Mã test trong main
```c
LCD_Init();
LCD_Print("Hello World");
LCD_Write_data('A');
```

---

## Lưu ý phần cứng
- LCD 16x2 cần nguồn **5V**, nhưng chân điều khiển vẫn đọc được mức 3.3V từ STM32.
- Nên nối chung GND giữa STM32 và LCD.
- Potentiometer 10k để chỉnh độ tương phản chân V0.

---

## Kiểm tra hoạt động
- Nếu LCD trắng xoá → sai tương phản (V0).
- Nếu không hiện gì → kiểm tra RS/E/D0–D7 đúng chân chưa.
- Dùng **logic analyzer** nếu LCD không nhận xung E.

