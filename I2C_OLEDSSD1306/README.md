# I2C_OLED_SSD1306 – STM32F103C8T6

## Mục đích
- Hiển thị chữ/đồ họa trên **màn OLED SSD1306 128x64** sử dụng giao tiếp **I2C1**.
- Thực hành dùng thư viện tự viết `ssd1306.c / ssd1306.h`.
- Hiểu cách ghi **command**, **data**, **pixel**, và buffer màn hình.

---

## Cấu hình STM32CubeMX

### I2C1
- ClockSpeed: **100 kHz**
- Addressing Mode: **7-bit**
- SCL = PB6  
- SDA = PB7  
(CubeMX tự cấu hình khi bật I2C1)

### GPIO
- Bật clock:
  - `GPIOA`
  - `GPIOB`

### Clock hệ thống
- SYSCLK = **HSI** (8 MHz nội) hoặc bạn có thể đổi sang HSE + PLL.

---

## Thư viện SSD1306

### SSD1306_Init()
Gửi các lệnh khởi tạo màn hình:
- Bật/tắt màn hình
- Set địa chỉ trang
- Set contrast
- Toàn bộ command cấu hình phần cứng SSD1306

### SSD1306_Fill(color)
- Ghi toàn bộ buffer về **0x00** (đen) hoặc **0xFF** (trắng)

### SSD1306_UpdateScreen()
- Gửi toàn bộ buffer lên màn hình qua I2C

### SSD1306_DrawPixel(x, y, color)
- Set/reset pixel tại vị trí x–y trong buffer

### SSD1306_DrawChar(x, y, char)
- Vẽ ký tự dựa theo bitmap trong font 6x8

### SSD1306_DrawString(x, y, "text")
- In chuỗi bằng cách dịch từng ký tự 6 px

---

## Giao tiếp I2C với màn OLED
Địa chỉ OLED:
```
0x3C << 1 = 0x78  (chuẩn HAL)
```

Gói lệnh:
- Byte đầu: **0x00** → command  
- Byte đầu: **0x40** → data  

Ví dụ gửi command:
```c
uint8_t data[2] = {0x00, cmd};
HAL_I2C_Master_Transmit(&hi2c1, SSD1306_I2C_ADDR, data, 2, 100);
```

---

## Code sử dụng trong main.c

```c
SSD1306_Init();
SSD1306_Fill(0);
SSD1306_DrawString(10, 20, "anh ba");
SSD1306_UpdateScreen();
```

---

## Lưu ý phần cứng
- OLED SSD1306 dùng điện áp **3.3V** khi giao tiếp với STM32.
- Nếu module OLED của bạn có VIN 5V, nó đã có regulator, nhưng SDA/SCL vẫn là 3.3V.
- Bắt buộc nối **GND chung**.

---

## Debug lỗi thường gặp
| Lỗi | Nguyên nhân | Cách xử lý |
|-----|-------------|------------|
| Màn hình không sáng | Không bật nguồn/không nối GND | Kiểm tra VCC–GND |
| Chỉ hiện nhiễu | Sai địa chỉ I2C | Thử 0x3C hoặc 0x3D |
| Không hiển thị chữ | Không gọi UpdateScreen | Thêm `SSD1306_UpdateScreen()` |
| Không nhận I2C | Dây SCL/SDA bị đảo hoặc đứt | Kiểm tra lại PB6–PB7 |

---

## Kết luận
Thư viện SSD1306 giúp bạn điều khiển OLED 128x64 dễ dàng bằng I2C:  
- Vẽ pixel  
- In ký tự  
- In chuỗi  
- Điều khiển toàn bộ màn hình qua buffer  

Bạn chỉ cần gọi hàm trong main mà không phải lo việc gửi command phức tạp của OLED.
