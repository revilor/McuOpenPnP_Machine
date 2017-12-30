/* ###################################################################
**     This component module is generated by Processor Expert. Do not modify it.
**     Filename    : McuSSD1306.c
**     Project     : FRDM-K64F_Generator
**     Processor   : MK64FN1M0VLL12
**     Component   : SSD1306
**     Version     : Component 01.012, Driver 01.00, CPU db: 3.00.000
**     Repository  : Legacy User Components
**     Compiler    : GNU C Compiler
**     Date/Time   : 2017-12-30, 12:53, # CodeGen: 276
**     Abstract    :
**
Display driver for the SSD1351 (e.g. found on Hexiwear).
**     Settings    :
**          Component name                                 : McuSSD1306
**          Type                                           : 128x64
**          Orientation                                    : landscape
**          Width                                          : 128
**          Height                                         : 64
**          Bytes in rows                                  : no
**          Bytes in x direction                           : yes
**          MSB first                                      : no
**          Bits per pixel                                 : 1
**          Window capability                              : no
**          Display Memory Write                           : no
**          Display Memory Read                            : no
**          Use RAM Buffer                                 : yes
**          Clear display in init                          : no
**          Initialize on Init                             : yes
**          HW                                             : 
**            I2C Device Address                           : 0x3C
**            I2C Transaction Delay (us)                   : 100
**            I2C                                          : McuGenericI2C
**            Reset                                        : Disabled
**          System                                         : 
**            Wait                                         : McuWait
**            SDK                                          : McuLib
**     Contents    :
**         GetWidth              - McuSSD1306_PixelDim McuSSD1306_GetWidth(void);
**         GetHeight             - McuSSD1306_PixelDim McuSSD1306_GetHeight(void);
**         GetLongerSide         - McuSSD1306_PixelDim McuSSD1306_GetLongerSide(void);
**         GetShorterSide        - McuSSD1306_PixelDim McuSSD1306_GetShorterSide(void);
**         SetDisplayOrientation - void McuSSD1306_SetDisplayOrientation(McuSSD1306_DisplayOrientation...
**         GetDisplayOrientation - McuSSD1306_DisplayOrientation McuSSD1306_GetDisplayOrientation(void);
**         WriteData             - void McuSSD1306_WriteData(uint8_t data);
**         WriteDataWord         - void McuSSD1306_WriteDataWord(uint16_t data);
**         WriteDataWordRepeated - void McuSSD1306_WriteDataWordRepeated(uint16_t data, size_t nof);
**         WriteDataBlock        - void McuSSD1306_WriteDataBlock(uint8_t *data, size_t dataSize);
**         WriteCommand          - void McuSSD1306_WriteCommand(uint8_t cmd);
**         OpenWindow            - void McuSSD1306_OpenWindow(McuSSD1306_PixelDim x0, McuSSD1306_PixelDim y0,...
**         CloseWindow           - void McuSSD1306_CloseWindow(void);
**         Clear                 - void McuSSD1306_Clear(void);
**         UpdateFull            - void McuSSD1306_UpdateFull(void);
**         UpdateRegion          - void McuSSD1306_UpdateRegion(McuSSD1306_PixelDim x, McuSSD1306_PixelDim y,...
**         InitCommChannel       - void McuSSD1306_InitCommChannel(void);
**         SetContrast           - uint8_t McuSSD1306_SetContrast(uint8_t contrast);
**         DisplayOn             - uint8_t McuSSD1306_DisplayOn(bool on);
**         DisplayInvert         - uint8_t McuSSD1306_DisplayInvert(bool invert);
**         GetLCD                - void McuSSD1306_GetLCD(void);
**         GiveLCD               - void McuSSD1306_GiveLCD(void);
**         Init                  - void McuSSD1306_Init(void);
**
**     * Copyright (c) 2017, Erich Styger
**      * Web:         https://mcuoneclipse.com
**      * SourceForge: https://sourceforge.net/projects/mcuoneclipse
**      * Git:         https://github.com/ErichStyger/McuOnEclipse_PEx
**      * All rights reserved.
**      *
**      * Redistribution and use in source and binary forms, with or without modification,
**      * are permitted provided that the following conditions are met:
**      *
**      * - Redistributions of source code must retain the above copyright notice, this list
**      *   of conditions and the following disclaimer.
**      *
**      * - Redistributions in binary form must reproduce the above copyright notice, this
**      *   list of conditions and the following disclaimer in the documentation and/or
**      *   other materials provided with the distribution.
**      *
**      * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
**      * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
**      * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
**      * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
**      * ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
**      * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
**      * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
**      * ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
**      * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
**      * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
** ###################################################################*/
/*!
** @file McuSSD1306.c
** @version 01.00
** @brief
**
Display driver for the SSD1351 (e.g. found on Hexiwear).
*/         
/*!
**  @addtogroup McuSSD1306_module McuSSD1306 module documentation
**  @{
*/         

/* MODULE McuSSD1306. */

#include "McuSSD1306.h"
#include McuSSD1306_CONFIG_I2C_HEADER_FILE  /* I2C driver */

