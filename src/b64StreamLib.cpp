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
#include <Arduino.h>
#include "b64StreamLib.h"

// Nothing to do on declaration
b64StreamLib::b64StreamLib() { }


// STATIC VARIABLES

// Base64 URL-oriented
char b64StreamLib::_charmap[64] = {
	'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z',
	'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
	'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', '-', '_'
};
// Calculate max (external) array size depending on index datatype
unsigned int b64StreamLib::_maxLength = 3 * (((   (1 << (sizeof(B64STREAMLIB_indexDataType) * 8 - 1)) - 1) << 1) + 1) / 4;



// FUNCTIONS

/**
 * Initializes internal buffer
 *
 * @param len int Length of internal buffer.
 */
bool b64StreamLib::initBuffer(B64STREAMLIB_indexDataType len) {
	if (_status == B64STREAMLIB_STATUS_EMPTY) {
		_actualLength = len;
		_buffer = (char *) malloc((len+1) * sizeof(char));
		_position = 0; // Position on the buffer
		_status = B64STREAMLIB_STATUS_INIT;
		return true;
	}
	return false;
}
	
/**
 * Clears old buffer and initializes it again.
 *
 * @param len int Length of internal buffer
 */
bool b64StreamLib::reinitBuffer(B64STREAMLIB_indexDataType len) {
	if (_status != B64STREAMLIB_STATUS_EMPTY) {
		_status = B64STREAMLIB_STATUS_EMPTY;
		free(_buffer);
	}
	return this->initBuffer(len);
}


/**
 * Given a char from base64 dictionary returns its numeric value.
 *
 * @param c unsigned char value corresponding to base64 char decoded; 0 if error
 */
unsigned char b64StreamLib::_char2val(char c) {
	EXTRA_YIELD();
	if (c == _charmap[63]) {
		return 63;
	}
	if (c == _charmap[62]) {
		return 62;
	}
	if (c >= 'A' && c <= 'Z') {
		return (c - 'A');
	}
	if (c >= 'a' && c <= 'z') {
		return (c - 'a' + 26);
	}
	if (c >= '0' && c <= '9') {
		return (c - '0' + 52);
	}
	return 0;
}
	

/**
 * Encode a character and add it to internal buffer.
 *
 * CAUTION: When a character is not fully complete buffer stores its partial numerical value, not coded value.
 * It's done for performance reason. writeCharEnd fixes this.
 *
 * @param c char Character to be encoded and added
 */
bool b64StreamLib::writeChar(char c) {
	EXTRA_YIELD();
	if (_status == B64STREAMLIB_STATUS_EMPTY) {
		b64StreamLib::initBuffer(B64STREAMLIB_defaultInnerBufferLength);
	}
	if (_position >= _actualLength) {
		return false;
	}
	if (_status == B64STREAMLIB_STATUS_INIT || _status == B64STREAMLIB_STATUS_FILLING) {
		_status = B64STREAMLIB_STATUS_FILLING;
		switch(_position % 4) {
			case 0: // Next will be 1
				_buffer[_position] = _charmap[c >> 2];
				_position++;
				_buffer[_position] = (c % 4) << 4;
				break;
			
			case 1: // Next will be 2
				_buffer[_position] = _charmap[(c >> 4) + _buffer[_position]];
				_position++;
				_buffer[_position] = (c % 16) << 2;
				break;
			
			case 2:  // Next will be 4 -> 0; no case "3".
				_buffer[_position] = _charmap[(c >> 6) + _buffer[_position]];
				_position++;
				_buffer[_position] = _charmap[c % 64];
				_position++;
				break;
		}
		return true;
	}
	return false;
}
			

/**
 * Changes status to READY, instead FILLING.
 *
 * Marks variable as already filled.
 */
bool b64StreamLib::writeCharEnd() {
	if (_status == B64STREAMLIB_STATUS_FILLING) {
		if (_position % 4) {
			// Not at full 3 chars group (4 chars at buffer)
			_buffer[_position] = _charmap[_buffer[_position]];
			// Add terminator
			_buffer[_position + 1] = '\0';
		} else {
			// Add terminator
			_buffer[_position] = '\0';
		}
		_status = B64STREAMLIB_STATUS_READY;
		_position = 0;
		return true;
	}
	return false;
}


