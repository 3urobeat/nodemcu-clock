/*
   Noiasca Liquid Crystal - Move.h
   
   This file contains 
   - Class to add scroll and shift movements to a LCD
  
   It is a separate file, because these mappings are very specific to LCD Hardware 
   
   copyright 2022 noiasca noiasca@yahoo.com
   
   todo: 
   - make canvasStart != 0 working

   Version
   2022-09-10 initial version
 */
 
#pragma once
#include <NoiascaLiquidCrystal.h>

#define  DEBUG  1     // Turn on debug statements to the serial output  

#if DEBUG
#define DEBUG_P(...)       Serial.print(__VA_ARGS__);
#define DEBUG_PLN(...)     Serial.println(__VA_ARGS__);
#else
#define DEBUG_P(...)
#define DEBUG_PLN(...)
#endif


/** 
   \brief a class for the buffer
   
   to receive characters into a buffer if called with a "print"
   and send characters to a LCD object
*/
template<class T>
class MyBuffer : public Print
{
  protected:
    T &lcd;                             // a reference to the LCD object
    const byte canvasStart;             // canvas: start column of used LCD area
    const byte canvasLength;            // canvas: length of used LCD area
    const byte currentRow;              // the row on the LCD
    // variables for the buffer
    static const byte buffersize = 48;  // size of internal buffer, largest size of a LCD + some space for UTF-8 multibyte characters
    char internal[buffersize];          // the internal buffer for the scroll text
    byte indexWrite = 0;                // next write position in buffer, implicit the current length of buffer
    byte indexRead = 0;                 // next start position for read - used in case the buffer will not get leftshift / destroyed
    // variables for the movement
    uint32_t previousMillis;            // time management scroll
    uint16_t interval = 500;            // milliseconds between each shift/scroll
    
    byte moveCursor = 0;                // start position within canvas?
    
    bool moveKeep = false;              // keep or delete buffer after last print
    byte moveCount = 0;                 // actual position for movements
    byte leaveChars = canvasLength - canvasStart;      // how many characters should be left on the scroll arear / 0 or canvasStart = ganz links rausshiften
    byte moveStartPos = 0;              // start of movement: 0 = left / canvasLength-1 = rechts - within scrollArea
    bool moveFinalChar = false;         // flag if last char was printed already

  public:
    MyBuffer (T &lcd, const byte canvasStart, const byte canvasLength, const byte currentRow) :
      lcd(lcd), canvasStart(canvasStart), canvasLength{canvasLength}, currentRow{currentRow}
    {
      clear();     // init buffer
    }

    /*
        \brief write for Print.h
        
        adds written value to internal buffer
    */
    size_t write (uint8_t value)
    {
      if (indexWrite < buffersize - 1)
      {
        internal[indexWrite] = value;
        indexWrite++;
        internal[indexWrite] = '\0';
        moveFinalChar = false;
      }
      return 1;
    }

    /**
        \brief clear the internal buffer

    */
    void clear()
    {
      memset(internal, '\0', buffersize);
      indexWrite = 0;
      indexRead = 0;
      moveCount = 0;
    }

    /**
        \brief delete the defined scroll on LCD

        deletes the defined scroll area on the LCD
        \param c a character which should be used to "delete", default = ' ' (blank)
    */
    void clearCanvas(const char c = ' ')
    {
      lcd.setCursor(canvasStart, currentRow);
      for (byte i = 0; i < canvasLength; i++) lcd.print(c); // clear Canvas area
      lcd.setCursor(canvasStart, currentRow);
    }

    /**
        \brief set interval of movement

    */
    void setInterval(uint16_t interval)
    {
      this->interval = interval;
    }    
    
    /**
        \brief set the characters to be left on the display

        Set the remaining characters on the display when the scrolling has finished.
        \param chars the characters to be left on the display (0 or available colums, i.e. 16)
    */
    void setLeaveChars(byte chars)
    {
      if (chars <= canvasLength)
        leaveChars = chars;
      else
        leaveChars = canvasLength;
    }

    /**
        \brief set the start position for the scrolling

        Set the position of the first character to be scrolled on the display.
        \param pos the display position (0 or last position e.g. 15)
    */
    void setMoveStartPos(byte pos)
    {
      if (pos > 40)          // hardcoded to 40 as display size is not known to the buffer class
        moveStartPos = 16;
      else
        moveStartPos = pos;
    }

    /**
        \brief keep or delete internal buffer

        Set the position of the first character to be scrolled on the display.
        \param value true will keep the internal buffer, false will delete characters which are shifted out to the left
    */
    void setMoveKeep(bool value = true)
    {
      moveKeep = value;
    }