uint8_t McuSSD1306_DisplayBuf[((McuSSD1306_DISPLAY_HW_NOF_ROWS-1)/8)+1][McuSSD1306_DISPLAY_HW_NOF_COLUMNS]; /* buffer for the display */

#if McuSSD1306_CONFIG_DYNAMIC_DISPLAY_ORIENTATION
  static McuSSD1306_DisplayOrientation currentOrientation;
#endif

/** tiny 5x8 font. */
static unsigned char font[] = {
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,    // [0x20] ' '
    0x0,  0x0,  0x2F,  0x0,  0x0,  0x0,    // [0x21] '!'
    0x0,  0x3,  0x0,  0x3,  0x0,  0x0,    // [0x22] '"'
    0x14,  0x3E,  0x14,  0x3E,  0x14,  0x0,    // [0x23] '#'
    0x24,  0x2A,  0x7F,  0x2A,  0x12,  0x0,    // [0x24] '$'
    0x22,  0x10,  0x8,  0x4,  0x22,  0x0,    // [0x25] '%'
    0x18,  0x24,  0x24,  0x1E,  0x4,  0x0,    // [0x26] '&'
    0x0,  0x0,  0x3,  0x0,  0x0,  0x0,    // [0x27] '''
    0x0,  0x1C,  0x22,  0x41,  0x0,  0x0,    // [0x28] '('
    0x0,  0x41,  0x22,  0x1C,  0x0,  0x0,    // [0x29] ')'
    0x2A,  0x1C,  0x3E,  0x1C,  0x2A,  0x0,    // [0x2A] '*'
    0x8,  0x8,  0x3E,  0x8,  0x8,  0x0,    // [0x2B] '+'
    0x0,  0x40,  0x20,  0x0,  0x0,  0x0,    // [0x2C] ','
    0x8,  0x8,  0x8,  0x8,  0x8,  0x0,    // [0x2D] '-'
    0x0,  0x0,  0x20,  0x0,  0x0,  0x0,    // [0x2E] '.'
    0x0,  0xC0,  0x30,  0xC,  0x3,  0x0,    // [0x2F] '/'
    0x1E,  0x29,  0x2D,  0x25,  0x1E,  0x0,    // [0x30] '0'
    0x0,  0x22,  0x3F,  0x20,  0x0,  0x0,    // [0x31] '1'
    0x32,  0x29,  0x29,  0x29,  0x26,  0x0,    // [0x32] '2'
    0x12,  0x21,  0x29,  0x29,  0x16,  0x0,    // [0x33] '3'
    0x7,  0x8,  0x8,  0x8,  0x3E,  0x0,    // [0x34] '4'
    0x17,  0x25,  0x25,  0x25,  0x18,  0x0,    // [0x35] '5'
    0x1E,  0x25,  0x25,  0x25,  0x18,  0x0,    // [0x36] '6'
    0x1,  0x1,  0x9,  0x9,  0x3E,  0x0,    // [0x37] '7'
    0x1A,  0x25,  0x25,  0x25,  0x1A,  0x0,    // [0x38] '8'
    0x6,  0x9,  0x9,  0x9,  0x3E,  0x0,    // [0x39] '9'
    0x0,  0x0,  0x22,  0x0,  0x0,  0x0,    // [0x3A] ':'
    0x0,  0x40,  0x22,  0x0,  0x0,  0x0,    // [0x3B] ';'
    0x0,  0x8,  0x14,  0x22,  0x41,  0x0,    // [0x3C] '<'
    0x14,  0x14,  0x14,  0x14,  0x14,  0x0,    // [0x3D] '='
    0x0,  0x41,  0x22,  0x14,  0x8,  0x0,    // [0x3E] '>'
    0x2,  0x1,  0x29,  0x9,  0x6,  0x0,    // [0x3F] '?'
    0x1E,  0x21,  0x2D,  0x2D,  0x6,  0x0,    // [0x40] '@'
    0x3E,  0x11,  0x11,  0x11,  0x3E,  0x0,    // [0x41] 'A'
    0x3E,  0x25,  0x25,  0x25,  0x1A,  0x0,    // [0x42] 'B'
    0x1E,  0x21,  0x21,  0x21,  0x12,  0x0,    // [0x43] 'C'
    0x3E,  0x21,  0x21,  0x22,  0x1C,  0x0,    // [0x44] 'D'
    0x3F,  0x29,  0x29,  0x21,  0x21,  0x0,    // [0x45] 'E'
    0x3F,  0x9,  0x9,  0x1,  0x1,  0x0,    // [0x46] 'F'
    0x1E,  0x21,  0x29,  0x29,  0x1A,  0x0,    // [0x47] 'G'
    0x3F,  0x8,  0x8,  0x8,  0x3F,  0x0,    // [0x48] 'H'
    0x0,  0x21,  0x3F,  0x21,  0x0,  0x0,    // [0x49] 'I'
    0x10,  0x20,  0x21,  0x21,  0x1F,  0x0,    // [0x4A] 'J'
    0x3F,  0x8,  0xC,  0x12,  0x21,  0x0,    // [0x4B] 'K'
    0x1F,  0x20,  0x20,  0x20,  0x20,  0x0,    // [0x4C] 'L'
    0x3E,  0x1,  0x6,  0x1,  0x3E,  0x0,    // [0x4D] 'M'
    0x3E,  0x1,  0x1,  0x2,  0x3C,  0x0,    // [0x4E] 'N'
    0x1E,  0x21,  0x21,  0x21,  0x1E,  0x0,    // [0x4F] 'O'
    0x3E,  0x11,  0x11,  0x11,  0xE,  0x0,    // [0x50] 'P'
    0x1E,  0x21,  0x29,  0x71,  0x5E,  0x0,    // [0x51] 'Q'
    0x3E,  0x9,  0x9,  0x9,  0x36,  0x0,    // [0x52] 'R'
    0x12,  0x25,  0x25,  0x25,  0x18,  0x0,    // [0x53] 'S'
    0x1,  0x1,  0x3F,  0x1,  0x1,  0x0,    // [0x54] 'T'
    0x1F,  0x20,  0x20,  0x20,  0x1F,  0x0,    // [0x55] 'U'
    0xF,  0x10,  0x20,  0x10,  0xF,  0x0,    // [0x56] 'V'
    0x1F,  0x20,  0x18,  0x20,  0x1F,  0x0,    // [0x57] 'W'
    0x31,  0xA,  0x4,  0xA,  0x31,  0x0,    // [0x58] 'X'
    0x7,  0x28,  0x28,  0x28,  0x1F,  0x0,    // [0x59] 'Y'
    0x31,  0x29,  0x25,  0x23,  0x21,  0x0,    // [0x5A] 'Z'
    0x0,  0x7F,  0x41,  0x41,  0x0,  0x0,    // [0x5B] '['
    0x0,  0x3,  0xC,  0x30,  0xC0,  0x0,    // [0x5C] '\\'
    0x0,  0x41,  0x41,  0x7F,  0x0,  0x0,    // [0x5D] ']'
    0x0,  0x2,  0x1,  0x2,  0x0,  0x0,    // [0x5E] '^'
    0x40,  0x40,  0x40,  0x40,  0x40,  0x0,    // [0x5F] '_'
    0x1,  0x2,  0x0,  0x0,  0x0,  0x0,    // [0x60] '`'
    0x1C,  0x22,  0x22,  0x22,  0x3C,  0x0,    // [0x61] 'a'
    0x1F,  0x22,  0x22,  0x22,  0x1C,  0x0,    // [0x62] 'b'
    0x1C,  0x22,  0x22,  0x22,  0x20,  0x0,    // [0x63] 'c'
    0x1C,  0x22,  0x22,  0x22,  0x1F,  0x0,    // [0x64] 'd'
    0x1C,  0x2A,  0x2A,  0x2A,  0x4,  0x0,    // [0x65] 'e'
    0x8,  0x7E,  0x9,  0x1,  0x1,  0x0,    // [0x66] 'f'
    0xC,  0x52,  0x52,  0x52,  0x3C,  0x0,    // [0x67] 'g'
    0x3F,  0x2,  0x2,  0x2,  0x3C,  0x0,    // [0x68] 'h'
    0x0,  0x0,  0x3D,  0x0,  0x0,  0x0,    // [0x69] 'i'
    0x20,  0x40,  0x40,  0x40,  0x3D,  0x0,    // [0x6A] 'j'
    0x3F,  0x8,  0x8,  0x14,  0x22,  0x0,    // [0x6B] 'k'
    0x0,  0x1F,  0x20,  0x20,  0x0,  0x0,    // [0x6C] 'l'
    0x3C,  0x2,  0x4,  0x2,  0x3C,  0x0,    // [0x6D] 'm'
    0x3C,  0x2,  0x2,  0x2,  0x3C,  0x0,    // [0x6E] 'n'
    0x1C,  0x22,  0x22,  0x22,  0x1C,  0x0,    // [0x6F] 'o'
    0x7C,  0x12,  0x12,  0x12,  0xC,  0x0,    // [0x70] 'p'
    0xC,  0x12,  0x12,  0x12,  0x7E,  0x0,    // [0x71] 'q'
    0x3C,  0x2,  0x2,  0x2,  0x4,  0x0,    // [0x72] 'r'
    0x24,  0x2A,  0x2A,  0x2A,  0x10,  0x0,    // [0x73] 's'
    0x1F,  0x22,  0x22,  0x20,  0x10,  0x0,    // [0x74] 't'
    0x1E,  0x20,  0x20,  0x20,  0x1E,  0x0,    // [0x75] 'u'
    0xE,  0x10,  0x20,  0x10,  0xE,  0x0,    // [0x76] 'v'
    0x1E,  0x20,  0x10,  0x20,  0x1E,  0x0,    // [0x77] 'w'
    0x22,  0x14,  0x8,  0x14,  0x22,  0x0,    // [0x78] 'x'
    0xE,  0x50,  0x50,  0x50,  0x3E,  0x0,    // [0x79] 'y'
    0x22,  0x32,  0x2A,  0x26,  0x22,  0x0,    // [0x7A] 'z'
    0x0,  0x8,  0x36,  0x41,  0x0,  0x0,    // [0x7B] '{'
    0x0,  0x0,  0x7F,  0x0,  0x0,  0x0,    // [0x7C] '|'
    0x0,  0x41,  0x36,  0x8,  0x0,  0x0,    // [0x7D] '}'
    0x0,  0x2,  0x1,  0x2,  0x1,  0x0,    // [0x7E] '~'
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,    // [0x7F] ''
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,    // [0x80] ''
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,    // [0x81] ''
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,    // [0x82] ''
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,    // [0x83] ''
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,    // [0x84] ''
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,    // [0x85] ''
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,    // [0x86] ''
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,    // [0x87] ''
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,    // [0x88] ''
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,    // [0x89] ''
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,    // [0x8A] ''
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0,    // [0x8B] ''
};

