#ifndef DIYables_TFT_Shield_Mega_H
#define DIYables_TFT_Shield_Mega_H

#include <Arduino.h>
#include <Adafruit_GFX.h>

// ============================================================================
// 16-bit parallel data bus pin mapping (Mega form factor shield)
// DB0-DB7  (low byte)  → Arduino pins 37,36,35,34,33,32,31,30 (PORTC on Mega)
// DB8-DB15 (high byte) → Arduino pins 22,23,24,25,26,27,28,29 (PORTA on Mega)
// ============================================================================
#define D0_PIN  37
#define D1_PIN  36
#define D2_PIN  35
#define D3_PIN  34
#define D4_PIN  33
#define D5_PIN  32
#define D6_PIN  31
#define D7_PIN  30
#define D8_PIN  22
#define D9_PIN  23
#define D10_PIN 24
#define D11_PIN 25
#define D12_PIN 26
#define D13_PIN 27
#define D14_PIN 28
#define D15_PIN 29

// Control pins (Mega form factor shield)
#define API_PIN_CS     40
#define API_PIN_CD     38   // RS (Register Select)
#define API_PIN_WR     39
#define API_PIN_RD     44
#define API_PIN_RESET  41

// ============================================================================
// Board-specific macros for 16-bit parallel interface
// ============================================================================

#if defined(__AVR_ATmega2560__) || defined(__AVR_ATmega1280__)
// Arduino Mega 2560 / Mega 1280
// Data bus: PORTA = DB[15:8] (pins 22-29), PORTC = DB[7:0] (pins 37-30)
// Control: CS=pin40(PG1), CD=pin38(PD7), WR=pin39(PG2), RD=pin44(PL5), RST=pin41(PG0)
#define RD_PORT PORTL
#define RD_PIN  5       // PL5, pin 44
#define WR_PORT PORTG
#define WR_PIN  2       // PG2, pin 39
#define CD_PORT PORTD
#define CD_PIN  7       // PD7, pin 38
#define CS_PORT PORTG
#define CS_PIN  1       // PG1, pin 40
#define RESET_PORT PORTG
#define RESET_PIN  0    // PG0, pin 41

// Write 16-bit value: high byte → PORTA, low byte → PORTC
#define WRITE_16(x) do { \
  PORTA = (uint8_t)((x) >> 8); \
  PORTC = (uint8_t)((x) & 0xFF); \
} while(0)

// Write 8-bit value: only low byte → PORTC (for commands/parameters)
#define WRITE_8(x) do { \
  PORTA = 0; \
  PORTC = (uint8_t)(x); \
} while(0)

#define SET_DATA_DIR_OUT() do { DDRA = 0xFF; DDRC = 0xFF; } while(0)
#define SET_CONTROL_DIR_OUT() do { \
  DDRL |= (1 << 5); \
  DDRG |= (1 << 2) | (1 << 1) | (1 << 0); \
  DDRD |= (1 << 7); \
} while(0)
#define PIN_LOW(p, b)  (p) &= ~(1 << (b))
#define PIN_HIGH(p, b) (p) |= (1 << (b))
#define PIN_OUTPUT(p, b) *(&p - 1) |= (1 << (b))
#define WR_STROBE do { PIN_LOW(WR_PORT, WR_PIN); asm volatile("nop"); PIN_HIGH(WR_PORT, WR_PIN); } while(0)

#elif defined(__SAM3X8E__)
// Arduino Due (SAM3X8E) — Mega form factor
// Data bus (16-bit) mapped to pins 22-29 (DB8-15) and 30-37 (DB0-7):
//   DB0=pin37→PC5, DB1=pin36→PC4, DB2=pin35→PC3, DB3=pin34→PC2, DB4=pin33→PC1
//   DB5=pin32→PD10, DB6=pin31→PA7, DB7=pin30→PD9
//   DB8=pin22→PB26, DB9=pin23→PA14, DB10=pin24→PA15
//   DB11=pin25→PD0, DB12=pin26→PD1, DB13=pin27→PD2, DB14=pin28→PD3, DB15=pin29→PD6
// Control: CS=pin40→PC8, CD=pin38→PC6, WR=pin39→PC7, RD=pin44→PC19, RST=pin41→PC9

#define RD_PORT    PIOC
#define RD_PIN     19      // PC19, pin 44
#define WR_PORT    PIOC
#define WR_PIN     7       // PC7,  pin 39
#define CD_PORT    PIOC
#define CD_PIN     6       // PC6,  pin 38
#define CS_PORT    PIOC
#define CS_PIN     8       // PC8,  pin 40
#define RESET_PORT PIOC
#define RESET_PIN  9       // PC9,  pin 41

