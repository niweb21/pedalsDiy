/**************************************************************************
 This is an example for our Monochrome OLEDs based on SSD1306 drivers

 Pick one up today in the adafruit shop!
 ------> http://www.adafruit.com/category/63_98

 This example is for a 128x64 pixel display using I2C to communicate
 3 pins are required to interface (two I2C and one reset).

 Adafruit invests time and resources providing this open
 source code, please support Adafruit and open-source
 hardware by purchasing products from Adafruit!

 Written by Limor Fried/Ladyada for Adafruit Industries,
 with contributions from the open source community.
 BSD license, check license.txt for more information
 All text above, and the splash screen below must be
 included in any redistribution.
 **************************************************************************/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
// The pins for I2C are defined by the Wire-library.
// On an arduino UNO:       A4(SDA), A5(SCL)
// On an arduino MEGA 2560: 20(SDA), 21(SCL)
// On an arduino LEONARDO:   2(SDA),  3(SCL), ...
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3D ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define NUMFLAKES 10 // Number of snowflakes in the animation example

#define LOGO_HEIGHT 16
#define LOGO_WIDTH 16
static const unsigned char PROGMEM logo_bmp[] =
    {0b00000000, 0b11000000,
     0b00000001, 0b11000000,
     0b00000001, 0b11000000,
     0b00000011, 0b11100000,
     0b11110011, 0b11100000,
     0b11111110, 0b11111000,
     0b01111110, 0b11111111,
     0b00110011, 0b10011111,
     0b00011111, 0b11111100,
     0b00001101, 0b01110000,
     0b00011011, 0b10100000,
     0b00111111, 0b11100000,
     0b00111111, 0b11110000,
     0b01111100, 0b11110000,
     0b01110000, 0b01110000,
     0b00000000, 0b00110000};

void setup()
{
  Serial.begin(9600);

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
  {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;)
      ; // Don't proceed, loop forever
  }

  // Show initial display buffer contents on the screen --
  // the library initializes this with an Adafruit splash screen.
  display.display();
  delay(2000); // Pause for 2 seconds

  // Clear the buffer
  display.clearDisplay();

  // Draw a single pixel in white
  display.drawPixel(10, 10, SSD1306_WHITE);

  // Show the display buffer on the screen. You MUST call display() after
  // drawing commands to make them visible on screen!
  display.display();
  delay(2000);
  // display.display() is NOT necessary after every single drawing command,
  // unless that's what you want...rather, you can batch up a bunch of
  // drawing operations and then update the screen all at once by calling
  // display.display(). These examples demonstrate both approaches...

  welcomeMessage();

  int valueVertical = 0;
  int directionVertical = 1;

  int valueHorizontal = 0;
  int directionHorizontal = 1;
  while (true)
  {
    display.clearDisplay();

    drawVerticalLoader(3, 3, valueVertical);
    drawVerticalLoader(16, 3, 0);
    drawHorizontalLoader(48, 31, valueHorizontal);

    display.setTextSize(4);
    display.setTextColor(SSD1306_WHITE);
    display.setCursor(31 + 18, 10);
    display.println(F("DIY"));

    display.display();
    delay(2000);

    if(valueVertical == 7 or valueVertical == 0) {
      directionVertical = -1 * directionVertical;
    }
    valueVertical += directionVertical;

    if(valueHorizontal == 44 or valueHorizontal == -44) {
      directionHorizontal = -1 * directionHorizontal;
    }
    valueHorizontal += directionHorizontal;
  }
}

// Draw a vertical loader 10x58px 
// Value beetween 0 and 7
void drawVerticalLoader(int x, int y, int value)
{
  // left line
  display.fillRect(00 + x, 00 + y, 02, 58, SSD1306_WHITE);

  // right line
  display.fillRect(09 + x, 00 + y, 02, 58, SSD1306_WHITE);

  // upper line
  display.fillRect(02 + x, 00 + y, 08, 02, SSD1306_WHITE);

  // lower line
  display.fillRect(02 + x, 57 + y, 08, 02, SSD1306_WHITE);

  // pixels ladder
  display.drawPixel(x + 02, y + 09, SSD1306_WHITE);
  display.drawPixel(x + 08, y + 09, SSD1306_WHITE);

  display.drawPixel(x + 02, y + 17, SSD1306_WHITE);
  display.drawPixel(x + 08, y + 17, SSD1306_WHITE);

  display.drawPixel(x + 02, y + 25, SSD1306_WHITE);
  display.drawPixel(x + 08, y + 25, SSD1306_WHITE);

  display.drawPixel(x + 02, y + 33, SSD1306_WHITE);
  display.drawPixel(x + 08, y + 33, SSD1306_WHITE);

  display.drawPixel(x + 02, y + 41, SSD1306_WHITE);
  display.drawPixel(x + 08, y + 41, SSD1306_WHITE);

  display.drawPixel(x + 02, y + 49, SSD1306_WHITE);
  display.drawPixel(x + 08, y + 49, SSD1306_WHITE);

  value = value > 7 ? 7 : value;
  value = value < 0 ? 0 : value;

  // pixel values
  for (int i = 1; i < value; i++)
  {
    // 7 => 3
    // 6 => 11
    // 5 => 19
    // ...
    // 2 => 43
    // 1 => 51

    int yValue = (7 - i) * 8 + 3;
    display.fillRect(04 + x, yValue + y, 05, 05, SSD1306_WHITE);
  }
}

// Draw a horizontal loader 93x14px
// Value beetween -44 and 44
void drawHorizontalLoader(int x, int y, int value)
{
  // left line
  display.drawRect(00 + x, 00 + y, 93, 14, SSD1306_WHITE);
  display.drawRect(01 + x, 01 + y, 91, 12, SSD1306_WHITE);

  // pixels ladder
  for (int i = 1; i <= 8; i++)
  {
    display.drawPixel(i * 9 + x, 02 + y, SSD1306_WHITE);
    display.drawPixel(i * 9 + x, 11 + y, SSD1306_WHITE);
  }

  value = value > 44 ? 44 : value;
  value = value < -44 ? -44 : value;
  
  // cursor
  display.drawRect(value + 44 + x, 03 + y, 1, 10, SSD1306_WHITE);
}

 // @TODO mandatory ?
void loop()
{
}

void welcomeMessage()
{
  display.clearDisplay();

  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(29, 10);
  display.println(F("Welcome"));
  display.setCursor(29, 36);
  display.println(F("Pilot !"));
  display.display();
  delay(2000);
}
