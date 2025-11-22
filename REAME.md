# Tổng quan các giao thức giao tiếp trên STM32F103C8T6

---

## 1. GPIO (General Purpose Input Output)

- GPIO là các chân vào/ra số của vi điều khiển.  
- Có 2 chế độ chính:
  - **Input**: đọc trạng thái mức logic (0/1).
  - **Output**: xuất mức logic để điều khiển LED, relay, IC khác.
- Có thể cấu hình:
  - Pull-up / Pull-down  
  - Push-pull / Open-drain  
  - Speed (2MHz, 10MHz, 50MHz)

---

## 2. UART (Universal Asynchronous Receiver Transmitter)

- Giao tiếp nối tiếp không đồng bộ.  
- Dùng 2 dây:
  - **TX** – truyền dữ liệu
  - **RX** – nhận dữ liệu
- Tốc độ truyền (baudrate) phổ biến: 9600, 115200…  
- Một khung UART gồm:
  - Start bit  
  - 8 data bits  
  - Optional parity  
  - Stop bit  
- Ứng dụng: Debug, truyền dữ liệu với PC, module GPS, module WiFi, ESP8266…

---

## 3. UART Interrupt Mode

- ISR: **HAL_UART_RxCpltCallback()** được gọi khi nhận xong 1 byte.  
- Ưu điểm:
  - Không bị block chương trình
  - Nhận dữ liệu liên tục, real-time
- Cách dùng:
  - `HAL_UART_Receive_IT(&huart, &buffer, 1);`

---

## 4. SPI (Serial Peripheral Interface)

- Giao tiếp nối tiếp **đồng bộ**, tốc độ cao.  
- Dùng 4 dây:
  - **MOSI**: Master Out Slave In  
  - **MISO**: Master In Slave Out  
  - **SCK**: xung clock  
  - **SS/CS**: chọn slave  
- Ưu điểm:
  - Rất nhanh (MHz)  
  - Full-duplex  
- Nhược điểm:
  - Nhiều dây hơn I2C
- Ứng dụng: Flash, màn hình TFT, mạch thu phát RF…

---

## 5. I2C (Inter-Integrated Circuit)

- Giao tiếp nối tiếp **đồng bộ** nhưng chậm hơn SPI.  
- Dùng **2 dây**:
  - **SCL** – clock  
  - **SDA** – data  
- Một bus có thể có nhiều thiết bị (**multi-slave**)  
- Mỗi thiết bị có **địa chỉ 7-bit hoặc 10-bit**  
- Dùng cơ chế:
  - Start  
  - Address + Read/Write bit  
  - ACK/NACK  
  - Stop
- Ứng dụng: EEPROM, màn hình OLED, IMU MPU6050, cảm biến độ ẩm…

---

## 6. PWM (Pulse Width Modulation)

- Tín hiệu xung có chu kỳ cố định, nhưng **độ rộng xung (duty cycle)** thay đổi được.  
- Dùng Timer, chế độ PWM.  
- Ứng dụng:
  - Điều khiển servo SG90  
  - Điều chỉnh độ sáng LED  
  - Điều khiển tốc độ motor DC (qua driver)

---

## 7. Timer – Delay – Interrupt

### **Timer cơ bản**
- Timer đếm xung clock để tạo khoảng thời gian chính xác.  
- Dùng Prescaler + ARR (Period).

### **Delay us/ms**
- Tạo delay bằng cách đọc counter của timer (CTR).  
- Chính xác hơn HAL_Delay.

### **Timer Interrupt**
- ISR: `HAL_TIM_PeriodElapsedCallback()`  
- Ứng dụng:
  - Blink LED định kỳ
  - Thực hiện task theo chu kỳ (1ms, 10ms...)

---

## 8. Input Capture

- Chế độ của Timer dùng để **đo độ rộng xung**, **đo tần số**, **đo thời gian**.
- Timer ghi lại giá trị CNT khi có cạnh lên/xuống.
- Ứng dụng:
  - Đo tín hiệu từ encoder, cảm biến tốc độ
  - Xử lý xung ultrasonics HC-SR04

---

## 9. Output Compare

- Timer so sánh CNT với giá trị CCR.  
- Khi bằng nhau → tạo sự kiện (ngắt hoặc toggle chân).  
- Ứng dụng:
  - Tạo sóng vuông  
  - Tạo xung clock giả  
  - Phát sóng điều chế đơn giản

---

## 10. NVIC (Interrupt Controller)

- Quản lý ưu tiên ngắt.
- Mỗi ngoại vi (UART, SPI, EXTI, Timer…) đều có vector ngắt riêng.  
- Cho phép:
  - Enable/disable ngắt  
  - Set priority  
- Dùng để lập trình hệ thống real-time.

---

## 11. EXTI (External Interrupt)

- Ngắt từ chân GPIO.  
- Trigger:
  - Rising edge  
  - Falling edge  
  - Both edges  
- ISR: `HAL_GPIO_EXTI_Callback()`  
- Ứng dụng:
  - Nhấn nút  
  - Cảm biến PIR  
  - Encoder

---

# 📌 Tổng kết

Bạn đã thực hành gần như toàn bộ giao thức và ngoại vi quan trọng nhất của STM32:

- GPIO  
- UART (polling, interrupt)  
- SPI  
- I2C  
- PWM  
- Timer (delay us/ms)  
- Timer interrupt  
- Input Capture  
- EXTI  
- NVIC  

---

