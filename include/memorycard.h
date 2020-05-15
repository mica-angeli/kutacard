#ifndef PS1_MEMORY_CARD_H
#define PS1_MEMORY_CARD_H

#include <string>
#include <vector>

#include "filesystem.h"

namespace ps1
{

class MemoryCard : public Filesystem
{
public:
  MemoryCard() = default;

  void loadFile(const std::string& path);

  void printData() const;

  bool checkData() const override;

  std::string getSaveTitle(int block) const;

  int getBlocks() const override
  {
    return 16;
  }

private:
  inline bool checkSize() const { return data_.size() == CARD_SIZE; };

  static bool checkFrame(DataContainer::const_iterator frame_it);
};

}

#endif // PS1_MEMORY_CARD_H
