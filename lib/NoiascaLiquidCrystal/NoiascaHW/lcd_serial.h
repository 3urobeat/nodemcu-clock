/*
  Noiasca Liquid Crystal 2.x - Serial Interface
  
  This class supports a LCD over Serial Interface
  
  Hardware
  - Sureeno SLC1602F http://surenoo.tech/download/01_SLC/SLC1602F.pdf
  
  This classes are for LCD with Serial interface.
  You can build a "Serial LCD Sender" with this hardware implementation, 
  which uses the common LCD-API. Just connect the LCD via a serial line.
  In theory there is no need for a language converter in this "sender"
  But to overcome the flaws in the Serial Display, this sender can also use language converter
  
  This classes translate the default HD44780 commands into serial commands
  
  Serial display brought to my attention by this thread: https://forum.arduino.cc/t/softwareserial-mit-attiny85/1033410/

   
  Remarks:
  - Overall logic is 
    write --> send --> write8bits --> write (to serial)
    or
    command --> send --> write8bits --> write (to serial)
    i.e. 
    - print commands are serialized already by Print.h. 
    - each character is send as single character to the LCD (including the header bytes and the trailer byte) --> 4 bytes
      To overcome this you could implement the Noiasca Buffer Print instead.
    - write8bits must reverse calculate commands based on variables set by command() from the parent class.
    
  - language converters are not in scope of this HW implementation because
    the mapping to character ROMs should be done on receiver side
    
  open topics (features in near future)
  - read keys (this client polls from Server) - will need a state machine with timeout and blocking of other sending
  - parse - read keys (callbacks when Server pushed)
  
  copyright 2022 noiasca noiasca@yahoo.com

  2022-12-11 ok and hardware tested
  2022-09-20 development - experimental 
*/

#pragma once
#include <NoiascaLiquidCrystal.h>
#include <Print.h>

/* ****************************************************************************
    base class for Serial LCD (TX sender)
************************************************************************** */

class LiquidCrystal_Serial_base : public Print, public LiquidCrystal_dummy {
  protected:
    Stream *stream;				                                 // connection to user's chosen serial interface
    uint8_t displaycontrolFlagOld = displaycontrolFlag;    // this display needs the old value to determine, what was changed
    void (*cbPreTransmission)();                           // gets called before writing a message
    void (*cbPostTransmission)();                          // gets called after writing a message
    //using Callback2 = void (*)(uint8_t, uint8_t);        // callback with two parameters
    using Callback1 = void (*)(uint8_t);                   // callback with one parameters 
    Callback1 cbKey;                                       // gets called when a key action was received
    static constexpr byte numChars {8};                    // receiver buffer size (*2 because of UTF-8 characters)
    byte receivedChars[numChars];                          // an array to store the received data
    byte ndx {0};                                          // length of received message
    
    static constexpr uint8_t startMarker {0xAA};           // all Serial commands need a trailing 0xAA 
    static constexpr uint8_t cmdProductReset {0x01};        
    static constexpr uint8_t cmdClearDisplay {0x10};        
    static constexpr uint8_t cmdDisplayOn {0x11};
    static constexpr uint8_t cmdDisplayOff {0x12};
    static constexpr uint8_t cmdSetBrightness {0x13};      
    static constexpr uint8_t cmdSetCursorPosition {0x20};   
    static constexpr uint8_t cmdCursorOn {0x21};
    static constexpr uint8_t cmdCursorOff {0x22};
    static constexpr uint8_t cmdCursorBlinkOn {0x23};
    static constexpr uint8_t cmdCursorBlinkOff {0x24};
    static constexpr uint8_t cmdWritingText {0x25};         
    static constexpr uint8_t cmdDefineCustomCharacter {0x26};       // MISSING: not implemented yet
    static constexpr uint8_t cmdOutputUserPressesTheKeycode {0x55}; // MISSING: not working fine

    void send(uint8_t value, uint8_t rs, uint8_t rw = rwWrite)       // generic
    {
      DEBUG_PRINTLN(F("Serial_base send"));
      if (cbPreTransmission) cbPreTransmission();          // can be used to write HIGH to txEnablePin
      write8bits(value, rs, rw);
      if (cbPostTransmission) cbPostTransmission();        // can be used to write LOW to txEnablePin
      delayMicroseconds(waitshort);    // standard delay after send
    }

