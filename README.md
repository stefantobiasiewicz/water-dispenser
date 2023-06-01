### List of characteristics

Service "water-dispenser":
- UUID: `12345678-1234-5678-1234-56789abcdef1`

1. Characteristic "impulsiset":
   - UUID: `12345678-1234-5678-1234-56789abcdef2`
   - Type: Read/Write
   - Description: Allows setting the impulse counter. The counter is reset upon device restart. User-set impulse count. Value ranges from 0 to 65535 (0x0000 - 0xFFFF) [2 bytes in Little Endian format].

2. Characteristic "run":
   - UUID: `12345678-1234-5678-1234-56789abcdef3`
   - Type: Write
   - Description: Controls the system startup. Possible values: 0 (off, 0x00), 1 (on, 0x01) [1 byte].

3. Characteristic "identify":
   - UUID: `12345678-1234-5678-1234-56789abcdef4`
   - Type: Read/Write
   - Description: Controls the LED blinking on the device. Possible values: 0 (off, 0x00), 1 (on, 0x01) [1 byte].

4. Characteristic "on/off":
   - UUID: `12345678-1234-5678-1234-56789abcdef5`
   - Type: Write
   - Description: Controls the pump on/off state. Possible values: 0 (off, 0x00), 1 (on, 0x01) [1 byte].
