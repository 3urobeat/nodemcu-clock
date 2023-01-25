/*
  Noiasca Liquid Crystal - 2.x
  
  copyright 2022 noiasca noiasca@yahoo.com
  
  Version
  2023-01-07 fix entrymodesetFlag
  2022-07-28 comments
  2022-03-06 added support for any I2C interface (TwoWire)
  2022-01-15 comments for doxygen
  2021-05-29 fixes für ESP8266, ESP32
  2021-05-17
  2021-03-13 started new development
*/

#pragma once
#include <Arduino.h>

//#define DEBUG_NLC    // activates debug messages

#ifdef DEBUG_NLC
  #define DEBUG_PRINTLN(x)  Serial.println(x)
#else
  #define DEBUG_PRINTLN(x)
#endif

//#include <Print.h>

// the converters
#include <utility/NoiascaUTF8.h>                 // contains the plain UTF8 to ASCII mapping
#include <utility/NoiascaConverter.h>            // all converters
#include <utility/NoiascaCustomCharacters.h>     // some predefined custom characters

// backward compatibilty 
// for begin method:
#define LCD_5x10DOTS 0x04                        // F  5x10
#define LCD_5x8DOTS 0x00                         // F  5x7
typedef enum {POSITIVE, NEGATIVE, FAULTY} t_backlightPol; // compatible to the very common New Liquid Crystal


/**
   \brief LCD parent for all classes.
   
   This is the parent class for all LCD hardware implementations.
*/
class LiquidCrystal_dummy {
//class LiquidCrystal_dummy : public Print {
  protected:
    // Table 6 Instructions p24
    static const uint8_t cleardisplay = 0x01;
    static const uint8_t returnhome = 0x02;
    static const uint8_t entrymodeset = 0x04;
    static const uint8_t displaycontrol = 0x08;
    static const uint8_t cursorshift = 0x10;
    static const uint8_t functionset = 0x20;
    static const uint8_t setcgramaddress = 0x40;           // characters
    static const uint8_t setddramaddress = 0x80; 
    // entry mode set 0x04
    static const uint8_t entrymode_shift = 0x01;           // S Shift
    static const uint8_t entrymode_increment = 0x02;       // I/D Increment/Decrement  
    // Display on/off controll 0x08                        
    static const uint8_t displaycontrol_blink = 0x01;      // B blinking cursor position
    static const uint8_t displaycontrol_cursor = 0x02;     // C cursor on
    static const uint8_t displaycontrol_on = 0x04;         // D display on
    // Curosr or display shift 0x10                        
    static const uint8_t cursorshift_right = 0x04;         // R/L Right Shift / Left Shift
    static const uint8_t cursorshift_displayshift = 0x08;  // S/C Display Shift / Cursor Move
    // function set 0x20                                   
    static const uint8_t functionset_font = 0x04;          // F sets the character font to 5x10  (5x7 is 0x00)
    static const uint8_t functionset_lines = 0x08;         // N Sets the number of display lines - 2 line mode
    static const uint8_t functionset_8bit = 0x10;          // DL sets the interface data length (8bit)
  
    // might be changed on instance level (one day...)
    const uint8_t waitshort = 37;                          // datasheet 37us 
    const uint16_t waitlong = 1500;
    uint8_t rowStartingAddress[4] {0x00, 0x40, 0x14, 0x54};// variables to enable modification for 16x4 and 40x4
    const uint8_t cols;                                    // real LCD colums - initilized with constructor
    const uint8_t rows;                                    // real LCD rows   - initilized with constructor
    uint8_t currentRow = 255;                              // keep track of the actual row, needed for 4004 
    
    // separate flag as there are several attributes to be set in a single method according to the LCD API (without the function set bit itself
    uint8_t entrymodesetFlag = 0;                          // I/D S
    uint8_t displaycontrolFlag = displaycontrol_on;       
    //uint8_t cursorshiftFlag = 0;                         // S/C R/L
 
    uint32_t special = 0;                                  // store special character 1680/64  --> move down to converter classes only
    
    // Pin control constants
    static const byte rsIR = 0;                            // register select: Instruction Register (commando)
    static const byte rsDR = 1;                            // register select: Data Register
    static const byte rwWrite = 0;                         // read/write WRITE
    static const byte rwRead = 1;                          // read/write READ
    
/* 
    must be implemented in HW class because classes differntiate between 4bit and 8bit
    virtual because other methods use send and so we need a late binding
    to make it compile for esp8266, esp32 we need it in the base class
*/ 
    
