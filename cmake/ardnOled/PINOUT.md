**Pinout for oLED based Arduino device :**

| RC522 | oLED (32x128) | ESP8266 | Relay | SPK | Arduino |
|-------|---------------|---------|-------|-----|---------|
| VCC   | VCC           | VCC     | VCC   |     | 5V      |
| GND   | GND           | GND     | GND   | GND | GND     |
|       |               | TX      |       |     | D5      |
|       |               | RX      |       |     | D6      |
|       |               |         | VIN   |     | D7      |
|       |               |         |       | VCC | D8      |
| RST   |               |         |       |     | D9      |
| SDA   |               |         |       |     | D10     |
| MOSI  |               |         |       |     | D11     |
| MISO  |               |         |       |     | D12     |
| SCK   |               |         |       |     | D13     |
|       | SCL           |         |       |     | A5      |
|       | SDA           |         |       |     | A4      |
