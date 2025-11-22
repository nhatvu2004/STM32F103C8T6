# I2C_MPU6050 – STM32F103C8T6

## Mục đích
- Thực hành giao tiếp **I2C1** với cảm biến **MPU6050**.
- Đọc dữ liệu **Gyroscope** và **Accelerometer** từ thanh ghi.
- Hiểu cách dùng hàm HAL: `HAL_I2C_Mem_Read`, `HAL_I2C_Mem_Write`.

---

## Cấu hình STM32CubeMX

### I2C1
- ClockSpeed: **100 kHz**
- DutyCycle: **I2C_DUTYCYCLE_2**
- Addressing Mode: **7-bit**
- OwnAddress: 0 (không dùng)
- NoStretchMode: Disable

### GPIO
- Bật clock:
  - `GPIOA`
  - `GPIOB`
- PB6 → I2C1_SCL  
- PB7 → I2C1_SDA  
(Do CubeMX tự cấu hình khi bật I2C1)

---

## Địa chỉ MPU6050
- Địa chỉ 7-bit: **0x68**
- Khi dùng HAL (8-bit): **0xD0**

```c
#define ADD 0xD0   // 11010000
```

---

## Khởi tạo MPU6050
```c
void MPU6050Init() {
    uint8_t check;
    uint8_t mData;
    HAL_I2C_Mem_Read(&hi2c1, ADD, 0x75, 1, &check, 1, 1000);
    if(check == 0x68){
        mData = 0x00;  // Wake up
        HAL_I2C_Mem_Write(&hi2c1, ADD, 0x6B, 1, &mData, 1, 1000);
        mData = 0x07;  // Sample rate
        HAL_I2C_Mem_Write(&hi2c1, ADD, 0x19, 1, &mData, 1, 1000);
        mData = 0x00;  // Gyro config
        HAL_I2C_Mem_Write(&hi2c1, ADD, 0x1B, 1, &mData, 1, 1000);
        mData = 0x00;  // Accel config
        HAL_I2C_Mem_Write(&hi2c1, ADD, 0x1C, 1, &mData, 1, 1000);
    }
}
```

---

## Đọc Gyroscope
```c
void MPU6050ReadG(void) {
    uint8_t dataG[6];
    HAL_I2C_Mem_Read(&hi2c1, ADD, 0x43, 1, dataG, 6, 1000);
    gx = (int16_t)(dataG[0] << 8 | dataG[1]);
    gy = (int16_t)(dataG[2] << 8 | dataG[3]);
    gz = (int16_t)(dataG[4] << 8 | dataG[5]);
}
```

---

## Đọc Accelerometer
⚠ **Lỗi thường gặp**: Bạn đang đọc nhầm địa chỉ (0x43 là Gyro).  
Accel phải bắt đầu tại **0x3B**.

Đoạn đúng:
```c
void MPU6050ReadA(void) {
    uint8_t dataA[6];
    HAL_I2C_Mem_Read(&hi2c1, ADD, 0x3B, 1, dataA, 6, 1000);
    ax = (int16_t)(dataA[0] << 8 | dataA[1]);
    ay = (int16_t)(dataA[2] << 8 | dataA[3]);
    az = (int16_t)(dataA[4] << 8 | dataA[5]);
}
```

---

## Vòng lặp chính
```c
while (1) {
    MPU6050ReadG();
    MPU6050ReadA();
}
```

---

## Kiểm tra hoạt động
- Dùng **Watch Window** để xem giá trị `ax, ay, az, gx, gy, gz`.
- Khi xoay cảm biến:
  - Gyro thay đổi nhanh
  - Accel thay đổi theo hướng trọng lực

---

## Lưu ý quan trọng
- Dây I2C cần **pull-up 4.7kΩ** → SCL, SDA.
- Nguồn MPU6050: **3.3V**, không cắm 5V vào SDA/SCL.
