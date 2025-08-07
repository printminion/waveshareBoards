#include "Display_ST7789.h"
   
#define SPI_WRITE(_dat)         SPI.transfer(_dat)
#define SPI_WRITE_Word(_dat)    SPI.transfer16(_dat)

int Offset_X=34;
int Offset_Y=0;



void SPI_Init()
{
  SPI.begin(EXAMPLE_PIN_NUM_SCLK,EXAMPLE_PIN_NUM_MISO,EXAMPLE_PIN_NUM_MOSI); 
}

void LCD_WriteCommand(uint8_t Cmd)  
{ 
  SPI.beginTransaction(SPISettings(SPIFreq, MSBFIRST, SPI_MODE0));
  digitalWrite(EXAMPLE_PIN_NUM_LCD_CS, LOW);  
  digitalWrite(EXAMPLE_PIN_NUM_LCD_DC, LOW); 
  SPI_WRITE(Cmd);
  digitalWrite(EXAMPLE_PIN_NUM_LCD_CS, HIGH);  
  SPI.endTransaction();
}
void LCD_WriteData(uint8_t Data) 
{ 
  SPI.beginTransaction(SPISettings(SPIFreq, MSBFIRST, SPI_MODE0));
  digitalWrite(EXAMPLE_PIN_NUM_LCD_CS, LOW);  
  digitalWrite(EXAMPLE_PIN_NUM_LCD_DC, HIGH);  
  SPI_WRITE(Data);  
  digitalWrite(EXAMPLE_PIN_NUM_LCD_CS, HIGH);  
  SPI.endTransaction();
}    
void LCD_WriteData_Word(uint16_t Data)
{
  SPI.beginTransaction(SPISettings(SPIFreq, MSBFIRST, SPI_MODE0));
  digitalWrite(EXAMPLE_PIN_NUM_LCD_CS, LOW);  
  digitalWrite(EXAMPLE_PIN_NUM_LCD_DC, HIGH); 
  SPI_WRITE_Word(Data);
  digitalWrite(EXAMPLE_PIN_NUM_LCD_CS, HIGH);  
  SPI.endTransaction();
}   
void LCD_WriteData_nbyte(uint8_t* SetData,uint8_t* ReadData,uint32_t Size) 
{ 
  SPI.beginTransaction(SPISettings(SPIFreq, MSBFIRST, SPI_MODE0));
  digitalWrite(EXAMPLE_PIN_NUM_LCD_CS, LOW);  
  digitalWrite(EXAMPLE_PIN_NUM_LCD_DC, HIGH);  
  SPI.transferBytes(SetData, ReadData, Size);
  digitalWrite(EXAMPLE_PIN_NUM_LCD_CS, HIGH);  
  SPI.endTransaction();
} 

void LCD_Reset(void)
{
  digitalWrite(EXAMPLE_PIN_NUM_LCD_CS, LOW);       
  delay(50);
  digitalWrite(EXAMPLE_PIN_NUM_LCD_RST, LOW); 
  delay(50);
  digitalWrite(EXAMPLE_PIN_NUM_LCD_RST, HIGH); 
  delay(50);
}

