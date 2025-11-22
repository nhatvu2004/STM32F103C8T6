# GPIO_LCD_LIB – STM32F103C8T6

## Mục đích
- Tách code điều khiển **LCD 16x2 (8-bit)** ra thành **thư viện LCD riêng (LCD.h / LCD.c)**.
- Dùng **struct LCD** để cấu hình linh hoạt các chân RS, RW, EN, D0–D7 và PORT tương ứng.
- Thực hành tổ chức project theo kiểu module: `main.c` chỉ khởi tạo và gọi hàm thư viện.

---

## Cấu trúc file
- `main.c`
  - Khai báo biến toàn cục:
    ```c
    LCD myLCD;
    ```
  - Gán cấu hình chân cho LCD:
    ```c
    myLCD.PORT_DATA = GPIOA;
    myLCD.PORT_R    = GPIOB;
    myLCD.D0 = GPIO_PIN_0;
    myLCD.D1 = GPIO_PIN_1;
    myLCD.D2 = GPIO_PIN_2;
    myLCD.D3 = GPIO_PIN_3;
    myLCD.D4 = GPIO_PIN_4;
    myLCD.D5 = GPIO_PIN_5;
    myLCD.D6 = GPIO_PIN_6;
    myLCD.D7 = GPIO_PIN_7;
    myLCD.RS = GPIO_PIN_0;
    myLCD.RW = GPIO_PIN_1;
    myLCD.En = GPIO_PIN_10;
    ```
  - Gọi hàm:
    ```c
    LCD_Init(&myLCD);
    LCD_Print(&myLCD, "Hello World");
    ```

- `LCD.h`
  - Định nghĩa struct LCD:
    ```c
    typedef struct {
        GPIO_TypeDef *PORT_DATA;
        GPIO_TypeDef *PORT_R;
        uint16_t D0, D1, D2, D3, D4, D5, D6, D7;
        uint16_t RS, RW, En;
    } LCD;
    ```
  - Khai báo hàm:
    ```c
    void LCD_Init(LCD *mLCD);
    void LCD_Print(LCD *mLCD, char *str);
    void LCD_Write_Byte(LCD *mLCD, uint8_t data, uint8_t mode);
    void LCD_Write_data(LCD *mLCD, uint8_t data);
    void LCD_Write_cmd(LCD *mLCD, uint8_t data);
    ```

- `LCD.c`
  - Cài đặt các hàm trên:
    - `LCD_Write_Byte`: xuất 8 bit ra D0–D7, điều khiển RS, kích xung En.
    - `LCD_Init`:
      ```c
      HAL_GPIO_WritePin(mLCD->PORT_R, mLCD->RW, 0);
      LCD_Write_cmd(mLCD, 0x38);   // 8-bit, 2 line, 5x7
      LCD_Write_cmd(mLCD, 0x0C);   // Display ON, Cursor OFF
      LCD_Write_cmd(mLCD, 0x06);   // Entry mode: left → right
      ```
    - `LCD_Print`: in chuỗi bằng cách gọi `LCD_Write_data` từng ký tự.

- `main.h`
  - Include thư viện:
    ```c
    #include "LCD.h"
    ```

---

## Cấu hình GPIO trong STM32CubeMX
- Bật clock:
  - `GPIOA`
  - `GPIOB`
- PA0–PA7: Output Push-Pull, No Pull, Low Speed (D0–D7).
- PB0, PB1, PB10: Output Push-Pull, No Pull, Low Speed (RS, RW, En).

Clock:
- HSE ON, PLL ×9 → **72 MHz**.

---

## Cách sử dụng
1. Thêm `LCD.c` và `LCD.h` vào project Keil / CubeIDE.
2. Chỉnh lại chân trong `main.c` cho đúng sơ đồ nối thực tế.
3. Gọi:
   ```c
   LCD_Init(&myLCD);
   LCD_Print(&myLCD, "Hello World");
   ```
4. Build và nạp code, kiểm tra LCD hiển thị.