    /**
        \brief Set mode to Scroll the internal buffer to left

        Scroll the internal buffer endless from right to left.
        This helper functio ensures that the user sets alls parameters correct.
    */
    void setScrollLeft()
    {
      setLeaveChars(0);
      setMoveStartPos(canvasLength - 1); // was 16
      setMoveKeep(true);
      
      moveCursor = canvasLength + canvasStart - 1;
    }

    /**
        \brief Set mode to Shift the internal buffer to left

        Output wider than the display size will be shifted character by character until the last character was shown on the display
        This helper functio ensures that the user sets alls parameters correct.
    */
    void setShiftLeft()
    {
      setLeaveChars(canvasLength - canvasStart);   // was 16
      setMoveStartPos(canvasLength - canvasStart - 1); // was 15
      setMoveKeep(false);
    }
    
    /*
        internal helper function, displays several chars
        von indexRead
        ab fromCol
        bis toCol
        i start index within internal buffer
        \return the written bytes (could be more than 1 due to UFT-8 multibyte characters)
    */
    byte output(byte fromCol, byte toCol, byte i)
    {
      byte shiftBytes = 1;                       // how many bytes should be shifted at the end
      byte pos = fromCol;                        // position on LCD
      DEBUG_P("from:"); DEBUG_P(fromCol);
      DEBUG_P(" toCol:"); DEBUG_P(toCol);
      DEBUG_P(" startindex:"); DEBUG_P(i); DEBUG_P("\n");
      lcd.setCursor(pos, currentRow);
      while (pos <= toCol && i < indexWrite)     // display chars as long as available on display
      {
        lcd.write(internal[i]);
        DEBUG_P(internal[i]);
        switch (internal[i] & 0b11111000)        // check if character is a UTF-8 multi character
        {
          case 0b11000000 :
            lcd.write(internal[++i]);
            DEBUG_P(internal[i]);
            if (pos == 0) shiftBytes = 2;
            break;
          case 0b11100000 :
            lcd.write(internal[++i]);
            DEBUG_P(internal[i]);
            lcd.write(internal[++i]);
            DEBUG_P(internal[i]);
            if (pos == 0) shiftBytes = 3;
            break;
          case 0b11110000 :
            lcd.write(internal[++i]);
            DEBUG_P(internal[i]);
            lcd.write(internal[++i]);
            DEBUG_P(internal[i]);
            lcd.write(internal[++i]);
            DEBUG_P(internal[i]);
            if (pos == 0) shiftBytes = 4;
            break;
        }
        pos++;                         // one display position used
        i++;                           // next read position
      }
      if (pos < canvasLength) lcd.write(' '); //delete trailing character
      DEBUG_P("|\n");                  // the pipe | is just to indicate the end of the buffer
      return shiftBytes;
    }

    /*
    move according the internal parameters by one step
    */
    void moveOld()
    {
#if DEBUG
      // debug information in line 1:
      lcd.setCursor(8, 1); lcd.write('S'); lcd.print(moveStartPos); lcd.write('L'); lcd.print(leaveChars); lcd.write('K'); lcd.print(moveKeep);
#endif
      byte shiftBytes = 1;             // how many bytes should be shifted at the end
      byte index = 0;                  // start position to read from internal buffer
      if (moveKeep) index = indexRead; // 

      if (indexWrite > leaveChars + 1) // ok when internal buffer longer or shorter than cols
      {
        if (moveStartPos > canvasStart && canvasLength - moveCount - canvasStart > 1)
        {
          //DEBUG_PLN(F("D254"));
          shiftBytes = output(canvasLength - moveCount - 1, canvasLength - 1, index);
          moveCount++;
        }
        else
        {
          //DEBUG_PLN(F("D260"));
          shiftBytes = output(canvasStart, canvasLength - 1, index);
          moveCount++;
          if (moveKeep)
          {
            indexRead += shiftBytes;
            if (indexRead > indexWrite)
            {
              indexRead = 0;
              moveCount = 0;
            }
          }
          else
          {
            memmove(internal, internal + shiftBytes, indexWrite - shiftBytes); // cut one (or more) bytes
            internal[indexWrite - shiftBytes] = '\0';
            indexWrite -= shiftBytes;
          }
        }
      }
      else if (indexWrite > 0 && leaveChars == 0)          // "print" if internal buffer is equal or less than columns and delete buffer
      {
        DEBUG_PLN(F("D203"));
        lcd.setCursor(canvasStart, currentRow);
        //lcd.print(internal);
        output(canvasStart, canvasLength - 1, index);
        clear();
      }
      else if (leaveChars > 0 && indexWrite > 0 && indexWrite <= canvasLength) // eventuell nur im Destroy
      {
        DEBUG_PLN(F("D210"));
        if (!moveKeep) {
          DEBUG_PLN(F("D212"));
          lcd.setCursor(canvasStart, currentRow);
          output(canvasStart, canvasLength - 1, index);
          clear();
        }
      }
      else if (leaveChars == 0 && indexWrite == 0 && moveFinalChar == false)   // delete last remaining character
      {
        DEBUG_PLN(F("D289"));
        lcd.setCursor(canvasStart, currentRow);
        lcd.write(' ');
        moveFinalChar = true;
      }
    }
    