void SetOrientation(uint8_t orientation)
{
    uint8_t madctl = 0;

    switch (orientation)
    {
    case VERTICAL: // Portrait
        madctl = 0x00; // MY=0, MX=0, MV=0
        Offset_X = 34;
        Offset_Y = 0;
        break;
    case HORIZONTAL: // Landscape
        madctl = 0x60; // MY=0, MX=1, MV=1
        Offset_X = 0;
        Offset_Y = 34;
        break;
    case 2: // Inverted Portrait
        madctl = 0xC0; // MY=1, MX=1, MV=0
        Offset_X = 34;
        Offset_Y = 0;
        break;
    case 3: // Inverted Landscape
        madctl = 0xA0; // MY=1, MX=0, MV=1
        Offset_X = 0;
        Offset_Y = 34;
        break;
    default:
        return; // Invalid orientation
    }

    LCD_WriteCommand(0x36);
    LCD_WriteData(madctl);
}
void LCD_Init(void)
{
  pinMode(EXAMPLE_PIN_NUM_LCD_CS, OUTPUT);
  pinMode(EXAMPLE_PIN_NUM_LCD_DC, OUTPUT);
  pinMode(EXAMPLE_PIN_NUM_LCD_RST, OUTPUT); 
  Backlight_Init();
  SPI_Init();

  LCD_Reset();
  //************* Start Initial Sequence **********// 
  LCD_WriteCommand(0x11);
  delay(120);
  LCD_WriteCommand(0x36);

   SetOrientation(HORIZONTAL); // Change to VERTICAL if needed

  LCD_WriteCommand(0x3A);
  LCD_WriteData(0x05);

  LCD_WriteCommand(0xB0);
  LCD_WriteData(0x00);
  LCD_WriteData(0xE8);
  
  LCD_WriteCommand(0xB2);
  LCD_WriteData(0x0C);
  LCD_WriteData(0x0C);
  LCD_WriteData(0x00);
  LCD_WriteData(0x33);
  LCD_WriteData(0x33);

  LCD_WriteCommand(0xB7);
  LCD_WriteData(0x35);

  LCD_WriteCommand(0xBB);
  LCD_WriteData(0x35);

  LCD_WriteCommand(0xC0);
  LCD_WriteData(0x2C);

  LCD_WriteCommand(0xC2);
  LCD_WriteData(0x01);

  LCD_WriteCommand(0xC3);
  LCD_WriteData(0x13);

  LCD_WriteCommand(0xC4);
  LCD_WriteData(0x20);

  LCD_WriteCommand(0xC6);
  LCD_WriteData(0x0F);

  LCD_WriteCommand(0xD0);
  LCD_WriteData(0xA4);
  LCD_WriteData(0xA1);

  LCD_WriteCommand(0xD6);
  LCD_WriteData(0xA1);

  LCD_WriteCommand(0xE0);
  LCD_WriteData(0xF0);
  LCD_WriteData(0x00);
  LCD_WriteData(0x04);
  LCD_WriteData(0x04);
  LCD_WriteData(0x04);
  LCD_WriteData(0x05);
  LCD_WriteData(0x29);
  LCD_WriteData(0x33);
  LCD_WriteData(0x3E);
  LCD_WriteData(0x38);
  LCD_WriteData(0x12);
  LCD_WriteData(0x12);
  LCD_WriteData(0x28);
  LCD_WriteData(0x30);

  LCD_WriteCommand(0xE1);
  LCD_WriteData(0xF0);
  LCD_WriteData(0x07);
  LCD_WriteData(0x0A);
  LCD_WriteData(0x0D);
  LCD_WriteData(0x0B);
  LCD_WriteData(0x07);
  LCD_WriteData(0x28);
  LCD_WriteData(0x33);
  LCD_WriteData(0x3E);
  LCD_WriteData(0x36);
  LCD_WriteData(0x14);
  LCD_WriteData(0x14);
  LCD_WriteData(0x29);
  LCD_WriteData(0x32);

  LCD_WriteCommand(0x21);

  LCD_WriteCommand(0x11);
  delay(120);
  LCD_WriteCommand(0x29); 
}
/******************************************************************************
function: Set the cursor position
parameter :
    Xstart:   Start uint16_t x coordinate
    Ystart:   Start uint16_t y coordinate
    Xend  :   End uint16_t coordinates
    Yend  :   End uint16_t coordinatesen
******************************************************************************/
void LCD_SetCursor(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t  Yend)
{ 
  if (HORIZONTAL) {
    // set the X coordinates
    LCD_WriteCommand(0x2A);
    LCD_WriteData(Xstart >> 8);
    LCD_WriteData(Xstart + Offset_X);
    LCD_WriteData(Xend >> 8);
    LCD_WriteData(Xend + Offset_X);
    
    // set the Y coordinates
    LCD_WriteCommand(0x2B);
    LCD_WriteData(Ystart >> 8);
    LCD_WriteData(Ystart + Offset_Y);
    LCD_WriteData(Yend >> 8);
    LCD_WriteData(Yend + Offset_Y);
  }
  else {
    // set the X coordinates
    LCD_WriteCommand(0x2A);
    LCD_WriteData(Ystart >> 8);
    LCD_WriteData(Ystart + Offset_Y);
    LCD_WriteData(Yend >> 8);
    LCD_WriteData(Yend + Offset_Y);
    // set the Y coordinates
    LCD_WriteCommand(0x2B);
    LCD_WriteData(Xstart >> 8);
    LCD_WriteData(Xstart + Offset_X);
    LCD_WriteData(Xend >> 8);
    LCD_WriteData(Xend + Offset_X);
  }
  LCD_WriteCommand(0x2C);
}
/******************************************************************************
function: Refresh the image in an area
parameter :
    Xstart:   Start uint16_t x coordinate
    Ystart:   Start uint16_t y coordinate
    Xend  :   End uint16_t coordinates
    Yend  :   End uint16_t coordinates
    color :   Set the color
******************************************************************************/
void LCD_addWindow(uint16_t Xstart, uint16_t Ystart, uint16_t Xend, uint16_t Yend,uint16_t* color)
{          
  // uint16_t i,j;
  // LCD_SetCursor(Xstart, Ystart, Xend,Yend);
  // uint16_t Show_Width = Xend - Xstart + 1;
  // uint16_t Show_Height = Yend - Ystart + 1;
  // for(i = 0; i < Show_Height; i++){               
  //   for(j = 0; j < Show_Width; j++){
  //     LCD_WriteData_Word(color[(i*(Show_Width))+j]);                           
  //   }
  // }           
  uint16_t Show_Width = Xend - Xstart + 1;
  uint16_t Show_Height = Yend - Ystart + 1;
  uint32_t numBytes = Show_Width * Show_Height * sizeof(uint16_t);
  uint8_t Read_D[numBytes];
  LCD_SetCursor(Xstart, Ystart, Xend, Yend);
  LCD_WriteData_nbyte((uint8_t*)color, Read_D, numBytes);        
}
// backlight
void Backlight_Init(void)
{
  ledcAttach(EXAMPLE_PIN_NUM_BK_LIGHT, Frequency, Resolution);   
  ledcWrite(EXAMPLE_PIN_NUM_BK_LIGHT, 100);                        
}