#define SSD1306_CMD_REG         0x00
#define SSD1306_DATA_REG        0x40

#define SSD1306_SET_CONTRAST 0x81
#define SSD1306_DISPLAY_ALL_ON_RESUME 0xA4
#define SSD1306_DISPLAY_ALL_ON 0xA5
#define SSD1306_NORMAL_DISPLAY 0xA6
#define SSD1306_INVERT_DISPLAY 0xA7
#define SSD1306_DISPLAY_OFF 0xAE
#define SSD1306_DISPLAY_ON 0xAF

#define SSD1306_SET_DISPLAY_OFFSET 0xD3
#define SSD1306_SET_COM_PINS 0xDA

#define SSD1306_SET_VCOM_DETECT 0xDB

#define SSD1306_SET_DISPLAY_CLOCK_DIV 0xD5
#define SSD1306_SET_PRECHARGE 0xD9

#define SSD1306_SET_MULTIPLEX 0xA8

#define SSD1306_SET_LOW_COLUMN 0x00
#define SSD1306_SET_HIGH_COLUMN 0x10

#define SSD1306_SET_START_LINE 0x40

#define SSD1306_MEMORY_MODE 0x20
#define SSD1306_COLUMN_ADDR 0x21
#define SSD1306_PAGE_ADDR   0x22

