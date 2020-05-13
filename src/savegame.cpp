#include <cassert>

#include "savegame.h"

namespace ps1
{
SaveGame::SaveGame(const DataContainer& mem_card_data, int block) :
  dir_frame_{DataContainer(std::next(mem_card_data.begin(), getIndex(0, block)), std::next(mem_card_data.begin(), getIndex(0, block + 1)))}
{
  assert(block >= 0 and block <= 15);
  assert(dir_frame_.getBlockType() == DirectoryFrame::BlockType::Initial);
  
  const int save_size = dir_frame_.getSaveSize();

  auto save_it = std::next(mem_card_data.begin(), getIndex(block));
  save_data_ = DataContainer(save_it, std::next(save_it, save_size));
}

void SaveGame::save(const std::string& path)
{
  
}

}