void Set_Backlight(uint8_t Light)                        //
{
  if(Light > 100 || Light < 0)
    printf("Set Backlight parameters in the range of 0 to 100 \r\n");
  else{
    uint32_t Backlight = Light*10;
    ledcWrite(EXAMPLE_PIN_NUM_BK_LIGHT, Backlight);
  }
}

// Basic 5x8 ASCII font starting from space (32)
const unsigned char font5x7[] = {
    0x00, 0x00, 0x00, 0x00, 0x00, // Space
    0x00, 0x00, 0x5F, 0x00, 0x00, // !
    0x00, 0x07, 0x00, 0x07, 0x00, // "
    0x14, 0x7F, 0x14, 0x7F, 0x14, // #
    0x24, 0x2A, 0x7F, 0x2A, 0x12, // $
    0x23, 0x13, 0x08, 0x64, 0x62, // %
    0x36, 0x49, 0x55, 0x22, 0x50, // &
    0x00, 0x05, 0x03, 0x00, 0x00, // '
    0x00, 0x1C, 0x22, 0x41, 0x00, // (
    0x00, 0x41, 0x22, 0x1C, 0x00, // )
    0x14, 0x08, 0x3E, 0x08, 0x14, // *
    0x08, 0x08, 0x3E, 0x08, 0x08, // +
    0x00, 0x50, 0x30, 0x00, 0x00, // ,
    0x08, 0x08, 0x08, 0x08, 0x08, // -
    0x00, 0x60, 0x60, 0x00, 0x00, // .
    0x20, 0x10, 0x08, 0x04, 0x02, // /
    0x3E, 0x51, 0x49, 0x45, 0x3E, // 0
    0x00, 0x42, 0x7F, 0x40, 0x00, // 1
    0x42, 0x61, 0x51, 0x49, 0x46, // 2
    0x21, 0x41, 0x45, 0x4B, 0x31, // 3
    0x18, 0x14, 0x12, 0x7F, 0x10, // 4
    0x27, 0x45, 0x45, 0x45, 0x39, // 5
    0x3C, 0x4A, 0x49, 0x49, 0x30, // 6
    0x01, 0x71, 0x09, 0x05, 0x03, // 7
    0x36, 0x49, 0x49, 0x49, 0x36, // 8
    0x06, 0x49, 0x49, 0x29, 0x1E, // 9
    0x00, 0x36, 0x36, 0x00, 0x00, // :
    0x00, 0x56, 0x36, 0x00, 0x00, // ;
    0x08, 0x14, 0x22, 0x41, 0x00, // <
    0x14, 0x14, 0x14, 0x14, 0x14, // =
    0x00, 0x41, 0x22, 0x14, 0x08, // >
    0x02, 0x01, 0x51, 0x09, 0x06, // ?
    0x32, 0x49, 0x79, 0x41, 0x3E, // @
    0x7E, 0x11, 0x11, 0x11, 0x7E, // A
    0x7F, 0x49, 0x49, 0x49, 0x36, // B
    0x3E, 0x41, 0x41, 0x41, 0x22, // C
    0x7F, 0x41, 0x41, 0x22, 0x1C, // D
    0x7F, 0x49, 0x49, 0x49, 0x41, // E
    0x7F, 0x09, 0x09, 0x09, 0x01, // F
    0x3E, 0x41, 0x49, 0x49, 0x7A, // G
    0x7F, 0x08, 0x08, 0x08, 0x7F, // H
    0x00, 0x41, 0x7F, 0x41, 0x00, // I
    0x20, 0x40, 0x41, 0x3F, 0x01, // J
    0x7F, 0x08, 0x14, 0x22, 0x41, // K
    0x7F, 0x40, 0x40, 0x40, 0x40, // L
    0x7F, 0x02, 0x0C, 0x02, 0x7F, // M
    0x7F, 0x04, 0x08, 0x10, 0x7F, // N
    0x3E, 0x41, 0x41, 0x41, 0x3E, // O
    0x7F, 0x09, 0x09, 0x09, 0x06, // P
    0x3E, 0x41, 0x51, 0x21, 0x5E, // Q
    0x7F, 0x09, 0x19, 0x29, 0x46, // R
    0x46, 0x49, 0x49, 0x49, 0x31, // S
    0x01, 0x01, 0x7F, 0x01, 0x01, // T
    0x3F, 0x40, 0x40, 0x40, 0x3F, // U
    0x1F, 0x20, 0x40, 0x20, 0x1F, // V
    0x3F, 0x40, 0x38, 0x40, 0x3F, // W
    0x63, 0x14, 0x08, 0x14, 0x63, // X
    0x07, 0x08, 0x70, 0x08, 0x07, // Y
    0x61, 0x51, 0x49, 0x45, 0x43, // Z
    0x00, 0x7F, 0x41, 0x41, 0x00, // [
    0x02, 0x04, 0x08, 0x10, 0x20, // backslash
    0x00, 0x41, 0x41, 0x7F, 0x00, // ]
    0x04, 0x02, 0x01, 0x02, 0x04, // ^
    0x40, 0x40, 0x40, 0x40, 0x40, // _
    0x00, 0x01, 0x02, 0x04, 0x00, // `
    0x20, 0x54, 0x54, 0x54, 0x78, // a
    0x7F, 0x48, 0x44, 0x44, 0x38, // b
    0x38, 0x44, 0x44, 0x44, 0x20, // c
    0x38, 0x44, 0x44, 0x48, 0x7F, // d
    0x38, 0x54, 0x54, 0x54, 0x18, // e
    0x08, 0x7E, 0x09, 0x01, 0x02, // f
    0x0C, 0x52, 0x52, 0x52, 0x3E, // g
    0x7F, 0x08, 0x04, 0x04, 0x78, // h
    0x00, 0x44, 0x7D, 0x40, 0x00, // i
    0x20, 0x40, 0x44, 0x3D, 0x00, // j
    0x7F, 0x10, 0x28, 0x44, 0x00, // k
    0x00, 0x41, 0x7F, 0x40, 0x00, // l
    0x7C, 0x04, 0x18, 0x04, 0x78, // m
    0x7C, 0x08, 0x04, 0x04, 0x78, // n
    0x38, 0x44, 0x44, 0x44, 0x38, // o
    0x7C, 0x14, 0x14, 0x14, 0x08, // p
    0x08, 0x14, 0x14, 0x18, 0x7C, // q
    0x7C, 0x08, 0x04, 0x04, 0x08, // r
    0x48, 0x54, 0x54, 0x54, 0x20, // s
    0x04, 0x3F, 0x44, 0x40, 0x20, // t
    0x3C, 0x40, 0x40, 0x20, 0x7C, // u
    0x1C, 0x20, 0x40, 0x20, 0x1C, // v
    0x3C, 0x40, 0x30, 0x40, 0x3C, // w
    0x44, 0x28, 0x10, 0x28, 0x44, // x
    0x0C, 0x50, 0x50, 0x50, 0x3C, // y
    0x44, 0x64, 0x54, 0x4C, 0x44, // z
    0x00, 0x08, 0x36, 0x41, 0x00, // {
    0x00, 0x00, 0x7F, 0x00, 0x00, // |
    0x00, 0x41, 0x36, 0x08, 0x00, // }
    0x10, 0x08, 0x08, 0x10, 0x08, // ~
};

