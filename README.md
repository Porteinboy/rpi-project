# RPi4 IRS-90 & LED Control System

This project implements a complete **kernel-space + user-space** framework on Raspberry Pi 4 (BCM2711) to control IRS-90 line sensors and LEDs.  
It includes custom **HAL (Hardware Abstraction Layer) modules**, **device drivers**, and **user applications**.  

---

## 📂 Project Structure

rpi_project/
├── elf/ # (Reserved for compiled ELF outputs or debug)
├── kernel_modules/ # Pre-built kernel modules (.ko)
│ ├── irs_90_driver.ko
│ ├── irs_90_hal.ko
│ ├── led_driver.ko
│ └── led_hal.ko
├── kernel_space/ # Source code for kernel space
│ ├── include/ # Common header files
│ │ ├── irs_90_hal.h
│ │ ├── led_hal.h
│ │ ├── pin_mapping.h
│ │ └── register_map.h
│ ├── irs_90/ # IRS-90 sensor driver
│ │ ├── irs_90_driver.c
│ │ ├── irs_90_hal.c
│ │ └── Makefile
│ ├── led/ # LED driver
│ │ ├── led_driver.c
│ │ ├── led_hal.c
│ │ └── Makefile
│ └── script/ # Load/unload scripts
│ ├── device_load.sh
│ └── device_unload.sh
├── user_space/ # User-space applications
│ ├── Makefile
│ ├── mqtt/
│ │ └── simple_irs90_publisher.c
│ ├── sensor/
│ │ ├── Makefile
│ │ └── test_irs90_all.c
│ ├── sensor_led_controller # Compiled binary
│ ├── sensor_led_controller.c
│ ├── sensor_led_controller_B.c
│ └── uart/
│ ├── test_uart_14_15
│ ├── test_uart_14_15.c
│ ├── test_uart_8_1.c
│ └── test_uart.c
└── README.md

yaml
複製程式碼

---

## ⚙️ Features

- **Kernel Space**
  - IRS-90 HAL and driver
  - LED HAL and driver
  - Register-level control based on BCM2711 datasheet
- **User Space**
  - Sensor test applications
  - LED + IRS-90 controller
  - UART communication tests
  - MQTT publisher for IRS-90 sensor data
- **Automation**
  - Scripts to load/unload kernel modules
  - Optional `systemd` service for auto-loading on boot

---

## 🔨 Build & Install

### 1. Build Kernel Modules
```bash
cd kernel_space/irs_90
make
cd ../led
make
The .ko files will be generated in the project folder.

2. Build User Applications
bash
複製程式碼
cd user_space
make
3. Run Test Programs
bash
複製程式碼
cd user_space/sensor
sudo ./test_irs90_all
🚀 Auto-Load Modules at Boot
We use a systemd service to run device_load.sh automatically when Raspberry Pi boots.

Create Service File
bash
複製程式碼
sudo nano /etc/systemd/system/rpi_modules.service
Paste the following:

ini
複製程式碼
[Unit]
Description=Load custom IRS-90 and LED modules at boot
After=multi-user.target

[Service]
Type=oneshot
ExecStart=/root/rpi_project/kernel_space/script/device_load.sh
RemainAfterExit=yes

[Install]
WantedBy=multi-user.target
Enable & Start Service
bash
複製程式碼
sudo systemctl daemon-reload
sudo systemctl enable rpi_modules.service
sudo systemctl start rpi_modules.service
Check status:

bash
複製程式碼
systemctl status rpi_modules.service
📝 Notes
Tested on Raspberry Pi 4 (BCM2711)

Kernel modules must be rebuilt if kernel version changes

Requires make, gcc, and kernel headers installed

📌 Future Work
Add more sensor drivers (e.g., ultrasonic, servo control)

Expand MQTT integration for IoT applications

Develop web dashboard for sensor/LED monitoring
