# ADC – STM32F103C8T6

## Mục đích
- Thực hành sử dụng **ADC1** để đọc tín hiệu analog trên kênh **ADC_CHANNEL_0 (PA0)**.
- Chuyển đổi giá trị ADC 12-bit (0–4095) sang điện áp (0–3.3V).
- Làm quen với cách cấu hình ADC trong **STM32CubeMX** và dùng HAL (start, poll, get value, stop, calibration).

## Cấu hình chính trong STM32CubeMX

### Clock
- HSI ON, PLLSOURCE = HSI/2, PLL MUL16 → CPU chạy ~64 MHz.
- ADC clock: `RCC_ADCPCLK2_DIV8` (giảm tần số cho ADC).

### ADC1
- Instance: **ADC1**
- ScanConvMode: **Disable** (chỉ 1 kênh).
- ContinuousConvMode: **Disable** (tự tay gọi mỗi lần đọc).
- ExternalTrigConv: **ADC_SOFTWARE_START** (bắt đầu bằng phần mềm).
- DataAlign: **Right** (12-bit lệch phải).
- NbrOfConversion: **1**.

Channel:
- Channel: **ADC_CHANNEL_0**
- Rank: **1**
- SamplingTime: **239.5 cycles** (thời gian lấy mẫu dài, đọc ổn định hơn).

### GPIO
- Bật clock **GPIOA**.
- PA0 tự động cấu hình thành **Analog** (chân ADC_IN0).

## Cách hoạt động của chương trình

1. Khởi tạo HAL, clock, GPIO, ADC:
   ```c
   HAL_Init();
   SystemClock_Config();
   MX_GPIO_Init();
   MX_ADC1_Init();
   HAL_ADCEx_Calibration_Start(&hadc1);
   ```

2. Trong vòng lặp chính, mỗi 1 giây đọc ADC một lần:
   ```c
   while (1)
   {
       ADC_Handle();      // đọc ADC và cập nhật valueADC, _V
       HAL_Delay(1000);   // delay 1s
   }
   ```

3. Hàm xử lý ADC:
   ```c
   void ADC_Handle() {
       HAL_ADC_Start(&hadc1);
       HAL_ADC_PollForConversion(&hadc1, 1000);
       valueADC = HAL_ADC_GetValue(&hadc1);
       _V = ADCtoV(valueADC);
       HAL_ADC_Stop(&hadc1);
   }
   ```

4. Hàm đổi ADC sang Volt (Vref = 3.3V, 12-bit):
   ```c
   float ADCtoV(uint16_t x) {
       return (x * 3.3f) / 4095.0f;
   }
   ```

## Gợi ý kiểm tra
- Có thể debug bằng KeilC: **Watch window** xem `valueADC` và `_V` thay đổi khi xoay biến trở hoặc thay đổi điện áp vào PA0.
- Đảm bảo điện áp đưa vào PA0 luôn trong khoảng **0–3.3V** để tránh hỏng MCU.