    void write8bits(uint8_t value, uint8_t rs = rsIR, uint8_t rw = rwWrite) {  // low level HW access
      DEBUG_PRINTLN(F("Serial_base write8bits"));
      (void)rw;
      
      if (rs == rsDR)                      // data register
      {
        stream->write(startMarker);
        stream->write(cmdWritingText);
        stream->write(value);
        stream->write(0x0D);               // end flag for writing text
        stream->flush();
      }
      else                                 // instruction register
      {
        if (value == cleardisplay)
        {
          stream->write(startMarker);
          stream->write(cmdClearDisplay);
          stream->flush();
        }
        else if ((value & 0b11000000) == setcgramaddress)  // special characaters
        {
          // reverse calculate from command(setcgramaddress | (address << 3));
          byte address = (value & 0b00111111) >> 3;
          stream->write(startMarker);
          stream->write(cmdDefineCustomCharacter);
          stream->write(address);
          stream->flush();          
        }
        else if (value & setddramaddress)  
        {
          stream->write(startMarker);
          stream->write(cmdSetCursorPosition); 
          // reverse Calculate from command(setddramaddress | (rowStartingAddress[newRow] + newCol)); 
          byte row = 0;
          if ((value - setddramaddress) >= rowStartingAddress[1]) row = 1;
          byte col = value - setddramaddress - rowStartingAddress[row];
          stream->write(row);  // the byte order differs from used API
          stream->write(col);
          stream->flush();
        }
        else if ((value & 0b11111000) == displaycontrol)
        {
          // compare the "displaycontrolFlag" with the old flag to determine, what was changed and act accordingly
          if ((displaycontrolFlag & displaycontrol_blink) != (displaycontrolFlagOld & displaycontrol_blink))
          {
            stream->write(startMarker);
            if (displaycontrolFlag & displaycontrol_blink) 
              stream->write(cmdCursorBlinkOn);
            else            
              stream->write(cmdCursorBlinkOff);
            stream->flush();
            displaycontrolFlagOld = displaycontrolFlag;
          }
          else if ((displaycontrolFlag & displaycontrol_cursor) != (displaycontrolFlagOld & displaycontrol_cursor))
          {
            stream->write(startMarker);
            if (displaycontrolFlag & displaycontrol_cursor) 
              stream->write(cmdCursorOn);
            else            
              stream->write(cmdCursorOff);
            stream->flush();
            displaycontrolFlagOld = displaycontrolFlag;
          }
          else if ((displaycontrolFlag & displaycontrol_on) != (displaycontrolFlagOld & displaycontrol_on))
          {
            stream->write(startMarker);
            if (displaycontrolFlag & displaycontrol_on) 
              stream->write(cmdDisplayOn);
            else            
              stream->write(cmdDisplayOff);
            stream->flush();
            displaycontrolFlagOld = displaycontrolFlag;
          }
        }
        else if (value == returnhome)
        {
          stream->write(startMarker);
          stream->write(cmdSetCursorPosition); 
          stream->write(byte(0x00));
          stream->write(byte(0x00));
          stream->flush();
        }
      }
    }
    
    /*
      delete internal buffer
    */
    void delBuffer()
    {
      memcpy(receivedChars, "", sizeof(receivedChars));
      ndx = 0;
    }
  
  public:     
    LiquidCrystal_Serial_base(Stream &stream, uint8_t cols = 16, uint8_t rows = 2) : 
      LiquidCrystal_dummy(cols, rows),
      stream(&stream)
      {
        delBuffer();
      }

    size_t write(uint8_t value) {
      DEBUG_PRINTLN(F("Serial_base write"));
      send(value, rsDR);
      return 1;
    } 

    virtual void hwInit() {
      DEBUG_PRINTLN(F("Serial_base hwInit"));
      // reset
      if (cbPreTransmission) cbPreTransmission();          // can be used to write HIGH to txEnablePin
      stream->write(0xAA);
      stream->write(cmdProductReset);
      if (cbPostTransmission) cbPostTransmission();        // can be used to write LOW to txEnablePin
    }

/**  
    Initializes the display (LCD API: init())
    Clears the screen and puts the cursor to 0, 0
    noiasca: methods are on HW implementation level
             I prefer the begin() methods
             init() will be just an alias for begin() as it is part of LCD API
*/
    