// Data port masks
#define DUE_AMASK ((1UL<<7)|(1UL<<14)|(1UL<<15))
#define DUE_BMASK (1UL<<26)
#define DUE_CMASK ((1UL<<1)|(1UL<<2)|(1UL<<3)|(1UL<<4)|(1UL<<5))
#define DUE_DMASK ((1UL<<0)|(1UL<<1)|(1UL<<2)|(1UL<<3)|(1UL<<6)|(1UL<<9)|(1UL<<10))

// Write 16-bit value to data bus
#define WRITE_16(x) do { \
  PIOA->PIO_CODR = DUE_AMASK; \
  PIOB->PIO_CODR = DUE_BMASK; \
  PIOC->PIO_CODR = DUE_CMASK; \
  PIOD->PIO_CODR = DUE_DMASK; \
  PIOC->PIO_SODR = \
    (((x) & (1<<0)) << 5) | \
    (((x) & (1<<1)) << 3) | \
    (((x) & (1<<2)) << 1) | \
    (((x) & (1<<3)) >> 1) | \
    (((x) & (1<<4)) >> 3); \
  PIOD->PIO_SODR = \
    (((x) & (1<<5)) << 5) | \
    (((x) & (1<<7)) << 2) | \
    (((x) & (0xF << 11)) >> 11) | \
    (((x) & (1<<15)) >> 9); \
  PIOA->PIO_SODR = \
    (((x) & (1<<6)) << 1) | \
    (((x) & (3<<9)) << 5); \
  PIOB->PIO_SODR = (((x) & (1<<8)) << 18); \
} while(0)

// Write 8-bit value (command/parameter, only low byte matters)
#define WRITE_8(x) do { \
  PIOA->PIO_CODR = DUE_AMASK; \
  PIOB->PIO_CODR = DUE_BMASK; \
  PIOC->PIO_CODR = DUE_CMASK; \
  PIOD->PIO_CODR = DUE_DMASK; \
  PIOC->PIO_SODR = \
    (((x) & (1<<0)) << 5) | \
    (((x) & (1<<1)) << 3) | \
    (((x) & (1<<2)) << 1) | \
    (((x) & (1<<3)) >> 1) | \
    (((x) & (1<<4)) >> 3); \
  PIOD->PIO_SODR = \
    (((x) & (1<<5)) << 5) | \
    (((x) & (1<<7)) << 2); \
  PIOA->PIO_SODR = (((x) & (1<<6)) << 1); \
} while(0)

#define SET_DATA_DIR_OUT() do { \
  PIOA->PIO_OER = DUE_AMASK; PIOA->PIO_PER = DUE_AMASK; \
  PIOB->PIO_OER = DUE_BMASK; PIOB->PIO_PER = DUE_BMASK; \
  PIOC->PIO_OER = DUE_CMASK; PIOC->PIO_PER = DUE_CMASK; \
  PIOD->PIO_OER = DUE_DMASK; PIOD->PIO_PER = DUE_DMASK; \
} while(0)
#define SET_CONTROL_DIR_OUT() do { \
  PIOC->PIO_OER = (1UL<<6)|(1UL<<7)|(1UL<<8)|(1UL<<9)|(1UL<<19); \
  PIOC->PIO_PER = (1UL<<6)|(1UL<<7)|(1UL<<8)|(1UL<<9)|(1UL<<19); \
} while(0)
#define PIN_LOW(p, b)  (p)->PIO_CODR = (1UL << (b))
#define PIN_HIGH(p, b) (p)->PIO_SODR = (1UL << (b))
#define PIN_OUTPUT(p, b) (p)->PIO_OER = (1UL << (b))
#define WR_STROBE do { PIN_LOW(WR_PORT, WR_PIN); PIN_HIGH(WR_PORT, WR_PIN); } while(0)

#elif defined(ARDUINO_GIGA)
// Arduino Giga R1 WiFi (STM32H747XI) — Mega form factor
// Direct register access for 16-bit parallel data bus
//
// Data bus mapping (from GIGA variant.cpp):
//   DB0=pin37→PJ6,  DB1=pin36→PK6,  DB2=pin35→PJ5,  DB3=pin34→PK5
//   DB4=pin33→PJ4,  DB5=pin32→PK4,  DB6=pin31→PJ3,  DB7=pin30→PK3
//   DB8=pin22→PJ12, DB9=pin23→PG13, DB10=pin24→PG12, DB11=pin25→PJ0
//   DB12=pin26→PJ14,DB13=pin27→PJ1, DB14=pin28→PJ15, DB15=pin29→PJ2
// Control:
//   CS=pin40→PE6, CD=pin38→PJ7, WR=pin39→PI14, RD=pin44→PG10, RST=pin41→PK7

