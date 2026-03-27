#include <DIYables_TFT_Shield_Mega.h>

DIYables_TFT_ILI9486_Shield_Mega::DIYables_TFT_ILI9486_Shield_Mega()
  : Adafruit_GFX(320, 480) {
  _d[0]  = D0_PIN;  _d[1]  = D1_PIN;  _d[2]  = D2_PIN;  _d[3]  = D3_PIN;
  _d[4]  = D4_PIN;  _d[5]  = D5_PIN;  _d[6]  = D6_PIN;  _d[7]  = D7_PIN;
  _d[8]  = D8_PIN;  _d[9]  = D9_PIN;  _d[10] = D10_PIN; _d[11] = D11_PIN;
  _d[12] = D12_PIN; _d[13] = D13_PIN; _d[14] = D14_PIN; _d[15] = D15_PIN;
  _rd = API_PIN_RD; _wr = API_PIN_WR; _cd = API_PIN_CD;
  _cs = API_PIN_CS; _rst = API_PIN_RESET;
  _useAPI = false;
}

DIYables_TFT_ILI9486_Shield_Mega::DIYables_TFT_ILI9486_Shield_Mega(
  uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
  uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
  uint8_t rd, uint8_t wr, uint8_t cd, uint8_t cs, uint8_t rst)
  : Adafruit_GFX(320, 480) {
  // 8-pin constructor: maps to low byte of 16-bit bus, high byte pins default
  _d[0] = d0; _d[1] = d1; _d[2] = d2; _d[3] = d3;
  _d[4] = d4; _d[5] = d5; _d[6] = d6; _d[7] = d7;
  _d[8]  = D8_PIN;  _d[9]  = D9_PIN;  _d[10] = D10_PIN; _d[11] = D11_PIN;
  _d[12] = D12_PIN; _d[13] = D13_PIN; _d[14] = D14_PIN; _d[15] = D15_PIN;
  _rd = rd; _wr = wr; _cd = cd; _cs = cs; _rst = rst;
  _useAPI = true;
}

// Write 16-bit value to the data bus (for pixel data)
inline void DIYables_TFT_ILI9486_Shield_Mega::writeBus16(uint16_t val) {
  #ifndef ARDUINO_API_USED
  if (!_useAPI) {
    WRITE_16(val);
  } else
  #endif
  {
    for (uint8_t i = 0; i < 16; i++) {
      digitalWrite(_d[i], (val >> i) & 0x01);
    }
  }
}

// Write 8-bit value to the data bus (for commands/parameters)
inline void DIYables_TFT_ILI9486_Shield_Mega::writeBus8(uint8_t val) {
  #ifndef ARDUINO_API_USED
  if (!_useAPI) {
    WRITE_8(val);
  } else
  #endif
  {
    // Low byte on DB[7:0], high byte = 0 (don't care for cmd/param)
    for (uint8_t i = 0; i < 8; i++) {
      digitalWrite(_d[i], (val >> i) & 0x01);
    }
    for (uint8_t i = 8; i < 16; i++) {
      digitalWrite(_d[i], LOW);
    }
  }
}

