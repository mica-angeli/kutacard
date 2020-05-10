#include <ios>
#include <iostream>
#include <fstream>
#include "memorycard.h"

namespace kutacard
{
void MemoryCard::loadFile(const std::string& path)
{
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  const auto size = file.tellg();
  file.seekg(0, std::ios::beg);

  data_.resize(size);
  file.read(data_.data(), size);
}

void MemoryCard::printData()
{
  for (const auto& val : data_)
  {
    std::cout << std::hex << static_cast<int>(val) << " ";
  }

  std::cout << std::endl;
}
}