// Control pin port/bit definitions
#define RD_PORT    GPIOG
#define RD_PIN     10      // PG10, pin 44
#define WR_PORT    GPIOI
#define WR_PIN     14      // PI14, pin 39
#define CD_PORT    GPIOJ
#define CD_PIN     7       // PJ7, pin 38
#define CS_PORT    GPIOE
#define CS_PIN     6       // PE6, pin 40
#define RESET_PORT GPIOK
#define RESET_PIN  7       // PK7, pin 41

// Data bus pin masks per GPIO port (GPIOJ: 10 pins, GPIOK: 4 pins, GPIOG: 2 pins)
#define GIGA_JMASK ((1UL<<0)|(1UL<<1)|(1UL<<2)|(1UL<<3)|(1UL<<4)|(1UL<<5)|(1UL<<6)|(1UL<<12)|(1UL<<14)|(1UL<<15))
#define GIGA_KMASK ((1UL<<3)|(1UL<<4)|(1UL<<5)|(1UL<<6))
#define GIGA_GMASK ((1UL<<12)|(1UL<<13))

// Write 16-bit value to data bus using BSRR (atomic set/clear in one write per port)
#define WRITE_16(x) do { \
  uint32_t _jv = \
    (((x) & (1<<0)) << 6)   | (((x) & (1<<2)) << 3)   | \
    ((x) & (1<<4))           | (((x) & (1<<6)) >> 3)   | \
    (((x) & (1<<8)) << 4)   | (((x) & (1<<11)) >> 11)  | \
    (((x) & (1<<12)) << 2)  | (((x) & (1<<13)) >> 12)  | \
    (((x) & (1<<14)) << 1)  | (((x) & (1<<15)) >> 13); \
  uint32_t _kv = \
    (((x) & (1<<1)) << 5)   | (((x) & (1<<3)) << 2)   | \
    (((x) & (1<<5)) >> 1)   | (((x) & (1<<7)) >> 4); \
  uint32_t _gv = \
    (((x) & (1<<9)) << 4)   | (((x) & (1<<10)) << 2); \
  GPIOJ->BSRR = _jv | ((GIGA_JMASK & ~_jv) << 16); \
  GPIOK->BSRR = _kv | ((GIGA_KMASK & ~_kv) << 16); \
  GPIOG->BSRR = _gv | ((GIGA_GMASK & ~_gv) << 16); \
} while(0)

// Write 8-bit value to data bus (command/parameter, only low byte; high byte cleared)
#define WRITE_8(x) do { \
  uint32_t _jv = \
    (((x) & (1<<0)) << 6) | (((x) & (1<<2)) << 3) | \
    ((x) & (1<<4))         | (((x) & (1<<6)) >> 3); \
  uint32_t _kv = \
    (((x) & (1<<1)) << 5) | (((x) & (1<<3)) << 2) | \
    (((x) & (1<<5)) >> 1) | (((x) & (1<<7)) >> 4); \
  GPIOJ->BSRR = _jv | ((GIGA_JMASK & ~_jv) << 16); \
  GPIOK->BSRR = _kv | ((GIGA_KMASK & ~_kv) << 16); \
  GPIOG->BSRR = GIGA_GMASK << 16; \
} while(0)

#define SET_DATA_DIR_OUT() do { \
  uint32_t _m; \
  /* Enable GPIO clocks for ports E, G, I, J, K */ \
  RCC->AHB4ENR |= (1UL<<4)|(1UL<<6)|(1UL<<8)|(1UL<<9)|(1UL<<10); \
  _m = RCC->AHB4ENR; (void)_m; /* read-back delay for clock stabilization */ \
  /* GPIOJ: pins 0,1,2,3,4,5,6,12,14,15 as output (MODER=01) */ \
  _m = GPIOJ->MODER; _m &= ~0xF3003FFFUL; _m |= 0x51001555UL; GPIOJ->MODER = _m; \
  /* GPIOK: pins 3,4,5,6 as output */ \
  _m = GPIOK->MODER; _m &= ~0x00003FC0UL; _m |= 0x00001540UL; GPIOK->MODER = _m; \
  /* GPIOG: pins 12,13 as output */ \
  _m = GPIOG->MODER; _m &= ~0x0F000000UL; _m |= 0x05000000UL; GPIOG->MODER = _m; \
  /* Set data pins to very high speed (OSPEEDR=11) */ \
  GPIOJ->OSPEEDR |= 0xF3003FFFUL; \
  GPIOK->OSPEEDR |= 0x00003FC0UL; \
  GPIOG->OSPEEDR |= 0x0F000000UL; \
} while(0)

