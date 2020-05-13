#ifndef PS1_PS1COMMON_H
#define PS1_PS1COMMON_H

#include <vector>

namespace ps1
{
static constexpr int NUM_BLOCKS = 16;
static constexpr int NUM_FRAMES = 64;
static constexpr int FRAME_SIZE = 128;
static constexpr int BLOCK_SIZE = NUM_FRAMES * FRAME_SIZE;
static constexpr int CARD_SIZE = NUM_BLOCKS * BLOCK_SIZE;

using DataContainer = std::vector<char>;

template<typename ValueT>
static inline ValueT getValue(DataContainer::const_iterator it)
{
  ValueT val = 0;
  for(int i = 0; i < sizeof(ValueT); i++)
  {
    val += static_cast<uint8_t>(*it++) << i * 8;
  }
  return val;
}

static inline int getIndex(int block, int frame, int byte = 0)
{
  return block * BLOCK_SIZE + frame * FRAME_SIZE + byte;
}

}

#endif // PS1_PS1COMMON_H
