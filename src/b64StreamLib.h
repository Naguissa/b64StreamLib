/**
 * Base64 web without terminators basic library.
 *
 * Accepts char aray or char-by-char as arguments.
 *
 *
 * @copyright Naguissa
 * @author Naguissa
 * @email naguissa.com@gmail.com
 * @version 1.0
 * @created 2016-07-24
 */
#ifndef __B64STREAMLIB_H__
	#define __B64STREAMLIB_H__
	
	// Warnng: Max length when datatype is char: 190 -- Because 4:3 ratio on base64 and 255 max value on byte variable.
	#define B64STREAMLIB_defaultInnerBufferLength 128

	// You can use longer buffers switching type to unisgned int, but seems too much for usual cases
	#define B64STREAMLIB_indexDataType unsigned char
	
	#define B64STREAMLIB_STATUS_EMPTY 0
	#define B64STREAMLIB_STATUS_INIT 1
	#define B64STREAMLIB_STATUS_FILLING 2
	#define B64STREAMLIB_STATUS_READY 3
	#define B64STREAMLIB_STATUS_READING 4
	
	

	// yeld() to let ESP8266 do its tasks and not block it.	
	#ifndef EXTRA_YIELD()
		#ifdef ESP8266
		  #define EXTRA_YIELD() yield()
		#else
		  #define EXTRA_YIELD()
		#endif
	#endif
	
	
	
	class b64StreamLib {
		public:
			b64StreamLib();
			
			bool initBuffer(B64STREAMLIB_indexDataType = B64STREAMLIB_defaultInnerBufferLength);
			bool reinitBuffer(B64STREAMLIB_indexDataType = B64STREAMLIB_defaultInnerBufferLength);
			
			// To encode from plain text to base64: Write chars or string and read buffer
			bool writeChar(char);
			bool writeCharEnd();
			bool writeCharArray(char *);
			// Caution: not modify data and take care with internal status; buffer may be NULL
			char * getEncodedBuffer();
			
			// To decode from base64 to plain text: Write buffer and read string or chars
			bool writeEncodedCharArray(char *);
			char readChar();
			bool readToCharArray(char *);

			unsigned char getStatus();
			int getMaxLength();
			B64STREAMLIB_indexDataType getActualLength();


		private:
			// In CPP file you can change maping; by default base64 URL-based is used
			static char _charmap[];
			static unsigned int _maxLength;

			char * _buffer; // Data
			B64STREAMLIB_indexDataType _actualLength = 0; // Position on the buffer
			B64STREAMLIB_indexDataType _position = 0; // Position on the buffer
			unsigned char _status = B64STREAMLIB_STATUS_EMPTY;
			unsigned char _char2val(char);
			
	};
#endif
