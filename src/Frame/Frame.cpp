#include "Frame/Frame.h"

namespace dudanov {
namespace midea {

void Frame::setData(const FrameData &data) {
  this->m_trimData();
  this->m_appendData(data);
  this->m_data[OFFSET_LENGTH] = this->m_data.size();
  this->m_data[OFFSET_SYNC] = this->m_data[OFFSET_LENGTH] ^ this->m_data[OFFSET_APPTYPE];
  this->m_appendCS();
}

uint8_t Frame::m_calcCS() const {
  if (this->m_data.size() <= OFFSET_LENGTH)
    return -1;
  uint8_t cs = 0;
  for (auto it = this->m_data.begin() + OFFSET_LENGTH; it != this->m_data.end(); ++it)
    cs -= *it;
  return cs;
}

static char u4hex(uint8_t num) { return num + ((num < 10) ? '0' : ('A' - 10)); }

String Frame::toString() const {
  String ret;
  char buf[4];
  buf[2] = ' ';
  buf[3] = '\0';
  ret.reserve(3 * this->size());
  for (const uint8_t data : this->m_data) {
    buf[0] = u4hex(data / 16);
    buf[1] = u4hex(data % 16);
    ret += buf;
  }
  return ret;
}


const char* Frame::toHexBuffer(char* buf, size_t buf_size) const {
  if (buf_size == 0) return nullptr;
  // Each byte -> 2 hex chars + space; plus terminating null.
  size_t needed = this->m_data.size() * 3 + 1;
  if (buf_size < needed) {
    // truncate if not enough space
    size_t max_bytes = (buf_size - 1) / 3;
    size_t pos = 0;
    for (size_t i = 0; i < max_bytes; ++i) {
      uint8_t v = this->m_data[i];
      const char hex[] = "0123456789ABCDEF";
      buf[pos++] = hex[(v >> 4) & 0xF];
      buf[pos++] = hex[v & 0xF];
      buf[pos++] = ' ';
    }
    if (pos > 0) pos--; // remove trailing space
    buf[pos] = '\\0';
    return buf;
  }
  size_t pos = 0;
  for (size_t i = 0; i < this->m_data.size(); ++i) {
    uint8_t v = this->m_data[i];
    const char hex[] = "0123456789ABCDEF";
    buf[pos++] = hex[(v >> 4) & 0xF];
    buf[pos++] = hex[v & 0xF];
    if (i + 1 != this->m_data.size())
      buf[pos++] = ' ';
  }
  buf[pos] = '\\0';
  return buf;
}

}  // namespace midea
}  // namespace dudanov
