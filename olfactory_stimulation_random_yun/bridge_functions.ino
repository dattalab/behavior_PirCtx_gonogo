#include <Bridge.h>
#include <stdarg.h>

int getIntBridge(char* key) {
  char bufferData[10];
  Bridge.get(key, bufferData, 10);
  return atoi(bufferData);
}

String getStringBridge(char* key) {
  char bufferData[25];
  Bridge.get(key, bufferData, 25);
  String toReturn = String(bufferData);
  return toReturn;
}

char* fstringF(const __FlashStringHelper *fmt, ... ) {
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt);
#ifdef __AVR__
  vsnprintf_P(buf, sizeof(buf), (const char *)fmt, args); // progmem for AVR
#else
  vsnprintf(buf, sizeof(buf), (const char *)fmt, args); // for the rest of the world
#endif
  va_end(args);
  return buf;
}

char* fstring(const __FlashStringHelper *fmt, ... ) {
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt);
  vsnprintf(buf, sizeof(buf), (const char *)fmt, args); // for the rest of the world
  va_end(args);
  return buf;
}

