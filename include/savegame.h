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

  SaveGame(const std::string& path);

  void save(const std::string& path) const;

  bool checkData() const override;

  int getBlocks() const override { return 1; };

  inline std::string getFullName() const { return Filesystem::getFullName(0); };

  inline std::string getSaveTitle() const { return Filesystem::getSaveTitle(1); };

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
};

}

#endif // PS1_SAVEGAME_H