void DIYables_TFT_ILI9486_Shield_Mega::begin() {
  #ifndef ARDUINO_API_USED
  if (!_useAPI) {
    SET_DATA_DIR_OUT();
    SET_CONTROL_DIR_OUT();
  } else
  #endif
  {
    for (uint8_t i = 0; i < 16; i++) { pinMode(_d[i], OUTPUT); }
    pinMode(_rd, OUTPUT);
    pinMode(_wr, OUTPUT);
    pinMode(_cd, OUTPUT);
    pinMode(_cs, OUTPUT);
    pinMode(_rst, OUTPUT);
  }

  #ifndef ARDUINO_API_USED
  if (!_useAPI) {
    PIN_HIGH(RD_PORT, RD_PIN);
  } else
  #endif
  {
    digitalWrite(_rd, HIGH);
  }
  
  
  #ifndef ARDUINO_API_USED
  if (!_useAPI) {
    PIN_LOW(CS_PORT, CS_PIN);
  } else
  #endif
  {
    digitalWrite(_cs, LOW);
  }

  reset();

  writeCommand(0x11); // Sleep out
  delay(120);
  writeCommand(0xD0); // Power Setting
  writeData(0x07);
  writeData(0x42);
  writeData(0x18);
  writeCommand(0xD1); // VCOM Control
  writeData(0x00);
  writeData(0x07);
  writeData(0x10);
  writeCommand(0x36); // Memory Access Control
  writeData(0x48);    // MX, BGR
  writeCommand(0x3A); // Interface Pixel Format
  writeData(0x55);    // 16-bit/pixel
  writeCommand(0xC5); // Frame Rate Control
  writeData(0x10);
  writeCommand(0xC8); // Gamma Adjustment
  writeData(0x00); writeData(0x32); writeData(0x36);
  writeData(0x45); writeData(0x06); writeData(0x16);
  writeData(0x37); writeData(0x75); writeData(0x77);
  writeData(0x54); writeData(0x0C); writeData(0x00);
  writeCommand(0x13); // Normal Display Mode
  writeCommand(0x29); // Display ON
  delay(50);
}

void DIYables_TFT_ILI9486_Shield_Mega::fillScreen(uint16_t color) {
  setAddrWindow(0, 0, width() - 1, height() - 1);
  writeData16(color, (uint32_t)width() * height());
}

inline void DIYables_TFT_ILI9486_Shield_Mega::reset() {
  #ifndef ARDUINO_API_USED
  if (!_useAPI) {
    PIN_LOW(RESET_PORT, RESET_PIN);
  } else
  #endif
  {
    digitalWrite(_rst, LOW);
  }

  delay(20);
  #ifndef ARDUINO_API_USED
  if (!_useAPI) {
    PIN_HIGH(RESET_PORT, RESET_PIN);
  } else
  #endif
  {
    digitalWrite(_rst, HIGH);
  }
  
  delay(120);
}

void DIYables_TFT_ILI9486_Shield_Mega::setWriteDir() {
  #ifndef ARDUINO_API_USED
  if (!_useAPI) {
    SET_DATA_DIR_OUT();
  } else
  #endif
  {
    for (uint8_t i = 0; i < 16; i++) { pinMode(_d[i], OUTPUT); }
  }
}

// Write a 16-bit value to the bus and pulse WR (for pixel data and 16-bit address values)
inline void DIYables_TFT_ILI9486_Shield_Mega::write16(uint16_t val) {
  writeBus16(val);
  #ifndef ARDUINO_API_USED
  if (!_useAPI) {
    WR_STROBE;
  } else
  #endif
  {
    digitalWrite(_wr, LOW);
    digitalWrite(_wr, HIGH);
  }
}

// Write an 8-bit value to the bus and pulse WR (for commands and parameters)
inline void DIYables_TFT_ILI9486_Shield_Mega::write8(uint8_t val) {
  writeBus8(val);
  #ifndef ARDUINO_API_USED
  if (!_useAPI) {
    WR_STROBE;
  } else
  #endif
  {
    digitalWrite(_wr, LOW);
    digitalWrite(_wr, HIGH);
  }
}

inline void DIYables_TFT_ILI9486_Shield_Mega::pulseWR() {
  #ifndef ARDUINO_API_USED
  if (!_useAPI) {
    WR_STROBE;
  } else
  #endif
  {
    digitalWrite(_wr, LOW);
    __asm__ volatile ("nop");
    digitalWrite(_wr, HIGH);
  }
}

inline void DIYables_TFT_ILI9486_Shield_Mega::writeCommand(uint8_t cmd) {
  #ifndef ARDUINO_API_USED
  if (!_useAPI) {
    PIN_LOW(CD_PORT, CD_PIN);
  } else
  #endif
  {
    digitalWrite(_cd, LOW);
  }
  write8(cmd);
}

