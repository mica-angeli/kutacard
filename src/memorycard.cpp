#include <algorithm>
#include <ios>
#include <iostream>
#include <fstream>

#include "memorycard.h"

namespace ps1
{

MemoryCard::MemoryCard()
{
  // Fill data with zeros
  for(int i = 0; i < CARD_SIZE; i++)
    data_.push_back(0);
  
  // Create identity frame
  setValue(data_.begin(), static_cast<uint32_t>(BlockType::Identity));

  // Create block 0 frames
  for(int frame = 1; frame < 16; frame++)
  {
    auto frame_it = std::next(data_.begin(), frame * FRAME_SIZE);
    setValue(frame_it, static_cast<uint32_t>(BlockType::Formatted));

    auto next_block_it = std::next(frame_it, 8);
    setValue(next_block_it, NEXT_BLOCK_NONE);
  }

  // Create reserved frames
  for(int frame = 16; frame < 36; frame++)
  {
    auto frame_it = std::next(data_.begin(), frame * FRAME_SIZE);
    setValue(frame_it, static_cast<uint32_t>(BlockType::Reserved));

    auto next_block_it = std::next(frame_it, 8);
    setValue(next_block_it, NEXT_BLOCK_NONE);
  }

  // Add checksums to frames
  for(int frame = 0; frame < 36; frame++)
  {
    auto frame_it = std::next(data_.begin(), frame * FRAME_SIZE);
    auto checksum_it = std::next(frame_it, FRAME_SIZE - 1);
    auto sum = checksum(frame_it, checksum_it);
    setValue(checksum_it, sum);
  }

  // Fill unused area in first block
  std::fill(std::next(data_.begin(), 36 * FRAME_SIZE), std::next(data_.begin(), 63 * FRAME_SIZE), 0xff);
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

SaveGame MemoryCard::getSaveGame(int block) const
{
  return SaveGame(data_, block);
}

void MemoryCard::save(const std::string& path) const
{
  std::ofstream file(path, std::ios::binary | std::ios::out | std::ios::trunc);
  file.write(data_.data(), data_.size());
  file.close();
}

}