#define SSD1306_COM_SCAN_INC 0xC0
#define SSD1306_COM_SCAN_DEC 0xC8

#define SSD1306_SEG_REMAP 0xA0

#define SSD1306_CHARGE_PUMP 0x8D

#define SSD1306_EXTERNAL_VCC 0x1
#define SSD1306_SWITCH_CAP_VCC 0x2

// Scrolling #defines
#define SSD1306_ACTIVATE_SCROLL 0x2F
#define SSD1306_DEACTIVATE_SCROLL 0x2E
#define SSD1306_SET_VERTICAL_SCROLL_AREA 0xA3
#define SSD1306_RIGHT_HORIZONTAL_SCROLL 0x26
#define SSD1306_LEFT_HORIZONTAL_SCROLL 0x27
#define SSD1306_VERTICAL_AND_RIGHT_HORIZONTAL_SCROLL 0x29
#define SSD1306_VERTICAL_AND_LEFT_HORIZONTAL_SCROLL 0x2A

#if McuSSD1306_CONFIG_SSD1306_128X64
  #define SSD1306_LCDWIDTH                      128
  #define SSD1306_LCDHEIGHT                     64
  #define SSD1306_DISPLAY_HW_NOF_COLUMNS        128u /* number of columns in hardware */
  #define SSD1306_DISPLAY_HW_NOF_ROWS           64u /* number of rows in hardware */
  #define SSD1306_DISPLAY_HW_NOF_PAGES          8u
#elif McuSSD1306_CONFIG_SSD1306_128X32
  #define SSD1306_LCDWIDTH                      128
  #define SSD1306_LCDHEIGHT                     32
  #define SSD1306_DISPLAY_HW_NOF_COLUMNS        128u /* number of columns in hardware */
  #define SSD1306_DISPLAY_HW_NOF_ROWS           32u /* number of rows in hardware */
  #define SSD1306_DISPLAY_HW_NOF_PAGES          4u
#endif

static void SSD1306_WriteCommand(uint8_t cmd) {
  McuGenericI2C_WriteByteAddress8(McuSSD1306_CONFIG_SSD1306_I2C_ADDR, SSD1306_CMD_REG, cmd);
#if McuSSD1306_CONFIG_SSD1306_I2C_DELAY_US>0
  McuWait_Waitus(McuSSD1306_CONFIG_SSD1306_I2C_DELAY_US);
#endif
}

static void SSD1306_WriteData(uint8_t data) {
  McuGenericI2C_WriteByteAddress8(McuSSD1306_CONFIG_SSD1306_I2C_ADDR, SSD1306_DATA_REG, data);
#if McuSSD1306_CONFIG_SSD1306_I2C_DELAY_US>0
  McuWait_Waitus(McuSSD1306_CONFIG_SSD1306_I2C_DELAY_US);
#endif
}

static uint8_t actCol = 0;
static uint8_t actPage = 0;

static void SSD1306_SetPageStartAddr(uint8_t page) {
  actPage = page;
  if(actPage>=SSD1306_DISPLAY_HW_NOF_PAGES) {
    return;
  }
  SSD1306_WriteCommand(0xB0 | actPage);
}

