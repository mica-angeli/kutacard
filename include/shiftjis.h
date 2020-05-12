#ifndef SHIFTJIS_SHIFTJIS_H
#define SHIFTJIS_SHIFTJIS_H

#include <sstream>
#include <string>
#include <unordered_map>

#include "shiftjis_table.h"

namespace shiftjis
{
  static std::unordered_map<uint16_t, char> generateConversionTable()
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
    
    // Punctuation
    map[0x8142] = ',';
    map[0x8143] = '.';
    map[0x8145] = ':';
    map[0x8146] = ';';
    map[0x8147] = '?';
    map[0x8148] = '!';
    map[0x814f] = '^';
    map[0x8151] = '_';
    map[0x815e] = '/';
    map[0x815f] = '\\';
    map[0x8160] = '~';
    map[0x8162] = '|';
    map[0x8169] = '(';
    map[0x816a] = ')';
    map[0x816d] = '[';
    map[0x816e] = ']';
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

  std::string toUtf8(const std::string& input)
  {
    std::ostringstream oss;
    for(auto it = input.begin(); it != input.end() and std::next(it) != input.end(); it += 2)
    {
      const uint16_t c = (static_cast<uint8_t>(*it) << 8) + static_cast<uint8_t>(*std::next(it));
      
      if(Utf8ConversionTable.find(c) != Utf8ConversionTable.end())
      {
        oss << Utf8ConversionTable.at(c);
      }
      else
      {
        throw std::runtime_error("Unable to convert Shift-JIS character to UTF-8");
      }
    }

    return oss.str();
  }
}

#endif //SHIFTJIS_SHIFTJIS_H
