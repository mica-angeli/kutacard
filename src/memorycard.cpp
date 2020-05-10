#include <algorithm>
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

  for (int frame = 0; frame < 16; frame++)
  {
    auto it = getFrame(0, frame);

    DirectoryFrame dir_frame(it);
    std::cout << dir_frame.license_code << ", " << dir_frame.save_code << std::endl;

    if(frame == 0)
    {
      if(dir_frame.block_type != BlockType::Identity)
      {
        return false;
      }
    }
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

}
