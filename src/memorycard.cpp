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

bool MemoryCard::checkData()
{
  if (!checkSize())
  {
    return false;
  }

  return checkFrame(data_.begin());
}

bool MemoryCard::checkFrame(DataContainer::iterator frame_it)
{
  char checksum = 0;
  auto checksum_it = std::next(frame_it, 127);

  for (auto it = frame_it; it != checksum_it; ++it)
  {
    checksum ^= *it;
  }

  return checksum == *checksum_it;
}

}