inline void DIYables_TFT_ILI9486_Shield_Mega::writeData(uint8_t data) {
  #ifndef ARDUINO_API_USED
  if (!_useAPI) {
    PIN_HIGH(CD_PORT, CD_PIN);
  } else
  #endif
  {
    digitalWrite(_cd, HIGH);
  }
  write8(data);
}

// Write a 16-bit data value (for pixel data or address words in 16-bit mode)
inline void DIYables_TFT_ILI9486_Shield_Mega::writeData16_val(uint16_t val) {
  #ifndef ARDUINO_API_USED
  if (!_useAPI) {
    PIN_HIGH(CD_PORT, CD_PIN);
  } else
  #endif
  {
    digitalWrite(_cd, HIGH);
  }
  write16(val);
}

// Write 16-bit pixel color data, repeated 'count' times.
// In 16-bit parallel mode, each pixel needs only ONE WR cycle (vs two in 8-bit mode).
void DIYables_TFT_ILI9486_Shield_Mega::writeData16(uint16_t data, uint32_t count) {
  #ifndef ARDUINO_API_USED
  if (!_useAPI) {
    PIN_HIGH(CD_PORT, CD_PIN);
    // Put the 16-bit color on the bus once, then just pulse WR for each pixel
    WRITE_16(data);
    while (count >= 16) {
      WR_STROBE; WR_STROBE; WR_STROBE; WR_STROBE;
      WR_STROBE; WR_STROBE; WR_STROBE; WR_STROBE;
      WR_STROBE; WR_STROBE; WR_STROBE; WR_STROBE;
      WR_STROBE; WR_STROBE; WR_STROBE; WR_STROBE;
      count -= 16;
    }
    while (count--) {
      WR_STROBE;
    }
    return;
  }
  #endif

  // API path: use writeBus16 + pulseWR for each pixel
  digitalWrite(_cd, HIGH);
  writeBus16(data);
  while (count >= 16) {
    pulseWR(); pulseWR(); pulseWR(); pulseWR();
    pulseWR(); pulseWR(); pulseWR(); pulseWR();
    pulseWR(); pulseWR(); pulseWR(); pulseWR();
    pulseWR(); pulseWR(); pulseWR(); pulseWR();
    count -= 16;
  }
  while (count--) {
    pulseWR();
  }
}

// ILI9486 address parameters are byte-oriented (D[7:0] only) even on 16-bit bus.
// Pixel data uses full 16-bit bus (D[15:0]).
void DIYables_TFT_ILI9486_Shield_Mega::setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1) {
  #ifndef ARDUINO_API_USED
  if (!_useAPI) {
    // Command 0x2A (Column Address Set)
    PIN_LOW(CD_PORT, CD_PIN);
    WRITE_8(0x2A); WR_STROBE;
    // Data: SC_H, SC_L, EC_H, EC_L (byte-oriented parameters)
    PIN_HIGH(CD_PORT, CD_PIN);
    WRITE_8(x0 >> 8);   WR_STROBE;
    WRITE_8(x0 & 0xFF); WR_STROBE;
    WRITE_8(x1 >> 8);   WR_STROBE;
    WRITE_8(x1 & 0xFF); WR_STROBE;
    // Command 0x2B (Row Address Set)
    PIN_LOW(CD_PORT, CD_PIN);
    WRITE_8(0x2B); WR_STROBE;
    // Data: SR_H, SR_L, ER_H, ER_L (byte-oriented parameters)
    PIN_HIGH(CD_PORT, CD_PIN);
    WRITE_8(y0 >> 8);   WR_STROBE;
    WRITE_8(y0 & 0xFF); WR_STROBE;
    WRITE_8(y1 >> 8);   WR_STROBE;
    WRITE_8(y1 & 0xFF); WR_STROBE;
    // Command 0x2C (Memory Write)
    PIN_LOW(CD_PORT, CD_PIN);
    WRITE_8(0x2C); WR_STROBE;
    return;
  }
  #endif

  writeCommand(0x2A);
  writeData(x0 >> 8);
  writeData(x0 & 0xFF);
  writeData(x1 >> 8);
  writeData(x1 & 0xFF);

  writeCommand(0x2B);
  writeData(y0 >> 8);
  writeData(y0 & 0xFF);
  writeData(y1 >> 8);
  writeData(y1 & 0xFF);

  writeCommand(0x2C);
}