#define SET_CONTROL_DIR_OUT() do { \
  uint32_t _m; \
  /* CD=PJ7 as output */ \
  _m = GPIOJ->MODER; _m &= ~(3UL << 14); _m |= (1UL << 14); GPIOJ->MODER = _m; \
  /* WR=PI14 as output, very high speed */ \
  _m = GPIOI->MODER; _m &= ~(3UL << 28); _m |= (1UL << 28); GPIOI->MODER = _m; \
  GPIOI->OSPEEDR |= (3UL << 28); \
  /* CS=PE6 as output */ \
  _m = GPIOE->MODER; _m &= ~(3UL << 12); _m |= (1UL << 12); GPIOE->MODER = _m; \
  /* RD=PG10 as output */ \
  _m = GPIOG->MODER; _m &= ~(3UL << 20); _m |= (1UL << 20); GPIOG->MODER = _m; \
  /* RST=PK7 as output */ \
  _m = GPIOK->MODER; _m &= ~(3UL << 14); _m |= (1UL << 14); GPIOK->MODER = _m; \
} while(0)

#define PIN_LOW(p, b)  (p)->BSRR = (1UL << ((b) + 16))
#define PIN_HIGH(p, b) (p)->BSRR = (1UL << (b))
#define PIN_OUTPUT(p, b) do { \
  uint32_t _pos = (b) * 2; \
  (p)->MODER = ((p)->MODER & ~(3UL << _pos)) | (1UL << _pos); \
} while(0)
#define WR_STROBE do { PIN_LOW(WR_PORT, WR_PIN); __asm__ volatile("nop"); PIN_HIGH(WR_PORT, WR_PIN); } while(0)

#else
// Fallback for other boards with Mega form factor — uses Arduino API (digitalWrite)
#define ARDUINO_API_USED

#define WRITE_16(x) do {} while(0) // placeholder, not used in API mode
#define WRITE_8(x)  do {} while(0) // placeholder, not used in API mode
#define SET_DATA_DIR_OUT() do {} while(0)
#define SET_CONTROL_DIR_OUT() do {} while(0)
#define PIN_LOW(p, b)  do {} while(0)
#define PIN_HIGH(p, b) do {} while(0)
#define PIN_OUTPUT(p, b) do {} while(0)
#define WR_STROBE do {} while(0)

#endif

class DIYables_TFT_ILI9486_Shield_Mega : public Adafruit_GFX {
public:
  DIYables_TFT_ILI9486_Shield_Mega();
  DIYables_TFT_ILI9486_Shield_Mega(uint8_t d0, uint8_t d1, uint8_t d2, uint8_t d3,
                               uint8_t d4, uint8_t d5, uint8_t d6, uint8_t d7,
                               uint8_t d8, uint8_t d9, uint8_t d10, uint8_t d11,
                               uint8_t d12, uint8_t d13, uint8_t d14, uint8_t d15,
                               uint8_t rd, uint8_t wr, uint8_t cd, uint8_t cs, uint8_t rst);
  void begin();
  void fillScreen(uint16_t color) override;
  void fillRect(int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color) override;
  void drawFastHLine(int16_t x, int16_t y, int16_t w, uint16_t color) override;
  void drawFastVLine(int16_t x, int16_t y, int16_t h, uint16_t color) override;
  void setRotation(uint8_t r) override;
  void invertDisplay(bool i) override;
  void drawRGBBitmap(int16_t x, int16_t y, const uint16_t bitmap[], int16_t w, int16_t h);
  void drawRGBBitmap(int16_t x, int16_t y, uint16_t *bitmap, int16_t w, int16_t h);
  void pushColors(uint16_t *data, uint32_t len);
  void setAddrWindow(int16_t x0, int16_t y0, int16_t x1, int16_t y1);

  /**
   * Convert 8-bit R,G,B to 16-bit 565 color format.
   */
  static uint16_t colorRGB(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
  }

  void drawPixel(int16_t x, int16_t y, uint16_t color) override;

protected:
  uint8_t _d[16];
  uint8_t _rd, _wr, _cd, _cs, _rst;
  bool _useAPI;

private:
  inline void reset();
  inline void writeCommand(uint8_t cmd);
  inline void writeData(uint8_t data);
  inline void writeData16_val(uint16_t val);
  inline void write16(uint16_t val);
  inline void write8(uint8_t val);
  inline void writeBus16(uint16_t val);
  inline void writeBus8(uint8_t val);
  inline void pulseWR();
  void setWriteDir();
  void writeData16(uint16_t data, uint32_t count);
};

// Short aliases
using DIYables_TFT_Shield_Mega = DIYables_TFT_ILI9486_Shield_Mega;
using DIYables_TFT = DIYables_TFT_ILI9486_Shield_Mega;

#endif // DIYables_TFT_Shield_Mega_H