static void SSD1306_SetColStartAddr(uint8_t col){
  actCol = col;
  if(actCol>=SSD1306_DISPLAY_HW_NOF_COLUMNS) {
    return;
  }
  SSD1306_WriteCommand(0x10 | (actCol>>4));
  SSD1306_WriteCommand(actCol & 0x0F);
}

static void SSD1306_Clear(void) {
  uint8_t col, page;

  for(page=0; page<SSD1306_DISPLAY_HW_NOF_PAGES; page++) {
    SSD1306_SetPageStartAddr(page);
    SSD1306_SetColStartAddr(0);
    for(col=0; col<SSD1306_DISPLAY_HW_NOF_COLUMNS ; col++) {
      SSD1306_WriteData(0x00);
    }
  }
  actPage = 0;
  actCol = 0;
}

static void SSD1306_PrintChar(uint8_t ch) {
  uint8_t i;

  for(i = 0; i<5; i++) {
    SSD1306_WriteData(font[((ch-0x20)*6)+i]);
  }
}

void SSD1306_PrintString(uint8_t *ch) {
  while(*ch != '\0'){
    if(*ch == '\n') {
      actPage++;
      if(actPage >= SSD1306_DISPLAY_HW_NOF_PAGES) {
        actPage=0;
      }
      SSD1306_SetPageStartAddr(actPage);
      SSD1306_SetColStartAddr(0);
      ch++;
    } else {
      SSD1306_PrintChar(*ch++);
    }
  }
}

/*
** ===================================================================
**     Method      :  McuSSD1306_WriteData (component SSD1306)
**     Description :
**         Writes a data byte to the bus
**     Parameters  :
**         NAME            - DESCRIPTION
**         data            - data byte to send
**     Returns     : Nothing
** ===================================================================
*/
void McuSSD1306_WriteData(uint8_t data)
{
}

/*
** ===================================================================
**     Method      :  McuSSD1306_WriteDataWordRepeated (component SSD1306)
**     Description :
**         Sends a data word to the display a number of times
**     Parameters  :
**         NAME            - DESCRIPTION
**         data            - data to write
**         nof             - How many times the data word shall be sent
**     Returns     : Nothing
** ===================================================================
*/
void McuSSD1306_WriteDataWordRepeated(uint16_t data, size_t nof)
{
}

/*
** ===================================================================
**     Method      :  McuSSD1306_WriteDataBlock (component SSD1306)
**     Description :
**         Sends a data buffer to the display
**     Parameters  :
**         NAME            - DESCRIPTION
**       * data            - Pointer to data to write
**         dataSize        - 
**     Returns     : Nothing
** ===================================================================
*/
void McuSSD1306_WriteDataBlock(uint8_t *data, size_t dataSize)
{
}

/*
** ===================================================================
**     Method      :  McuSSD1306_WriteCommand (component SSD1306)
**     Description :
**         Sends a command byte to the bus
**     Parameters  :
**         NAME            - DESCRIPTION
**         cmd             - the command to be sent
**     Returns     : Nothing
** ===================================================================
*/
void McuSSD1306_WriteCommand(uint8_t cmd)
{
}

/*
** ===================================================================
**     Method      :  McuSSD1306_WriteDataWord (component SSD1306)
**     Description :
**         Sends a data word to the display
**     Parameters  :
**         NAME            - DESCRIPTION
**         data            - data to write
**     Returns     : Nothing
** ===================================================================
*/
void McuSSD1306_WriteDataWord(uint16_t data)
{
}

/*
** ===================================================================
**     Method      :  McuSSD1306_OpenWindow (component SSD1306)
**     Description :
**         Opens a window inside the display we want to write to.
**     Parameters  :
**         NAME            - DESCRIPTION
**         x0              - 
**         y0              - 
**         x1              - 
**         y1              - 
**     Returns     : Nothing
** ===================================================================
*/
void McuSSD1306_OpenWindow(McuSSD1306_PixelDim x0, McuSSD1306_PixelDim y0, McuSSD1306_PixelDim x1, McuSSD1306_PixelDim y1)
{
}

/*
** ===================================================================
**     Method      :  McuSSD1306_Clear (component SSD1306)
**     Description :
**         Clears the whole display memory.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void McuSSD1306_Clear(void)
{
  SSD1306_Clear();
}

/*
** ===================================================================
**     Method      :  McuSSD1306_UpdateFull (component SSD1306)
**     Description :
**         Updates the whole display. This is only a stub for this
**         display as we are using windowing.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void McuSSD1306_UpdateFull(void)
{
  uint8_t col, page;

  for(page=0; page<SSD1306_DISPLAY_HW_NOF_PAGES; page++) {
    SSD1306_SetPageStartAddr(page);
    SSD1306_SetColStartAddr(0);
    for(col=0; col<SSD1306_DISPLAY_HW_NOF_COLUMNS ; col++) {
      SSD1306_WriteData(McuSSD1306_DisplayBuf[page][col]);
    }
  }
}

/*
** ===================================================================
**     Method      :  McuSSD1306_UpdateRegion (component SSD1306)
**     Description :
**         Updates a region of the display. This is only a stub for
**         this display as we are using windowing.
**     Parameters  :
**         NAME            - DESCRIPTION
**         x               - x coordinate
**         y               - y coordinate
**         w               - width of the region
**         h               - Height of the region
**     Returns     : Nothing
** ===================================================================
*/
/*
void McuSSD1306_UpdateRegion(McuSSD1306_PixelDim x, McuSSD1306_PixelDim y, McuSSD1306_PixelDim w, McuSSD1306_PixelDim h)
{
  implemented as macro in McuSSD1306.h
}
*/