void DIYables_TFT_ILI9486_Shield_Mega::drawPixel(int16_t x, int16_t y, uint16_t color) {
  if ((x < 0) || (x >= width()) || (y < 0) || (y >= height())) return;

  #ifndef ARDUINO_API_USED
  if (!_useAPI) {
    // Inline setAddrWindow (byte-oriented address params) + 16-bit pixel write
    PIN_LOW(CD_PORT, CD_PIN);
    WRITE_8(0x2A); WR_STROBE;
    PIN_HIGH(CD_PORT, CD_PIN);
    WRITE_8(x >> 8);   WR_STROBE;
    WRITE_8(x & 0xFF); WR_STROBE;
    WRITE_8(x >> 8);   WR_STROBE;
    WRITE_8(x & 0xFF); WR_STROBE;
    PIN_LOW(CD_PORT, CD_PIN);
    WRITE_8(0x2B); WR_STROBE;
    PIN_HIGH(CD_PORT, CD_PIN);
    WRITE_8(y >> 8);   WR_STROBE;
    WRITE_8(y & 0xFF); WR_STROBE;
    WRITE_8(y >> 8);   WR_STROBE;
    WRITE_8(y & 0xFF); WR_STROBE;
    PIN_LOW(CD_PORT, CD_PIN);
    WRITE_8(0x2C); WR_STROBE;
    // Write pixel color: single 16-bit write
    PIN_HIGH(CD_PORT, CD_PIN);
    WRITE_16(color); WR_STROBE;
    return;
  }
  #endif

  setAddrWindow(x, y, x, y);
  writeData16_val(color);
}

void DIYables_TFT_ILI9486_Shield_Mega::setRotation(uint8_t r) {
  Adafruit_GFX::setRotation(r);
  writeCommand(0x36);
  writeData(r == 0 ? 0x48 :
            r == 1 ? 0x28 :
            r == 2 ? 0x88 :
                     0xE8);
}

void DIYables_TFT_ILI9486_Shield_Mega::invertDisplay(bool i) {
  writeCommand(i ? 0x21 : 0x20);
}

void DIYables_TFT_ILI9486_Shield_Mega::fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) {
  if (x >= width() || y >= height() || w <= 0 || h <= 0) return;
  if (x < 0) { w += x; x = 0; }
  if (y < 0) { h += y; y = 0; }
  if ((x + w) > width())  w = width()  - x;
  if ((y + h) > height()) h = height() - y;

  setAddrWindow(x, y, x + w - 1, y + h - 1);
  writeData16(color, (uint32_t)w * h);
}

void DIYables_TFT_ILI9486_Shield_Mega::drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) {
  fillRect(x, y, w, 1, color);
}

void DIYables_TFT_ILI9486_Shield_Mega::drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) {
  fillRect(x, y, 1, h, color);
}

void DIYables_TFT_ILI9486_Shield_Mega::pushColors(uint16_t *data, uint32_t len) {
  #ifndef ARDUINO_API_USED
  if (!_useAPI) {
    PIN_HIGH(CD_PORT, CD_PIN);
    uint16_t c;
    while (len >= 8) {
      c = *data++; WRITE_16(c); WR_STROBE;
      c = *data++; WRITE_16(c); WR_STROBE;
      c = *data++; WRITE_16(c); WR_STROBE;
      c = *data++; WRITE_16(c); WR_STROBE;
      c = *data++; WRITE_16(c); WR_STROBE;
      c = *data++; WRITE_16(c); WR_STROBE;
      c = *data++; WRITE_16(c); WR_STROBE;
      c = *data++; WRITE_16(c); WR_STROBE;
      len -= 8;
    }
    while (len--) {
      c = *data++; WRITE_16(c); WR_STROBE;
    }
    return;
  }
  #endif

  digitalWrite(_cd, HIGH);
  while (len >= 8) {
    writeBus16(*data++); pulseWR();
    writeBus16(*data++); pulseWR();
    writeBus16(*data++); pulseWR();
    writeBus16(*data++); pulseWR();
    writeBus16(*data++); pulseWR();
    writeBus16(*data++); pulseWR();
    writeBus16(*data++); pulseWR();
    writeBus16(*data++); pulseWR();
    len -= 8;
  }
  while (len--) {
    writeBus16(*data++); pulseWR();
  }
}

