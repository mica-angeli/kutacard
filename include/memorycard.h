#ifndef PS1_MEMORY_CARD_H
#define PS1_MEMORY_CARD_H

#include <string>
#include <vector>

#include "filesystem.h"
#include "savegame.h"

namespace ps1
{

class MemoryCard : public Filesystem
{
public:
  MemoryCard();
  
  MemoryCard(const std::string& path);

  void printData() const;

  bool checkData() const override;

  int getBlocks() const override
  {
    return NUM_BLOCKS;
  }

  SaveGame getSaveGame(int block) const;

private:
  inline bool checkSize() const { return data_.size() == CARD_SIZE; };

  static bool checkFrame(DataContainer::const_iterator frame_it);
};

}

#endif // PS1_MEMORY_CARD_H
