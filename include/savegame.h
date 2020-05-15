#ifndef PS1_SAVEGAME_H
#define PS1_SAVEGAME_H

#include <string>
#include <vector>

#include "filesystem.h"
#include "ps1common.h"

namespace ps1
{

class SaveGame : public Filesystem
{
public:
  SaveGame(const DataContainer& card_data, int block);

  void save(const std::string& path);

  int getBlocks() const override { return blocks_; };

  std::string getSaveTitle() const;
private:
  int blocks_;
};

}

#endif // PS1_SAVEGAME_H
