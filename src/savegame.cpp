#include <cassert>

#include "shiftjis.h"
#include "savegame.h"

namespace ps1
{
SaveGame::SaveGame(const DataContainer& mem_card_data, int block)
{
  assert(block >= 0 and block <= 15);

  // Add directory frame data
  auto dir_frame_it = std::next(mem_card_data.begin(), getIndex(0, block));
  data_.insert(data_.end(), dir_frame_it, std::next(dir_frame_it, FRAME_SIZE));

  assert(getBlockType(0) == BlockType::Initial);
  
  // TODO: We should not assume the save size like this
  const int save_size = getSaveSize(0);

  blocks_ = save_size / 8192;

  // Add save data
  auto save_it = std::next(mem_card_data.begin(), getIndex(block));
  data_.insert(data_.end(), save_it, std::next(save_it, save_size));
}

bool SaveGame::checkData() const
{
  return true;
}

void SaveGame::save(const std::string& path)
{
  
}

}