    // size_t strlcpy( char *dst, const char *src, size_t siz);
    size_t utf8cpy(char *dst, const char *src, size_t siz)
    {
      size_t i = 0; // total bytes added to src
      size_t c = 0; // total characters processed from dst

      while (src[i] != '\0' && c < siz)
      {
        dst[i] = src[i];
        //Serial.println(byte(dst[i]), HEX);
        if (byte(dst[i]) >= 0b11000000) 
        {
          i++;
          dst[i] = src[i]; 
          if (byte(dst[i]) >= 0b11100000) 
          {
            i++;
            dst[i] = src[i];
            if (byte(dst[i]) >= 0b11100000) 
            {
              i++;
              dst[i] = src[i];
            }
          }
        }
        if (src[i] == '\0') break;
        //if (i >= siz) break;        
        i++; 
        c++;        
      }
     
      
      if (i < buffersize) dst[i]= '\0';         // If the destination buffer still has room.
      else dst[-1] = '\0';                      // We ran out of room, so zero out the last char (if the destination buffer has any items at all).
      
      DEBUG_P(F("angefordert=")); DEBUG_PLN(siz);
      DEBUG_P(F("c =")); DEBUG_PLN(c);
      DEBUG_P(F("return i=")); DEBUG_PLN(i);
      return i;
    }
    
    // Start left, leave 0
    void move()
    {
#if DEBUG
      // debug information in line 1:
      lcd.setCursor(6, 1); lcd.write('S'); lcd.print(moveStartPos); lcd.write('L'); lcd.print(leaveChars); lcd.write('K'); lcd.print(moveKeep); lcd.write('M'); lcd.print(moveCursor);
#endif
      
      char temp[buffersize];
      if (moveCount == 0)
      {
        this->clearCanvas();
      }
      byte lcdPos = 0;
      if (moveCount < canvasLength)
      {
        DEBUG_PLN(F("D329 start"));          // start shift in from right to left  
        byte charsToPrint = moveCount + 1;
        lcdPos = canvasStart + canvasLength - 1 - moveCount;
        utf8cpy(temp, internal, charsToPrint);
        lcd.setCursor(lcdPos, currentRow);   
      }
      else
      {
        if (strlen(internal) > canvasLength)
        {
          DEBUG_PLN(F("D340 rest longer"));      // remaining text is longer than canvas 
          byte charsToPrint = canvasLength;
          utf8cpy(temp, internal + moveCount - canvasLength + 1, charsToPrint);
          this->clearCanvas();                   // not nice, but needed for UTF-8 characters
          lcdPos = canvasStart;
          lcd.setCursor(lcdPos, currentRow);
        }
        else
        {
          DEBUG_PLN(F("D348 rest shorter"));     // remaining text is shorter then canvas
          byte charsToPrint = canvasLength;      // can be shorter
          utf8cpy(temp, internal + moveCount - canvasLength + 1, charsToPrint);
          this->clearCanvas();                   // not nice, but needed for UTF-8 characters
          lcdPos = canvasStart;
          lcd.setCursor(lcdPos, currentRow);
        }
      }
      //DEBUG_P(F(" moveCount=")); DEBUG_P(moveCount);
      //DEBUG_P(F(" lcdPos=")); DEBUG_P(lcdPos);
      //DEBUG_PLN();   
      DEBUG_PLN(temp);
      lcd.print(temp);
      
      if (strlen(temp) < canvasLength) lcd.print(' ');     // delete old character - will only work for left shift and left scroll
      
      moveCount++;
      //if (moveCount >= strnlen_s(internal, buffersize)) // since C11 - but not working
      if (moveCount > strlen(internal) + canvasLength) 
      {
        DEBUG_PLN(F("D376 reset moveCount"));
        moveCount = 0;
      }
    }
 
  
    /**
        \brief update the display line

        Should be called in your loop over and over again.
        Will call the internal move function to make the scrolling/shifting visible.
    */
    void update(uint32_t currentMillis = millis())
    {
      if (currentMillis - previousMillis > interval)
      {
        previousMillis = currentMillis;
        move();
      }
    }
};
