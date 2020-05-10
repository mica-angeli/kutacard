#ifndef KUTACARD_MEMORY_CARD_H
#define KUTACARD_MEMORY_CARD_H

#include <string>
#include <vector>

namespace kutacard
{

class MemoryCard
{
  static constexpr int NUM_BLOCKS = 16;
  static constexpr int NUM_FRAMES = 64;
  static constexpr int FRAME_SIZE = 128;
  static constexpr int BLOCK_SIZE = NUM_FRAMES * FRAME_SIZE;
  static constexpr int CARD_SIZE = NUM_BLOCKS * BLOCK_SIZE;

  enum class BlockType : uint32_t {
    Initial = 81,
    Medial,
    Final,
    Formatted = 160,
    Deleted_Initial,
    Deleted_Medial,
    Deleted_Final,
    Identity = 17229,
    Reserved = 0xFFFFFFFF
  };

public:
  MemoryCard() = default;

  void loadFile(const std::string& path);

  void printData();

  inline std::size_t size() { return data_.size(); };

  bool checkData();

private:
  using DataContainer = std::vector<char>;

  inline bool checkSize() { return data_.size() == CARD_SIZE; };

  inline DataContainer::iterator getFrame(int block, int frame, int byte = 0)
  {
    const int i = block * BLOCK_SIZE + frame * FRAME_SIZE + byte;
    return std::next(data_.begin(), i);
  }

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

  static bool checkFrame(DataContainer::const_iterator frame_it);

  DataContainer data_;
};

}

#endif // KUTACARD_MEMORY_CARD_H
