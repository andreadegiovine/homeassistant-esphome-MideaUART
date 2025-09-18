/* Logging disabled by patch for ESP8266 stability unless MIDEA_NO_LOG=0 */
#if defined(MIDEA_NO_LOG) && MIDEA_NO_LOG==1
#define LOG_D(...) do {} while(0)
#define LOG_I(...) do {} while(0)
#define LOG_W(...) do {} while(0)
#define LOG_E(...) do {} while(0)
#else
#pragma once
#include <Arduino.h>
#include <cstdarg>
#include <functional>

namespace dudanov {

using LoggerFn = std::function<void(int, const char *, int, String /* TODO: consider replacing with char[] to avoid fragmentation */, va_list)>;
extern LoggerFn logger_;
void setLogger(LoggerFn logger);

}  // namespace dudanov

#endif
