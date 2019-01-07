#include "oled.h"





OLed::~OLed()
{
    
}

boolean OLed::begin(void)
{
  boolean ret;


  Wire1.begin(17, 16);
    
  ret = Adafruit_SSD1306::begin(SSD1306_SWITCHCAPVCC, 0x3C, false, false);

  textcolor = WHITE;
  textbgcolor = BLACK;

  return ret;
}

void OLed::printf(int x, int y,  const char *fmt, ...)
{
  int32_t ret = 0;
  va_list arg;
  va_start (arg, fmt);
  int32_t len;
  char print_buffer[256];
  int Str_Size;
  int Size_Char;
  int i, x_Pre = x;
  PHAN_FONT_OBJ FontBuf;


  len = vsnprintf(print_buffer, 255, fmt, arg);
  va_end (arg);

  for( i=0; i<len; i+=Size_Char )
  {
    PHan_FontLoad( &print_buffer[i], &FontBuf );
      
      
    disHanFont( x, y, &FontBuf );
                      
    Size_Char = FontBuf.Size_Char;
    if (Size_Char >= 2)
    {           
        x += 2*8;
    }       
    else
    {
        x += 1*8;
    }
    
    if( 128 < x ) 
    {
        x  = x_Pre;
        y += 16;
    }
    
    if( FontBuf.Code_Type == PHAN_END_CODE ) break;
  }         
}

void OLed::disHanFont(int x, int y, PHAN_FONT_OBJ *FontPtr)
{
  uint16_t    i, j, Loop;
  uint16_t  FontSize = FontPtr->Size_Char;
  uint16_t index_x;

  if (FontSize > 2)
  {
    FontSize = 2;
  }

  for ( i = 0 ; i < 16 ; i++ )        // 16 Lines per Font/Char
  {
    index_x = 0;
    for ( j = 0 ; j < FontSize ; j++ )      // 16 x 16 (2 Bytes)
    {
      for( Loop=0; Loop<8; Loop++ )
      {
        if( FontPtr->FontBuffer[i*FontSize +j] & (0x80>>Loop))
        {
          drawPixel(x + index_x++, y + i, textcolor);
        } 
        else
        {
          drawPixel(x + index_x++, y + i, textbgcolor);
        }
      }
    }
  }
}