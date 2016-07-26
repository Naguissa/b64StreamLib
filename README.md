# Arduino IDE base64 (URL) stream library

## What is this repository for? ##

Base64 encoder/decoder that can act with whole strings or with byte-by-byte streams.


It's coded with base64 URL standard WITHOUT terminators. You can modify _charmap variable on cpp file to adapt your desired standard

B64STREAMLIB_indexDataType define controls capacity. Actually it uses unsigned char, so maximum buffer is 255 and maximum decoded variable is 191 chars long: mod(255 * 3 / 4)

EXTRA_YIELD() is a trick to not overload ESP8266 microcontrollers; in other boards it does nothing (skipped in compile-time).




## How do I get set up? ##

 * Get the ZIP.
 * Rename to b64StreamLib.zip
 * Install library on Arduino


## Example ##

Included on example folder, available on Arduino IDE.



## Who do I talk to? ##

 * [Naguissa](https://github.com/Naguissa)
 * http://www.naguissa.com
 * http://www.foroelectro.net