    virtual void send(uint8_t, uint8_t, uint8_t = rwWrite);
    //virtual void send(uint8_t, uint8_t, uint8_t = rwWrite) = 0; // avoids link error for esp8266 https://stackoverflow.com/questions/9406580/c-undefined-reference-to-vtable-and-inheritance
  
    //testwise local implementation
    //void send(uint8_t, uint8_t);
    //virtual void setRegisterSelect(uint8_t);
    //virtual void write4bits(uint8_t);
    
  public:
    LiquidCrystal_dummy(uint8_t, uint8_t);                 // constructor
    
    //virtual ~LiquidCrystal_dummy(); 

/*  
    init()
    LCD API: Initializes the display.
    Clears the screen and puts the cursor to 0, 0
    noiasca: begin() methods are on implementation level
             and therefore this alias also
*/
    
/* 
    \brief Lets you override the static delays in the library (LCD API)
    
    Some displays require delays to allow command to complete
    Typically there are two different delays.
    One for LCD commands, and one when sending characters to the display.
    The library should set the default for the display.
    <br>noiasca: only empty alias
             no implementaion in this library, 
             but some classes might use different delays if necessary.
*/
    void setDelay(uint8_t, char);

/*
    \brief Raw Write a value to the display (LCD API)
    
    noiasca: the write is defined in the HW implementation
*/    
//  virtual size_t write(uint8_t);
//  size_t write(uint8_t); 
    
/**
    \brief unmodified version of write
    
    noiasca: behaves like the original write 
*/
    size_t writeOld(uint8_t);                              // legacy write of special characters >=0xC0

/**
    \brief Send a command to the display, for commands not supported by the library (LCD API)
    
    noiasca: this is a "transmit to LCD" with "Register select" = 0 Instruction Register
    (so something like a data command in contrast with 1 Data Register doesn't exist officially)
*/
    void command(uint8_t);

/**  
    \brief Clear the display and place cursor at 0,0 (LCD API)
    
    Clears the display and sets the cursor to 0,0.
*/
    void clear();  
    
/**  
    \brief Home the cursor to 0,0 and leave displayed characters (LCD API)
    
    Sets the cursor to 0,0.
*/
    void home();

/**
    \brief Where Row 0-MAXLINEs, and Col 0-MAXCOLUMNS
    
    noiasca: every common LCD implementation is column - row, therefore also in this library.
    @param newCol column the new column (0..39)
    @param newRow the new row (0..3)
*/
    void setCursor(uint8_t, uint8_t);
    
/**
    \brief Turn the block cursor on
    
    noiasca: LCD API defines this as cursor_on()
*/
    void cursor();
    void cursor_on() {cursor();};      // alias for LCD API
    
/** 
    \brief Turn the block cursor off
    
    noiasca: LCD API definies this as cursor_off()
*/
    void noCursor();
    void cursor_off() {noCursor();};   // alias for LCD API

/**
    \brief Turn on the blinking underline cursor (LCD API blink_on)
    
    noiasca: LCD API definies this as cursor_on()
*/
    void blink();
    void blink_on() {blink();};        // alias for LCD API
    
/**  
    \brief Turn off the blinking underline cursor (LCD blink_off)
    
    noiasca: LCD API definies this as noBlink()
*/
    void noBlink();
    void blink_off() {noBlink();};     // alias for LCD API
    
// Optional Functions:
// Should be included in the library, but might not be functional 

/*
    \brief Set the backlight off/on, or set Backlight brightness (0-255) (LCD API optional)
    
    If the display only has the option to turn the backlight on and off: 0 = off, >0 = on
    noiasca: implemented in the HW classes as not all HW classes support this command
    
    setBacklight(val)
*/

/* 
    \brief Set the contrast value of the display (0-255) (LCD API optional)
    
    noiasca: empty alias only
*/
    void setContrast(int);

/** 
    \brief Turn the LCD display on (LCD API optional)
    
    If the display does not have an option to turn on/off just turn backlight on/off.
    In this library it is used to control the display control flag.
*/
    void on();

/** 
    \brief Turn the LCD display off (LCD API optional)
    
    In this library it is used to control the display control flag.
*/
    void off();

/*    
    \brief Return the status of the display (LCD API optional)
    
    Returns the FIFO buffer on the robot-electronics display
    Can be used to get r/w status of hardwired displays
    
    status()
*/

// Extend Functions:
// Things that are specific to a particular display 

/**
    \brief Load up a custom character (LCD API extend)
    
    original description is: Load up a custom character [0-7] 
    *Rows is an array of 7 bytes that contain bitmap
    To send custom character to display use lcd.write(0-7);
    See sample sketch on how to implement this.
    
    noiasca: Most libraries use "createChar" instead.
             So the LCD API name load_custom_character is used as alias only.
             this library expects a 5x8(!) character - not 7 bytes like the LCD API description.
*/

