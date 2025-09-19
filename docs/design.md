
---

### 📄 docs/design.md  

```markdown
# System Design Document – RPi Project

## 1. Introduction
The system is designed to demonstrate an embedded control platform on Raspberry Pi 4, integrating sensor drivers, LED indicators, and UART communication for automated data collection and communication.

---

## 2. System Architecture
### Layers
- **Kernel Space**
  - Hardware Abstraction Layer (HAL) for IRS-90 and LED
  - Character device drivers for /dev/irs90, /dev/led, /dev/uart
- **User Space**
  - C test programs for read/write validation
  - MQTT publisher for data communication
  - Console-based utilities for UART interaction

### Diagrm
+----------------------+
| User Applications | (MQTT, test tools)
+----------------------+
| User-Space API |
+----------------------+
| Kernel Drivers | (irs_90.ko, led.ko, uart.ko)
+----------------------+
| Hardware | (IRS-90 sensors, LEDs, UART pins)

---

## 3. Modules
- **IRS-90 Sensor Module**
  - Reads infrared line-tracking sensor values
  - Exposed via `/dev/irs90`
- **LED Control Module**
  - Simple GPIO LED control
  - Exposed via `/dev/led`
- **UART Module**
  - Provides serial communication
  - Exposed via `/dev/uart`

---

## 4. Data Flow
1. IRS-90 sensors detect line status → values passed to kernel driver.  
2. Kernel driver exposes data to `/dev/irs90`.  
3. User program reads sensor data and processes it.  
4. Processed data published to remote system via MQTT (optional).  

---

## 5. Key Design Decisions
- **Separation of HAL and Driver**: HAL encapsulates register access, driver manages kernel interface.  
- **Character Devices**: Chosen for simple user-space interaction (`open/read/write`).  
- **MQTT**: Lightweight publish-subscribe protocol suitable for IoT-style telemetry.  

---

## 6. Risks and Limitations
- Limited to Raspberry Pi 4 hardware.  
- Performance constrained by user-space polling design.  
- No DMA or advanced buffering implemented in UART driver.  

---

## 7. Future Work
- Integrate additional sensors (TCRT5000, ultrasonic).  
- Add kernel-space buffering and interrupts for UART.  
- Implement a web dashboard for real-time data visualization.  