void DIYables_TFT_ILI9486_Shield_Mega::drawRGBBitmap(int16_t x, int16_t y,
  const uint16_t bitmap[], int16_t w, int16_t h) {
  if (x >= width() || y >= height() || (x + w) <= 0 || (y + h) <= 0) return;

  if (x >= 0 && y >= 0 && (x + w) <= width() && (y + h) <= height()) {
    setAddrWindow(x, y, x + w - 1, y + h - 1);

    uint32_t count = (uint32_t)w * h;
    const uint16_t *ptr = bitmap;

    #ifndef ARDUINO_API_USED
    if (!_useAPI) {
      PIN_HIGH(CD_PORT, CD_PIN);
      uint16_t c;
      while (count >= 8) {
        c = pgm_read_word(ptr++); WRITE_16(c); WR_STROBE;
        c = pgm_read_word(ptr++); WRITE_16(c); WR_STROBE;
        c = pgm_read_word(ptr++); WRITE_16(c); WR_STROBE;
        c = pgm_read_word(ptr++); WRITE_16(c); WR_STROBE;
        c = pgm_read_word(ptr++); WRITE_16(c); WR_STROBE;
        c = pgm_read_word(ptr++); WRITE_16(c); WR_STROBE;
        c = pgm_read_word(ptr++); WRITE_16(c); WR_STROBE;
        c = pgm_read_word(ptr++); WRITE_16(c); WR_STROBE;
        count -= 8;
      }
      while (count--) {
        c = pgm_read_word(ptr++); WRITE_16(c); WR_STROBE;
      }
    } else
    #endif
    {
      digitalWrite(_cd, HIGH);
      while (count >= 8) {
        writeBus16(pgm_read_word(ptr++)); pulseWR();
        writeBus16(pgm_read_word(ptr++)); pulseWR();
        writeBus16(pgm_read_word(ptr++)); pulseWR();
        writeBus16(pgm_read_word(ptr++)); pulseWR();
        writeBus16(pgm_read_word(ptr++)); pulseWR();
        writeBus16(pgm_read_word(ptr++)); pulseWR();
        writeBus16(pgm_read_word(ptr++)); pulseWR();
        writeBus16(pgm_read_word(ptr++)); pulseWR();
        count -= 8;
      }
      while (count--) {
        writeBus16(pgm_read_word(ptr++)); pulseWR();
      }
    }
  } else {
    for (int16_t j = 0; j < h; j++) {
      for (int16_t i = 0; i < w; i++) {
        int16_t px = x + i, py = y + j;
        if (px >= 0 && px < width() && py >= 0 && py < height()) {
          drawPixel(px, py, pgm_read_word(&bitmap[j * w + i]));
        }
      }
    }
  }
}

void DIYables_TFT_ILI9486_Shield_Mega::drawRGBBitmap(int16_t x, int16_t y,
  uint16_t *bitmap, int16_t w, int16_t h) {
  if (x >= width() || y >= height() || (x + w) <= 0 || (y + h) <= 0) return;

  if (x >= 0 && y >= 0 && (x + w) <= width() && (y + h) <= height()) {
    setAddrWindow(x, y, x + w - 1, y + h - 1);
    pushColors(bitmap, (uint32_t)w * h);
  } else {
    for (int16_t j = 0; j < h; j++) {
      for (int16_t i = 0; i < w; i++) {
        int16_t px = x + i, py = y + j;
        if (px >= 0 && px < width() && py >= 0 && py < height()) {
          drawPixel(px, py, bitmap[j * w + i]);
        }
      }
    }
  }
}