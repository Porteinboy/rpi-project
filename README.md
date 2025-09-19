# RPi Project – Sensor & UART Control System

## Overview
This project implements a Raspberry Pi 4–based embedded control system that integrates:
- **IRS-90 infrared line-tracking sensors**
- **LED indicator modules**
- **UART communication**
- **User-space applications** (MQTT publisher, test programs)

The system is structured in two layers:
1. **Kernel space drivers** (.ko modules)  
2. **User space applications** (C programs, MQTT clients)

This ensures clear separation of hardware abstraction and application logic.

---

## Directory Structure
rpi_project/
├── bin/ # User-space executables
├── build/ # Build artifacts and object files
├── docs/ # Documentation (design, reports, slides)
├── include/ # Shared header files
├── kernel/ # Kernel-space driver sources
├── modules/ # Compiled kernel modules (.ko)
├── src/ # User-space source codes
└── README.md

---

## Build Instructions

### Kernel Modules
```bash
cd kernel/irs_90
make
cd ../led
make
cd ../uart
make
# install .ko modules
sudo insmod ../../modules/irs_90_driver.ko
sudo insmod ../../modules/led_driver.ko
sudo insmod ../../modules/uart.ko```

User-Space Applications
```bash
cd src/uart
make
cd ../mqtt
make```

Binaries will be placed in bin/.

Usage

Sensor test:
```bash
./bin/test_irs90_all```


UART test:
```bash
./bin/irs90_uart```


MQTT publisher:
```bash
./bin/irs90_mqtt_publisher```