/*
** ===================================================================
**     Method      :  McuSSD1306_CloseWindow (component SSD1306)
**     Description :
**         Closes a window previously opened with OpenWindow()
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
/*
void McuSSD1306_CloseWindow(void)
{
  implemented as macro in McuSSD1306.h
}
*/

/*
** ===================================================================
**     Method      :  McuSSD1306_GetDisplayOrientation (component SSD1306)
**     Description :
**         Returns the current display orientation
**     Parameters  : None
**     Returns     :
**         ---             - current display orientation
** ===================================================================
*/
McuSSD1306_DisplayOrientation McuSSD1306_GetDisplayOrientation(void)
{
#if McuSSD1306_CONFIG_DYNAMIC_DISPLAY_ORIENTATION
  return currentOrientation;
#elif McuSSD1306_CONFIG_FIXED_DISPLAY_ORIENTATION==McuSSD1306_CONFIG_ORIENTATION_PORTRAIT
  return McuSSD1306_ORIENTATION_PORTRAIT; /* Portrait mode */
#elif McuSSD1306_CONFIG_FIXED_DISPLAY_ORIENTATION==McuSSD1306_CONFIG_ORIENTATION_PORTRAIT180
  return McuSSD1306_ORIENTATION_PORTRAIT180; /* Portrait mode, rotated 180� */
#elif McuSSD1306_CONFIG_FIXED_DISPLAY_ORIENTATION==McuSSD1306_CONFIG_ORIENTATION_LANDSCAPE
  return McuSSD1306_ORIENTATION_LANDSCAPE; /* Landscape mode, rotated right 90� */
#elif McuSSD1306_CONFIG_FIXED_DISPLAY_ORIENTATION==McuSSD1306_CONFIG_ORIENTATION_LANDSCAPE180
  return McuSSD1306_ORIENTATION_LANDSCAPE180; /* Landscape mode, rotated left 90� */
#endif
}

/*
** ===================================================================
**     Method      :  McuSSD1306_SetDisplayOrientation (component SSD1306)
**     Description :
**         Sets the display orientation. If you enable this method,
**         then the orientation of the display can be changed at
**         runtime. However, this requires additional resources.
**     Parameters  :
**         NAME            - DESCRIPTION
**         newOrientation  - new orientation to
**                           be used
**     Returns     : Nothing
** ===================================================================
*/
void McuSSD1306_SetDisplayOrientation(McuSSD1306_DisplayOrientation newOrientation)
{
}

/*
** ===================================================================
**     Method      :  McuSSD1306_GetWidth (component SSD1306)
**     Description :
**         Returns the width of the display in pixels (in x direction)
**     Parameters  : None
**     Returns     :
**         ---             - Width of display
** ===================================================================
*/
#if McuSSD1306_CONFIG_DYNAMIC_DISPLAY_ORIENTATION
McuSSD1306_PixelDim McuSSD1306_GetWidth(void)
{
  if (currentOrientation==McuSSD1306_ORIENTATION_PORTRAIT || currentOrientation==McuSSD1306_ORIENTATION_PORTRAIT180) {
    return McuSSD1306_HW_SHORTER_SIDE;
  } else { /* McuSSD1306_ORIENTATION_LANDSCAPE or McuSSD1306_ORIENTATION_LANDSCAPE180 */
    return McuSSD1306_HW_LONGER_SIDE;
  }
}
#else
/*
McuSSD1306_PixelDim McuSSD1306_GetWidth(void)
{
  implemented as macro in McuSSD1306.h
}
*/
#endif

/*
** ===================================================================
**     Method      :  McuSSD1306_GetHeight (component SSD1306)
**     Description :
**         Returns the height of the display in pixels (in y direction)
**     Parameters  : None
**     Returns     :
**         ---             - Height of display
** ===================================================================
*/
#if McuSSD1306_CONFIG_DYNAMIC_DISPLAY_ORIENTATION
McuSSD1306_PixelDim McuSSD1306_GetHeight(void)
{
  if (currentOrientation==McuSSD1306_ORIENTATION_PORTRAIT || currentOrientation==McuSSD1306_ORIENTATION_PORTRAIT180) {
    return McuSSD1306_HW_LONGER_SIDE;
  } else { /* McuSSD1306_ORIENTATION_LANDSCAPE or McuSSD1306_ORIENTATION_LANDSCAPE180 */
    return McuSSD1306_HW_SHORTER_SIDE;
  }
}
#else
/*
McuSSD1306_PixelDim McuSSD1306_GetHeight(void)
{
  implemented as macro in McuSSD1306.h
}
*/
#endif

