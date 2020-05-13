#ifndef PS1_SAVEGAME_H
#define PS1_SAVEGAME_H

#include <string>
#include <vector>

#include "directoryframe.h"
#include "ps1common.h"

namespace ps1
{

class SaveGame
{
public:
  SaveGame(const DataContainer& card_data, int block);

  void save(const std::string& path);

private:
  DirectoryFrame dir_frame_;
  DataContainer save_data_;
};

}

#endif // PS1_SAVEGAME_H
