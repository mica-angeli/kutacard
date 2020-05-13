#ifndef PS1_MEMORY_CARD_H
#define PS1_MEMORY_CARD_H

#include <string>
#include <vector>

#include "directoryframe.h"
#include "ps1common.h"

namespace ps1
{

class MemoryCard
{
public:
  MemoryCard() = default;

  void loadFile(const std::string& path);

  void printData();

  inline std::size_t size() { return data_.size(); };

  bool checkData();

  std::string getSaveTitle(int block);

  inline const std::vector<DirectoryFrame>& getDirectoryFrames() { return dir_frames_; };
private:
  inline bool checkSize() { return data_.size() == CARD_SIZE; };

  inline DataContainer::iterator getFrame(int block, int frame, int byte = 0)
  {
    const int i = block * BLOCK_SIZE + frame * FRAME_SIZE + byte;
    return std::next(data_.begin(), i);
  }

  static bool checkFrame(DataContainer::const_iterator frame_it);

  DataContainer data_;
  std::vector<DirectoryFrame> dir_frames_;
};

}

#endif // PS1_MEMORY_CARD_H