/*
** ===================================================================
**     Method      :  McuSSD1306_GetLongerSide (component SSD1306)
**     Description :
**         Returns the size of the longer side of the display
**     Parameters  : None
**     Returns     :
**         ---             - number of pixels
** ===================================================================
*/
/*
McuSSD1306_PixelDim McuSSD1306_GetLongerSide(void)
{
  implemented as macro in McuSSD1306.h
}
*/

/*
** ===================================================================
**     Method      :  McuSSD1306_GetShorterSide (component SSD1306)
**     Description :
**         Returns the size of the shorter side of the display
**     Parameters  : None
**     Returns     :
**         ---             - number of pixels
** ===================================================================
*/
/*
McuSSD1306_PixelDim McuSSD1306_GetShorterSide(void)
{
  implemented as macro in McuSSD1306.h
}
*/

/*
** ===================================================================
**     Method      :  McuSSD1306_GetLCD (component SSD1306)
**     Description :
**         Method to be called for mutual exclusive access to the LCD
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void McuSSD1306_GetLCD(void)
{
  McuSSD1306_InitCommChannel();
}

/*
** ===================================================================
**     Method      :  McuSSD1306_GiveLCD (component SSD1306)
**     Description :
**         Method to be called for mutual exclusive access to the LCD
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void McuSSD1306_GiveLCD(void)
{
}

/*
** ===================================================================
**     Method      :  McuSSD1306_InitCommChannel (component SSD1306)
**     Description :
**         Method to initialize communication to the LCD. Needed if the
**         bus to the LCD is shared with other components and settings
**         are different.
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void McuSSD1306_InitCommChannel(void)
{
}

/*
** ===================================================================
**     Method      :  McuSSD1306_Init (component SSD1306)
**     Description :
**         Display driver initialization
**     Parameters  : None
**     Returns     : Nothing
** ===================================================================
*/
void McuSSD1306_Init(void)
{
#if McuSSD1306_CONFIG_SSD1306_HAS_RST
  McuSSD1306_CONFIG_SSD1306_RESET_LOW();
  McuWait_Waitms(1);                                                 /* wait for 1 ms */
  McuSSD1306_CONFIG_SSD1306_RESET_HIGH();
#endif
  SSD1306_WriteCommand(SSD1306_DISPLAY_OFF);                         /* turn of display */
  SSD1306_WriteCommand(SSD1306_SET_DISPLAY_CLOCK_DIV);               /* set display clock divide ratio/oscillator frequency */
  SSD1306_WriteCommand(0x80);                                        /* the suggested ratio 0x80 */

  SSD1306_WriteCommand(SSD1306_SET_MULTIPLEX);                       /* set multiplex ratio(1 to 64) */
#if McuSSD1306_CONFIG_SSD1306_128X64
  SSD1306_WriteCommand(0x3F);                                        /* the suggested 1/64 duty */
#elif McuSSD1306_CONFIG_SSD1306_128X32
  SSD1306_WriteCommand(0x1F);
#else
  #error "not supported"
#endif

  SSD1306_WriteCommand(SSD1306_SET_DISPLAY_OFFSET);                  /* set display offset */
  SSD1306_WriteCommand(0x00);                                        /* no offset */
  SSD1306_WriteCommand(SSD1306_SET_START_LINE | 0x00);               /* set start line address */

  SSD1306_WriteCommand(SSD1306_COLUMN_ADDR);                         /* set start and end address of the columns */
  SSD1306_WriteCommand(0);
  SSD1306_WriteCommand(SSD1306_DISPLAY_HW_NOF_COLUMNS-1);

  SSD1306_WriteCommand(SSD1306_PAGE_ADDR);                           /* set start and end address of the pages */
  SSD1306_WriteCommand(0);
  SSD1306_WriteCommand(SSD1306_DISPLAY_HW_NOF_PAGES-1);

  SSD1306_WriteCommand(SSD1306_CHARGE_PUMP);                         /* set charge pump enable/disable */
#if McuSSD1306_CONFIG_SSD1306_EXTERNAL == 1
  SSD1306_WriteCommand(0x10);                                        /* set to disabled */
#else
  SSD1306_WriteCommand(0x14);                                        /* set to enabled */
#endif

  SSD1306_WriteCommand(SSD1306_MEMORY_MODE);                         /* set memory mode */
  SSD1306_WriteCommand(0x00);

  SSD1306_WriteCommand(SSD1306_SEG_REMAP | 0x01);                    /* set segment re-map 128 to 0 */
  SSD1306_WriteCommand(SSD1306_COM_SCAN_DEC);                        /* set COM output scan direction 64 to 0 */
  SSD1306_WriteCommand(SSD1306_SET_COM_PINS);                        /* set COM pins hardware configuration */
#if McuSSD1306_CONFIG_SSD1306_128X64
  SSD1306_WriteCommand(0x12);                                        /* the suggested 1/64 duty */
#else
  SSD1306_WriteCommand(0x02);
#endif

  SSD1306_WriteCommand(SSD1306_SET_CONTRAST);                        /* set contrast control register */
#if McuSSD1306_CONFIG_SSD1306_128X64
  #if McuSSD1306_CONFIG_SSD1306_EXTERNAL == 1
  SSD1306_WriteCommand(0x9F);
  #else
  SSD1306_WriteCommand(0xCF);
  #endif
#else
  SSD1306_WriteCommand(0x8F);
#endif

  SSD1306_WriteCommand(SSD1306_SET_PRECHARGE);                       /* set pre-charge period */
#if McuSSD1306_CONFIG_SSD1306_EXTERNAL == 1
  SSD1306_WriteCommand(0x22);
#else
  SSD1306_WriteCommand(0xF1);
#endif
  SSD1306_WriteCommand(0xF1);
  SSD1306_WriteCommand(SSD1306_SET_VCOM_DETECT);                     /* set vcomh */
  SSD1306_WriteCommand(0x40);

  SSD1306_WriteCommand(SSD1306_DISPLAY_ALL_ON_RESUME);               /* disable entire display on */
  SSD1306_WriteCommand(SSD1306_NORMAL_DISPLAY);                      /* set normal display */
  SSD1306_WriteCommand(SSD1306_DISPLAY_ON);                          /* turn on oled panel */

#if McuSSD1306_CONFIG_FIXED_DISPLAY_ORIENTATION==McuSSD1306_CONFIG_ORIENTATION_PORTRAIT
  McuSSD1306_SetDisplayOrientation(McuSSD1306_ORIENTATION_PORTRAIT); /* Portrait mode */
#elif McuSSD1306_CONFIG_FIXED_DISPLAY_ORIENTATION==McuSSD1306_CONFIG_ORIENTATION_PORTRAIT180
  McuSSD1306_SetDisplayOrientation(McuSSD1306_ORIENTATION_PORTRAIT180); /* Portrait mode, rotated 180� */
#elif McuSSD1306_CONFIG_FIXED_DISPLAY_ORIENTATION==McuSSD1306_CONFIG_ORIENTATION_LANDSCAPE
  McuSSD1306_SetDisplayOrientation(McuSSD1306_ORIENTATION_LANDSCAPE); /* Landscape mode, rotated right 90� */
#elif McuSSD1306_CONFIG_FIXED_DISPLAY_ORIENTATION==McuSSD1306_CONFIG_ORIENTATION_LANDSCAPE180
  McuSSD1306_SetDisplayOrientation(McuSSD1306_ORIENTATION_LANDSCAPE180); /* Landscape mode, rotated left 90� */
#endif
#if McuSSD1306_CONFIG_CLEAR_DISPLAY_IN_INIT
  McuSSD1306_Clear();
#endif
}

