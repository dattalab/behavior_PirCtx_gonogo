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

char* fstring(char *fmt, ... ) {
  char buf[128]; // resulting string limited to 128 chars
  va_list args;
  va_start (args, fmt );
  vsnprintf(buf, 128, fmt, args);
  va_end (args);
  return buf;
}


