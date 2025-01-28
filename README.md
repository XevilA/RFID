# RFID Access Control System Setup Guide

## Hardware Requirements

- NodeMCU (ESP8266)

- RFID-RC522 Module

- Solenoid Lock

- Relay Module

- Buzzer

- Jumper wires

- Breadboard or soldering tools (optional)

## Hardware Connections

### Pin Connections

| **RFID-RC522 Module** | **NodeMCU (ESP8266)** |

|------------------------|------------------------|

| SDA                   | D4                     |

| SCK                   | D5                     |

| MOSI                  | D7                     |

| MISO                  | D6                     |

| IRQ                   | Not Connected          |

| GND                   | GND                    |

| RST                   | D3                     |

| 3.3V                  | 3.3V                   |

| **Relay Module** | **NodeMCU (ESP8266)** |

|-------------------|------------------------|

| IN                | D1                     |

| GND               | GND                    |

| VCC               | 3.3V                   |

| **Buzzer**  | **NodeMCU (ESP8266)** |

|-------------|------------------------|

| Positive (+)| D2                     |

| Negative (-)| GND                    |

### Solenoid Lock Connection

Connect the solenoid lock to the relay module as follows:

1\. One wire of the solenoid lock to the COM terminal of the relay.

2\. The other wire of the solenoid lock to the external power supply (+).

3\. Connect the GND of the power supply to the GND terminal of the relay.

4\. The NO (Normally Open) terminal of the relay to the GND of the power supply.

## Software Setup

### 1. Install Required Libraries

Ensure you have the following libraries installed in the Arduino IDE:

- [MFRC522 Library](https://github.com/miguelbalboa/rfid)

- ESP8266 Core for Arduino: Install via Arduino IDE board manager.

### 2. Configure the Code

1\. Open the provided `RFID_Access_Control.ino` file.

2\. Update the following variables in the code:

   ```cpp

   const char *ssid = "your_wifi_name"; // Replace with your WiFi name

   const char *password = "your_wifi_password"; // Replace with your WiFi password

   const char *lineToken = "your_line_notify_token"; // Replace with your Line Notify token

   byte allowedUID[4] = {0xDE, 0xAD, 0xBE, 0xEF}; // Replace with the UID of your RFID card

   ```

### 3. Upload the Code

1\. Connect the NodeMCU to your computer via a USB cable.

2\. In Arduino IDE:

   - Select the correct **Board**: `NodeMCU 1.0 (ESP-12E Module)`

   - Select the correct **Port**

3\. Click on **Upload** to flash the code to the NodeMCU.

### 4. Test the System

1\. Power on the NodeMCU and ensure it connects to WiFi (check the Serial Monitor for confirmation).

2\. Tap an RFID card to the RFID reader:

   - If the UID matches the `allowedUID`, the relay will activate, unlocking the solenoid lock, and the buzzer will beep.

   - A message will be sent to your Line Notify.

   - If the UID does not match, the buzzer will emit a "denied" sound.

## Troubleshooting

1\. **WiFi not connecting**:

   - Check your WiFi credentials in the code.

   - Ensure your WiFi network is 2.4GHz (ESP8266 does not support 5GHz).

2\. **RFID not reading**:

   - Double-check the wiring between the RFID-RC522 and the NodeMCU.

   - Ensure the RFID card is compatible with the module (13.56MHz).

3\. **Line Notify not working**:

   - Verify the Line Notify token.

   - Check the Serial Monitor for connection errors.

## Additional Notes

- Use a stable power supply for the solenoid lock (e.g., 12V, depending on your lock).

- Place the RFID module in an easily accessible location for convenient scanning.

- Ensure proper insulation of wires to avoid short circuits.