// TEST:
char * b64StreamLib::getEncodedBuffer() {
	return _buffer;
}





/**
 * Decode a character and from internal buffer and move pointer to next one
 *
 * @return char Decoded character
 */
char b64StreamLib::readChar() {
	char r;
	EXTRA_YIELD();
	if (_status != B64STREAMLIB_STATUS_READY && _status != B64STREAMLIB_STATUS_READING) {
		return 0;
	}
	// Pre-read test
	if (_position >= _actualLength || _buffer[_position] == '\0') {
		_status = B64STREAMLIB_STATUS_READY;
		_position = 0;
		return '\0';
	}
	_status = B64STREAMLIB_STATUS_READING;
	switch(_position % 4) {
		case 0: // Next will be 1
			_position++;
			r = ((_char2val(_buffer[_position-1]) << 2) | (_char2val(_buffer[_position]) >> 4));
			break;
	
		case 1: // Next will be 2
			_position++;
			r = (((_char2val(_buffer[_position-1]) % 16) << 4) | (_char2val(_buffer[_position]) >> 2));
			break;
	
		case 2:  // Next will be 4 -> 0; no case "3".
			_position += 2;
			r = (((_char2val(_buffer[_position-2]) % 4) << 6) | _char2val(_buffer[_position-1]));
			break;
	}
	// After-read test
	if (_position >= _actualLength || _buffer[_position] == '\0') {
		_status = B64STREAMLIB_STATUS_READY;
		_position = 0;
	}
	return r;
}

/**
 * Returns status code
 *
 * @return unsigned int Status (check agains constants)
 */
unsigned char b64StreamLib::getStatus() {
	return _status;
}

/**
 * Returns calculated max length
 *
 * @return int Max length
 */
int b64StreamLib::getMaxLength() {
	return _maxLength;
}


/**
 * Returns actual buffer length
 *
 * @return int Actual buffer length
 */
B64STREAMLIB_indexDataType b64StreamLib::getActualLength() {
	return _actualLength;
}



/**
 * Encodes a whole char array
 *
 * @param char* Char array to be encoded
 * @return bool True on success; false on failure
 */
bool b64StreamLib::writeCharArray(char * b) {
	int len = strlen(b);
	if (len > _maxLength) {
		return false;
	}
	reinitBuffer(len * 4 / 3 + 1);
	for (B64STREAMLIB_indexDataType idx = 0; idx < len; idx++) {
		EXTRA_YIELD();
		writeChar(b[idx]);
	}
	writeCharEnd();
	return true;
}


/**
 * Decodes whole buffer in a char array pointer.
 *
 * If pointer is NULL it reserves needed memory to store data.
 *
 * @param char* Char array to store decoded variable
 * @return bool True on success; false on failure
 */
bool b64StreamLib::readToCharArray(char * b) {
	B64STREAMLIB_indexDataType bl;
	if (_status != B64STREAMLIB_STATUS_READY) {
		return false;
	}
	bl = strlen(_buffer);
	if (b == NULL) {
		b = (char *) malloc(sizeof(char) * (int) (bl * 3 / 4 + 1));
	}
	B64STREAMLIB_indexDataType idx;
	for (idx = 0; _status == B64STREAMLIB_STATUS_READING; idx++) {
		EXTRA_YIELD();
		b[idx] = readChar();
	}
/*
	if(idx > 0) {
		b[idx - 1] = '\0';
	}
*/
	return true;
}


/**
 * Stores an encoded array in buffer and set correct status
 *
 * @param char* Char encoded array
 * @return bool True on success; false on failure
 */
bool b64StreamLib::writeEncodedCharArray(char * b) {
	int len = strlen(b);
	if (b <= 0) {
		return false;
	}
	if (len * 3 / 4 + 1 > _maxLength) {
		return false;
	}
	reinitBuffer(len);
	B64STREAMLIB_indexDataType idx;
	for (idx = 0; idx < len; idx++) {
		EXTRA_YIELD();
		_buffer[idx] = b[idx];
	}
	_buffer[idx] = '\0';
	_status = B64STREAMLIB_STATUS_READY;
	return true;
}