void LCD_DrawChar(uint16_t x, uint16_t y, char ch, uint16_t color) {
    if (ch < 32 || ch > 126) return;  // Only printable ASCII
    
    // Clear background
    LCD_SetCursor(x, y, x + 5, y + 7);
    for (int i = 0; i < 48; i++) {
        LCD_WriteData_Word(COLOR_BLACK);
    }
    
    // Get character index (subtract 32 for space character offset)
    int char_index = ch - 32;
    
    // Draw character pixel by pixel
    for (int col = 0; col < 5; col++) {
        uint8_t column_data = font5x7[char_index * 5 + col];
        
        for (int row = 0; row < 8; row++) {
            if (column_data & (1 << row)) {
                // Draw pixel at position
                LCD_SetCursor(x + col, y + row, x + col, y + row);
                LCD_WriteData_Word(color);
            }
        }
    }
}

void LCD_DrawString(uint16_t x, uint16_t y, const char* str, uint16_t color) {
    while (*str) {
        LCD_DrawChar(x, y, *str++, color);
        x += 6;  // 5 pixels for character + 1 pixel spacing
    }
}

void LCD_Clear(uint16_t color) {
    uint16_t i, j;
    LCD_SetCursor(0, 0, LCD_WIDTH - 1, LCD_HEIGHT - 1);
    
    for (i = 0; i < LCD_HEIGHT; i++) {
        for (j = 0; j < LCD_WIDTH; j++) {
            LCD_WriteData_Word(color);
        }
    }
}

void LCD_ShowStatusMessage(const char* message) {
    static uint16_t messageY = 20;  // Start position from top
    
    // Clear area for new message
    LCD_SetCursor(10, messageY, LCD_WIDTH - 10, messageY + 8);
    for (int i = 0; i < (LCD_WIDTH - 20) * 9; i++) {
        LCD_WriteData_Word(COLOR_BLACK);
    }
    
    // Draw the message
    LCD_DrawString(10, messageY, message, COLOR_WHITE);
    
    // Move to next line
    messageY += 12;  // Height of character (8) + spacing (4)
    
    // Reset to top if we're near the bottom
    if (messageY > LCD_HEIGHT - 20) {
        messageY = 20;
        LCD_Clear(COLOR_BLACK);  // Clear screen when starting over
    }
}






