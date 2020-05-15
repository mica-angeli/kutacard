#ifndef PS1_FILESYSTEM_H
#define PS1_FILESYSTEM_H

#include <string>
#include <vector>

namespace ps1
{

class Filesystem
{
public:
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

  enum class Region: uint16_t {
    American = 0x4142,
    European = 0x4542,
    Japanese = 0x4942
  };

  inline BlockType getBlockType(int block) const
  {
    auto it = std::next(data_.begin(), block * FRAME_SIZE);
    return static_cast<BlockType>(getValue<uint32_t>(it));
  }
  
  inline uint32_t getSaveSize(int block) const
  {
    auto it = std::next(data_.begin(), block * FRAME_SIZE + 4);
    return getValue<uint32_t>(it);
  }

  inline uint16_t getNextBlock(int block) const
  {
    auto it = std::next(data_.begin(), block * FRAME_SIZE + 8);
    return getValue<uint16_t>(it);
  }

  inline Region getRegion(int block) const
  {
    auto it = std::next(data_.begin(), block * FRAME_SIZE + 10);
    return static_cast<Region>(getValue<uint16_t>(it));
  }

  inline std::string getProductCode(int block) const
  {
    auto it = std::next(data_.begin(), block * FRAME_SIZE + 12);
    return std::string(it, std::next(it, 10));
  }

  inline std::string getIdentifier(int block) const
  {
    auto it = std::next(data_.begin(), block * FRAME_SIZE + 22);
    return std::string(&*it);
  }

  inline bool isSaveGame(int block) const
  {
    const auto type = getBlockType(block);
    return type == BlockType::Initial || type == BlockType::Medial || type == BlockType::Final;
  }

  virtual int getBlocks() const=0;

protected:
  using DataContainer = std::vector<char>;

  static constexpr int NUM_BLOCKS = 16;
  static constexpr int NUM_FRAMES = 64;
  static constexpr int FRAME_SIZE = 128;
  static constexpr int BLOCK_SIZE = NUM_FRAMES * FRAME_SIZE;
  static constexpr int CARD_SIZE = NUM_BLOCKS * BLOCK_SIZE;

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

  static inline int getIndex(int block, int frame = 0, int byte = 0)
  {
    return block * BLOCK_SIZE + frame * FRAME_SIZE + byte;
  }

  DataContainer data_;
};

}

#endif // PS1_FILESYSTEM_H
