#include <algorithm>
#include <ios>
#include <iostream>
#include <fstream>

#include "memorycard.h"

namespace ps1
{

MemoryCard::MemoryCard()
{

}

MemoryCard::MemoryCard(const std::string& path)
{
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  const auto size = file.tellg();
  file.seekg(0, std::ios::beg);

  data_.clear();
  data_.resize(size);
  file.read(data_.data(), size);
}

void MemoryCard::printData() const
{
  for (const auto& val : data_)
  {
    std::cout << std::hex << static_cast<int>(val) << " ";
  }

  std::cout << std::endl;
}

bool MemoryCard::checkData() const
{
  if (!checkSize())
  {
    return false;
  }

  if (getBlockType(0) != BlockType::Identity)
  {
    return false;
  }

  for (int frame = 0; frame < 16; frame++)
  {
    auto it = std::next(data_.begin(), getIndex(0, frame));

    if(!checkFrame(it))
    {
      return false;
    }
  }

  return true;
}

bool MemoryCard::checkFrame(DataContainer::const_iterator frame_it)
{
  uint8_t checksum = 0;
  auto checksum_it = std::next(frame_it, 127);

  for (auto it = frame_it; it != checksum_it; ++it)
  {
    checksum ^= getValue<uint8_t>(it);
  }

  return checksum == getValue<uint8_t>(checksum_it);
}

SaveGame MemoryCard::getSaveGame(int block) const
{
  return SaveGame(data_, block);
}

}