/*
** ===================================================================
**     Method      :  McuSSD1306_OnRequestBus (component SSD1306)
**
**     Description :
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void McuGenericI2C_OnRequestBus1(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Method      :  McuSSD1306_OnReleaseBus (component SSD1306)
**
**     Description :
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void McuGenericI2C_OnReleaseBus1(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Method      :  McuSSD1306_OnError (component SSD1306)
**
**     Description :
**         This method is internal. It is used by Processor Expert only.
** ===================================================================
*/
void McuGenericI2C_OnError(void)
{
  /* Write your code here ... */
}

/*
** ===================================================================
**     Method      :  McuSSD1306_SetContrast (component SSD1306)
**     Description :
**         Sets the display contrast level (default:0x7F)
**     Parameters  :
**         NAME            - DESCRIPTION
**         contrast        - Contrast level (default 0x7F).
**                           Contrast increases as the value increases.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuSSD1306_SetContrast(uint8_t contrast)
{
  SSD1306_WriteCommand(SSD1306_SET_CONTRAST);
  SSD1306_WriteCommand(contrast);
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  McuSSD1306_DisplayOn (component SSD1306)
**     Description :
**         Turns the display on or off (sleep)
**     Parameters  :
**         NAME            - DESCRIPTION
**         on              - turns the display on or off
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuSSD1306_DisplayOn(bool on)
{
  if(on) {
    SSD1306_WriteCommand(SSD1306_DISPLAY_ON);                        /* turn display on */
  } else {
    SSD1306_WriteCommand(SSD1306_DISPLAY_OFF);                       /* turn display off */
  }
  return ERR_OK;
}

/*
** ===================================================================
**     Method      :  McuSSD1306_DisplayInvert (component SSD1306)
**     Description :
**         Used to inverse the display. In normal mode, 1 one in the
**         display RAM is a pixel set and a 0 means pixel clear.
**     Parameters  :
**         NAME            - DESCRIPTION
**         invert          - if TRUE, a 1 is pixel clear and a 0
**                           is a pixel set.
**     Returns     :
**         ---             - Error code
** ===================================================================
*/
uint8_t McuSSD1306_DisplayInvert(bool invert)
{
  if(invert) {
    SSD1306_WriteCommand(SSD1306_INVERT_DISPLAY);
  } else {
    SSD1306_WriteCommand(SSD1306_NORMAL_DISPLAY);
  }
  return ERR_OK;
}

/* END McuSSD1306. */

/*!
** @}
*/
/*
** ###################################################################
**
**     This file was created by Processor Expert 10.5 [05.21]
**     for the Freescale Kinetis series of microcontrollers.
**
** ###################################################################
*/
