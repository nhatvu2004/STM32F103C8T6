# SPI – STM32F103C8T6 (SPI1 Master)

## Mục đích
- Thực hành giao tiếp **SPI1** ở chế độ **Master**.
- Dùng chân **PB0** làm **CS (Chip Select)** điều khiển bằng GPIO.
- Gửi byte dữ liệu mẫu `0xD9` định kỳ 500 ms (dùng để test logic SPI hoặc kết nối với slave ngoài).

---

## Cấu hình STM32CubeMX

### 1. SPI1
- Mode: **SPI Mode Master**
- Direction: **2 Lines (Full Duplex)**
- Data Size: **8-bit**
- Clock Polarity (CPOL): **Low**
- Clock Phase (CPHA): **1st Edge**
- NSS: **Software (SPI_NSS_SOFT)**
- BaudRate Prescaler: **2** (SPI clock = PCLK2 / 2)
- First Bit: **MSB First**

CubeMX sinh ra:
```c
hspi1.Instance = SPI1;
hspi1.Init.Mode = SPI_MODE_MASTER;
hspi1.Init.Direction = SPI_DIRECTION_2LINES;
hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
hspi1.Init.NSS = SPI_NSS_SOFT;
hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_2;
hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
```

### 2. GPIO
- Bật clock:
  - `GPIOA`
  - `GPIOB`
  - `GPIOD`
- Các chân SPI1 (do CubeMX tự cấu hình):
  - PA5 → SCK
  - PA6 → MISO
  - PA7 → MOSI

- Thêm 1 chân GPIO làm CS:
  - PB0 → **Output Push-Pull**, No Pull, Low Speed.

CubeMX sinh ra:
```c
GPIO_InitStruct.Pin = GPIO_PIN_0;
GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
GPIO_InitStruct.Pull = GPIO_NOPULL;
HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
```

### 3. Clock hệ thống
- SYSCLK lấy từ PLL (HSI/2 × 6) ≈ 24 MHz (theo code hiện tại).

---

## Chức năng chương trình

Hàm gửi 1 byte qua SPI với CS điều khiển bằng tay (manual chip select):

```c
void writeDataSPI(uint8_t data) {
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 0);     // CS xuống mức thấp
    HAL_SPI_Transmit(&hspi1, &data, 1, 1000);    // Gửi 1 byte
    HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1);     // Kéo CS lên lại
}
```

Trong `main()`:

```c
HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, 1); // CS ban đầu = 1 (không chọn slave)

while (1)
{
    writeDataSPI(0xD9);   // Gửi 0xD9 đến slave
    HAL_Delay(500);       // 500 ms
}
```

Mỗi 500 ms, một byte `0xD9` sẽ được gửi tới thiết bị slave SPI.

---

## Kết nối phần cứng (ví dụ)

| STM32 (Master) | Thiết bị SPI (Slave) |
|----------------|----------------------|
| PA5 (SCK)      | SCK                  |
| PA7 (MOSI)     | MOSI / SDI / DIN     |
| PA6 (MISO)     | MISO / SDO / DOUT    |
| PB0 (GPIO CS)  | CS / NSS             |
| GND            | GND                  |

Lưu ý:
- Slave thường **active low CS** (CS = 0 → được chọn).
- Đảm bảo chung GND giữa STM32 và slave.

---

## Gợi ý mở rộng
- Thêm hàm nhận dữ liệu: `HAL_SPI_Receive()` hoặc `HAL_SPI_TransmitReceive()`.
- Gửi nhiều byte (buffer) thay vì 1 byte.
- Kết nối với:
  - Module FLASH SPI
  - Module màn hình SPI (OLED/TFT)
  - IC shift register 74HC595
