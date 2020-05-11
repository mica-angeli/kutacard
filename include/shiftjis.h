#ifndef KUTACARD_SHIFTJIS_H
#define KUTACARD_SHIFTJIS_H

#include <sstream>
#include <string>
#include <unordered_map>

namespace shiftjis
{
  std::unordered_map<uint16_t, char> generateConversionTable()
  {
    std::unordered_map<uint16_t, char> map;

    // Numbers
    for (uint16_t c = 0x824f; c <= 0x8258; c++)
      map[c] = char(c - 0x824f + 0x30);

    // Capital letters
    for (uint16_t c = 0x8260; c <= 0x8279; c++)
      map[c] = char(c - 0x8260 + 0x41);

    // Lowercase letters
    for (uint16_t c = 0x8281; c <= 0x829a; c++)
      map[c] = char(c - 0x8281 + 0x61);

    return map;
  }

  std::string toAscii(const std::string& input)
  {
    static const auto conversionTable = generateConversionTable();

    std::ostringstream oss;
    for(auto it = input.begin(); it != input.end() and std::next(it) != input.end(); it += 2)
    {
      const uint16_t c = (static_cast<uint8_t>(*it) << 8) + static_cast<uint8_t>(*std::next(it));
      
      if(conversionTable.find(c) != conversionTable.end())
      {
        oss << conversionTable.at(c);
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