    void createChar(const uint8_t, const uint8_t[]);       // all major libraries use this method name 
    void load_custom_character(const uint8_t Char_Num , const uint8_t Rows[]) {createChar(Char_Num, Rows);};

/**
    \brief Load up a custom character from PROGMEM 
    
    noiasca: this function loads a special character from PROGMEM)
             this library expects a 5x8(!) character - not 7 bytes like the LCD API description
*/
    void createChar_P(const uint8_t, const uint8_t *);     // the implementation for custom characters in PROGMEM      
   
/*
    \brief Read value from keypad (LCD API extend)
    
    Keypad page
    noiasca: no implementation
    keypad()
*/

/*
    \brief Print a string without delay. (LCD API extend)
    
    noiasca: no implementation it seems to be a very old print command not needed.
    printstr(Str)
*/

// additional methods (not from LCD API)
/**
    \brief creates 3 German Umlauts using special characters 5, 6, and 7
    
    noiasca: this helper function creates the 3 German Umlauts Ä Ö Ü using special characters.
             It occupies the characters 5, 6, 7.
*/
    void createUml();
    
/**
  \brief sets the used font
  
  some OLED displays support extended commands
  which is used for this command
  @param value the new font
*/    
    void setFont(uint8_t value = 0);
    
/*
  there are some legacy methods which are included for backward compatibilty
*/
#include "legacy.h"

};


/*! \mainpage Some words to the Noiasca Liquid Crystal
 
  \section intro_sec Introduction
 
  This is a short description of the Noiasca Liquid Crystal.
  
  The library supports several hardware connections to the LCD: <br>
  - directly connected diplays (7 GPIOs in 4bit Mode), <br>
  - I2C port expander PCF8574, <br>
  - I2C port expander MCP23017, <br>
  - displays with SPI portexpander like the MCP23S08, <br>
  - displays with native I2C support like Grove or Sureeno inkl. the RGB backlight. <br>

  The Noiasca Liquid Crystal supports different display sizes (1x8, 1x16, 2x16, 2x20, 2x40, 4x40). Yes the 4004 are also included <br><br>
  Supported LED driver chips <br>
  - HD44810 this version uses the built in character Set A00 (English - Japanese)<br>
  - AIP31068 - driver with native I2C support <br>
  - SPLC780D1 with the European ROM 003A (Latin 1 - 2)<br>
  - SPLC780D1 with the Russian ROM 002A (Cyrillic)<br>
  - ST7070 for Latin 1 - 4 and Cyrillic <br>
  - OLED LED 1621 display with full support of all 4 fonts<br>
  
  The API/the interface is based on the official LCD API 1.0 and therefore compatible to the most common libraries like the Liquid Crystal or Adadruit LiquidCrystal I2C Version 1.1.2 from Frank de Brabander. <br>
  
\section purpose_sec Main Purpose

  This library extends the easy usage for special characters - German ä ö ü ß and a lot of Greek letters used for symbols in mathematics sent with print/println in UTF-8. 
  In general, a UTF-8 character can be displayed if it is available in the character set of the LCD.
  Consider following usecase: you want to print the temperature on your LCD. 
  Without the usage of special characters and without the usage of octal codes 
  you just print your characters to the LCD:
  <pre>lcd.print("12.34 °C");</pre>
  or you could use the micro sign:
  <pre>lcd.print("34 µV");</pre>
  And these characters will be printed on the LCD.
 
  \section install_sec Installation
  
  \subsection step0 Download the library
  The newest version of this library can be downloaded from http://werner.rothschopf.net/202009_arduino_liquid_crystal_intro.htm .
 
  \section example_sec Examples
  
  There are two groups of examples: For each hardware type you will find a specific strand test / hello world.
  Allways start with the simple sketch fitting to your hardware.
  
  For how to use UTF-8 converters see the examples in the folder 50. 
  There are examples for most of the European languages, but also for examples how to use Cyrillic or Japanese (halfsize) Katakana.
  
  \subsection step1 Install the library
 
  In the Arduino IDE use the Menu <br>
   Sketch / Include Library / Add .ZIP Library <br>
  to install the library.
  
 */
