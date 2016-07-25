#include <Arduino.h>
#include "b64StreamLib.h"

b64StreamLib toTest = b64StreamLib();

void setup() {
    Serial.begin(9600);
    toTest.initBuffer(64);
  
    toTest.writeChar('H');
    toTest.writeChar('e');
    toTest.writeChar('l');
    toTest.writeChar('l');
    toTest.writeChar('o');
    toTest.writeChar(' ');
    toTest.writeChar('W');
    toTest.writeChar('o');
    toTest.writeChar('r');
    toTest.writeChar('l');
    toTest.writeChar('d');
    toTest.writeChar('!');
    toTest.writeCharEnd();
    Serial.println(toTest.getEncodedBuffer());
    for(char c = toTest.readChar(); toTest.getStatus() == B64STREAMLIB_STATUS_READING; c = toTest.readChar()) {
      Serial.print(c);
    }
    Serial.println();
    Serial.println(toTest.getActualLength());

    Serial.println();
    Serial.println("-----------");
    Serial.println();

    toTest.writeCharArray("Second Hello!!");
    Serial.println(toTest.getEncodedBuffer());
    for(char c = toTest.readChar(); toTest.getStatus() == B64STREAMLIB_STATUS_READING; c = toTest.readChar()) {
      Serial.print(c);
    }
    Serial.println();
    Serial.println(toTest.getActualLength());
    
    Serial.println();
    Serial.println("-----------");
    Serial.println();

  // 1st string, again:
    toTest.writeCharArray("Hello World!");
    Serial.println(toTest.getEncodedBuffer());
    for(char c = toTest.readChar(); toTest.getStatus() == B64STREAMLIB_STATUS_READING; c = toTest.readChar()) {
      Serial.print(c);
    }
    Serial.println();
    Serial.println(toTest.getActualLength());
    
    Serial.println();
    Serial.println("-----------");
    Serial.println();

  // 2nd string, direct load:
    toTest.writeEncodedCharArray("U2Vjb25kIEhlbGxvISE");
    Serial.println(toTest.getEncodedBuffer());
    for(char c = toTest.readChar(); toTest.getStatus() == B64STREAMLIB_STATUS_READING; c = toTest.readChar()) {
      Serial.print(c);
    }
    Serial.println();
    Serial.println(toTest.getActualLength());
    
    Serial.println();
    Serial.println("-----------");
    Serial.println();
    
   
    
    Serial.println(toTest.getMaxLength());
}

void loop() {
  // put your main code here, to run repeatedly:

}
