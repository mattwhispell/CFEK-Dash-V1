#define ILI9341_DRIVER

                    // Typical board default pins - change to match your board
#define TFT_CS   3 //     10 or 34 (FSPI CS0) 
#define TFT_MOSI 6 //     11 or 35 (FSPI D)
#define TFT_SCLK 8 //     12 or 36 (FSPI CLK)
//#define TFT_MISO 37 //     13 or 37 (FSPI Q)

// Use pins in range 0-31
#define TFT_DC    7
#define TFT_RST   5

//#define TOUCH_CS 16 // Optional for touch screen

#define LOAD_GLCD
#define LOAD_FONT2
#define LOAD_FONT4
#define LOAD_FONT6
#define LOAD_FONT7
#define LOAD_FONT8
#define LOAD_GFXFF

#define SMOOTH_FONT

//  #define TFT_RGB_ORDER TFT_RGB  // Colour order Red-Green-Blue
  #define TFT_RGB_ORDER TFT_BGR  // Colour order Blue-Green-Red

 #define TFT_INVERSION_ON
// #define TFT_INVERSION_OFF

// FSPI (or VSPI) port (SPI2) used unless following defined. HSPI port is (SPI3) on S3.
//#define USE_HSPI_PORT

//#define SPI_FREQUENCY  27000000
#define SPI_FREQUENCY  40000000   // Maximum for ILI9341