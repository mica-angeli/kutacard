#ifndef PS1_SAVEGAME_H
#define PS1_SAVEGAME_H

#include <string>
#include <vector>

#include "filesystem.h"

namespace ps1
{

class SaveGame : public Filesystem
{
public:
  SaveGame(const DataContainer& card_data, int block);

  bool checkData() const override;

  void save(const std::string& path);

  int getBlocks() const override { return blocks_; };

  std::string getSaveTitle() const;
private:
  int getIndex(int block, int frame = 0, int byte = 0) const override
  {
    if(block == 0)
    {
      return frame * FRAME_SIZE + byte;
    }
    else
    {
      return (block - 1) * BLOCK_SIZE + (frame + 1) * FRAME_SIZE + byte;
    }
  }

  int blocks_;
};

}

#endif // PS1_SAVEGAME_H
