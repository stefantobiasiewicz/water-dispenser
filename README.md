### Lista charakterystyk

Serwis "water-dispenser":
- UUID: `12345678-1234-5678-1234-56789abcdef1`

1. Charakterystyka "impulsiset":
   - UUID: `12345678-1234-5678-1234-56789abcdef2`
   - Rodzaj: Read/Write
   - Opis: Umożliwia ustawienie licznika impulsów. Licznik jest resetowany podczas ponownego uruchomienia. Liczba impulsów ustawiona przez użytkownika. Wartość od 0 do 65535 (0x0000 - 0xFFFF) [2 bajty w formacie Little Endian].

2. Charakterystyka "run":
   - UUID: `12345678-1234-5678-1234-56789abcdef3`
   - Rodzaj: Write
   - Opis: Steruje uruchamianiem systemu. Możliwe wartości: 0 (wyłączony, 0x00), 1 (włączony, 0x01) [1 bajt].

3. Charakterystyka "identify":
   - UUID: `12345678-1234-5678-1234-56789abcdef4`
   - Rodzaj: Read/Write
   - Opis: Steruje migotaniem diody LED na urządzeniu. Możliwe wartości: 0 (wyłączony, 0x00), 1 (włączony, 0x01) [1 bajt].

4. Charakterystyka "on/off":
   - UUID: `12345678-1234-5678-1234-56789abcdef5`
   - Rodzaj: Write
   - Opis: Steruje włączaniem/wyłączaniem pompy. Możliwe wartości: 0 (wyłączona, 0x00), 1 (włączona, 0x01) [1 bajt].