    void begin()                                           // Serial interface
    {
      DEBUG_PRINTLN(F("Serial_base begin"));
      displaycontrolFlagOld = displaycontrolFlag;
      hwInit();                                          // init HW - not needed for Serial
      //clear();   // tbd: the hw implementation of begin() calls a clear(). 
                   //      this is just a sender... therfore currently deactivated      
     }

    void begin(uint8_t cols, uint8_t rows, uint8_t charsize = LCD_5x8DOTS){
      (void)cols;
      (void)rows;
      (void)charsize;
      begin();
    }
      
    void init() {begin();};            // LCD API
    
    /**
      \brief switch backlight on or off
      
      \param value 0 = off; 1 = on
    */  
    int setBacklight(uint8_t value)    // LCD API
    {   
      DEBUG_PRINTLN(F("Serial_base setBacklight"));
      value ? setBrightness(0xFF) : setBrightness(0); 
      return 1; // assume success
    }

    /**
      \brief switch backlight on
    */    
    int backlight() 
    {
      return setBacklight(1);
    }

    /**
      \brief switch backlight off
    */
    int noBacklight()
    {
      return setBacklight(0);
    } 

    /**
        specific for Serial display
    */
    void setBrightness(byte value)
    {
      if (cbPreTransmission) cbPreTransmission();
      stream->write(0xAA);
      stream->write(cmdSetBrightness);
      stream->write(value);
      if (cbPostTransmission) cbPostTransmission();
    }
    
    /**
      \brief action after the last byte was transmitted.

      This callback function gets called after a message has finished sending
      (i.e. after all data has been physically transmitted onto the serial bus).

      Typical usage of this callback is to enable an RS485 transceiver's
      Receiver Enable pin, and disable its driver enable pin.
    */
    void setPostTransmission(void (*cbPostTransmission)())
    {
      (*this).cbPostTransmission = cbPostTransmission;
    }

    /**
      \brief action before the first byte will be transmitted.

      This callback function gets called just before a message is responded over serial.
      Typical usage of this callback is to enable an RS485 transceiver's
      Driver Enable pin, and optionally disable its receiver enable pin.
    */
    void setPreTransmission(void (*cbPreTransmission)())
    {
      (*this).cbPreTransmission = cbPreTransmission;
    }

    /**
      \brief action when a key was pressed

      This callback function gets called when a key gets pressed in Server Push mode.
      \param cbKey the callback function
    */    
    void setKey(Callback1 cbKey)
    {
      (*this).cbKey = cbKey;
    }

   
    /**
      Check serial interface if something is to be readed and parsed.

      \note In this sketch we do an imidiately interpretation of the incoming data as the protocol has no end marker
    */
    void parse()
    {
      int rc = stream->read();
      if (rc >= 0)
      {
        receivedChars[ndx] = rc;
        Serial.print(receivedChars[ndx], HEX); // MISSING - nur debug
        ndx++;

        //debugDataHex();
        if (ndx == 1 && receivedChars[0] != startMarker)
        {
          // DEBUG_PRINTLN(F("throw away corrupt data")); debugDataHex();
          delBuffer();
        }
        else if (ndx == 2)
        {
          if (receivedChars[1] == cmdOutputUserPressesTheKeycode)
          {
            //Serial.print(F("D309 "));                              //MISSING geht noch nicht
            //Serial.print(receivedChars[2]);
            //Serial.print(" ");
            //Serial.print(receivedChars[3]);
            //Serial.println();
            if (cbKey) cbKey(receivedChars[2]);  
            delBuffer();
          }
        }
        // sanity check
        if (ndx >= numChars) delBuffer();
      }
    }


};

// variants removed (development base was "lcd_wire")

/* ****************************************************************************
    no class for custom pin assignment
   ************************************************************************** */

/* ****************************************************************************
    no class for 4004 display
    (not seen so far)
   ************************************************************************* */
   