#ifndef KUTACARD_SHIFTJIS_H
#define KUTACARD_SHIFTJIS_H

#include <sstream>
#include <string>

namespace shiftjis
{

  std::string toAscii(const std::string& input)
  {
    std::ostringstream oss;
    for(auto it = input.begin(); it != input.end() and std::next(it) != input.end(); it += 2)
    {
      const uint16_t c = (static_cast<uint8_t>(*it) << 8) + static_cast<uint8_t>(*std::next(it));
      if(c >= 0x824f and c <= 0x8258)
      {
        // Number
        oss << char(c - 0x824f + 0x30);
      }
      else if(c >= 0x8260 and c <= 0x8279)
      {
        // Capital letter
        oss << char(c - 0x8260 + 0x41);
      }
      else if(c >= 0x8281 and c <= 0x829a)
      {
        // Lowercase letter
        oss << char(c - 0x8281 + 0x61);
      }
      else
      {
        oss << ' ';
      }
    }

    return oss.str();
  }
}

#endif //KUTACARD_SHIFTJIS_H
