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

  inline std::size_t size() const { return data_.size(); };

  bool checkData() const;

  std::string getSaveTitle(int block) const;

  int getBlocks() const override
  {
    return 16;
  }

private:
  inline bool checkSize() const { return data_.size() == CARD_SIZE; };

  inline DataContainer::const_iterator getFrame(int block, int frame, int byte = 0) const
  {
    const int i = block * BLOCK_SIZE + frame * FRAME_SIZE + byte;
    return std::next(data_.begin(), i);
  }

  static bool checkFrame(DataContainer::const_iterator frame_it);
};

}

#endif // PS1_MEMORY_CARD_H
