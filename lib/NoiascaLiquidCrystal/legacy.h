/*
   Additional methods for backward compatibility
   
   These member functions are not part of the LCD API 1.0
   These member functions will assure backward compatibility to other libraries
   
   2023-01-04 fix leftToRight and others
   2021-08-15

*/

#pragma once

/*
#define LCD_5x10DOTS 0x04 //!< 10 pixel high font mode
#define LCD_5x8DOTS 0x00  //!< 8 pixel high font mode
*/


/*
    \brief Move cursor and shift display without changing DDRAM content
    
    Shifts the displayed frame by one character. Effects all lines
*/
void scrollDisplayLeft() {
  command(cursorshift | cursorshift_displayshift);
}

/*
    \brief Move cursor and shift display without changing DDRAM content
    
*/
void scrollDisplayRight() {
  command(cursorshift | cursorshift_displayshift | cursorshift_right);
}

/*
    \brief shift the display
    
    Shifts the entire display either to the left (I/D = 1) when S is 1. 
    If S is 1, it will seem as if the cursor does not move but the display does.
    Also, writing into or reading out from CGRAM does not shift the display.
    The display does not shift when reading from DDRAM. Also, writing into or reading out from CGRAM does not shift the display.
*/
void leftToRight() {
  entrymodesetFlag |= entrymode_increment;       // fixed 2023-01-04
  command(entrymodeset | entrymodesetFlag);
}

/*
    \brief display shift
    
    Shifts the entire display either to the right (I/D = 0) or to the left (I/D = 1) when S is 1. 
    The display does not shift if S is 0.
    The display does not shift when reading from DDRAM. 
*/
void rightToLeft() {
  entrymodesetFlag &= ~entrymode_increment;      // fixed 2023-01-04
  command(entrymodeset | entrymodesetFlag);
}

/*
    \brief scroll right from cursor 
    
    Increments (I/D = 1) the DDRAM address by 1 when a character code is written into or read from DDRAM. 
    The cursor or blinking moves to the right when incremented by 1.
*/
void autoscroll() {
  entrymodesetFlag |= entrymode_shift;           // fixed 2023-01-04
  command(entrymodeset | entrymodesetFlag);
}

/*
    \brief scroll left from cursor
    
    Decrements (I/D = 0) the DDRAM address by 1 when a character code is written into or read from DDRAM.
    The cursor or blinking moves to the left when decremented by 1.
*/
void noAutoscroll() {
  entrymodesetFlag &= ~entrymode_shift;          // fixed 2023-01-04
  command(entrymodeset | entrymodesetFlag);
}
