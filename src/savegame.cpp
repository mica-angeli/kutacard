#include <cassert>
#include <ios>
#include <iostream>
#include <fstream>

#include "shiftjis.h"
#include "savegame.h"

namespace ps1
{
SaveGame::SaveGame(const DataContainer& mem_card_data, int block)
{
  assert(block >= 0 and block <= 15);

  // Add directory frame data
  const int dir_frame_index = FRAME_SIZE * block;
  auto dir_frame_it = std::next(mem_card_data.begin(), dir_frame_index);
  data_.insert(data_.end(), dir_frame_it, std::next(dir_frame_it, FRAME_SIZE));

  assert(getBlockType(0) == BlockType::Initial);
  
  // TODO: We should not assume the save size like this
  const int save_size = getSaveSize(0);

  // Add save data
  const int save_index = block * BLOCK_SIZE;
  auto save_it = std::next(mem_card_data.begin(), save_index);
  data_.insert(data_.end(), save_it, std::next(save_it, save_size));
}

SaveGame::SaveGame(const std::string& path)
{
  // TODO: add RAW support
  std::ifstream file(path, std::ios::binary | std::ios::ate);
  const auto size = file.tellg();
  file.seekg(0, std::ios::beg);

  data_.clear();
  data_.resize(size);
  file.read(data_.data(), size);
  file.close();
}

bool SaveGame::checkData() const
{
  return true;
}

void SaveGame::save(const std::string& path) const
{
  std::ofstream file(path, std::ios::binary | std::ios::out | std::ios::trunc);
  file.write(data_.data(), data_.size());
  file.close();
}

}
