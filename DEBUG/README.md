# DEBUG trong KeilC – Hướng dẫn sử dụng

## 1. Mục đích
Tài liệu này hướng dẫn cách **sử dụng chế độ Debug của KeilC (µVision)** để theo dõi hoạt động của vi điều khiển STM32, bao gồm:
- Chạy từng bước (Step)
- Quan sát biến (Watch)
- Quan sát thanh ghi (Registers)
- Kiểm tra GPIO hoạt động đúng hay không
- Dừng chương trình tại breakpoint

---

## 2. Bật chế độ Debug trong KeilC
### Bước 1: Build project
Nhấn:
```
F7  hoặc  Project → Build Target
```
Nếu không lỗi, tiếp tục bước 2.

### Bước 2: Chọn Debugger
Vào:
```
Project → Options for Target → Debug
```
Chọn:
```
ST-Link Debugger
```

### Bước 3: Load chương trình vào MCU
Nhấn:
```
LOAD  hoặc  Ctrl + F8
```

### Bước 4: Vào chế độ Debug
Nhấn:
```
Debug → Start/Stop Debug Session
```

---

## 3. Các chức năng Debug quan trọng

### ✔ Run (Chạy chương trình)
```
F5
```

### ✔ Step Over (Chạy từng dòng)
```
F10
```

### ✔ Step Into (Đi vào hàm)
```
F11
```

### ✔ Stop (Dừng chương trình)
```
Ctrl + F5
```

### ✔ Reset MCU trong Debug
```
Debug → Reset
```

---

## 4. Đặt Breakpoint
Breakpoint giúp dừng chương trình tại 1 dòng cụ thể.

Cách đặt:
- Nhấn **F9**, hoặc
- Nhấn chuột trái vào bên trái dòng code

Dòng có breakpoint sẽ xuất hiện chấm đỏ.

---

## 5. Quan sát biến (Watch Window)

### Mở Watch Window
```
View → Watch Window → Watch1
```

### Thêm biến cần theo dõi
Nhập tên biến, ví dụ:
```
counter
adc_value
flag
```

Watch update theo từng bước chạy.

---

## 6. Kiểm tra GPIO trong Debug
Nếu bạn muốn xem trạng thái GPIO:

### Mở Peripheral Window
```
Peripherals → GPIO → GPIOA
```
Có thể quan sát:
- ODR (Output Data Register)
- IDR (Input Data Register)
- BSRR
- BRR

Ví dụ LED PA2:
- ODR bit 2 = 1 → LED bật
- ODR bit 2 = 0 → LED tắt

---

## 7. Kiểm tra hoạt động của code nháy LED
Dựa trên chương trình:

```
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 1);
HAL_Delay(1000);
HAL_GPIO_WritePin(GPIOA, GPIO_PIN_2, 0);
HAL_Delay(1000);
```

Trong Debug:
- Quan sát GPIOA->ODR bit 2 thay đổi 1 ↔ 0
- Quan sát delay bằng Step hoặc Run

Nếu LED trên board không nháy đúng → kiểm tra lại clock hoặc chân cấu hình.

---

## 8. Thoát Debug
Nhấn:
```
Debug → Start/Stop Debug Session
```
để quay về chế độ chỉnh sửa code.

---

## 9. Kết luận
Debug trong KeilC giúp:
- Theo dõi hoạt động của chương trình
- Hiểu rõ luồng lệnh
- Kiểm tra xem MCU chạy đúng hay không
- Giảm thời gian tìm lỗi khi lập trình

