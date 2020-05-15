#ifndef PS1_FILESYSTEM_H
#define PS1_FILESYSTEM_H

#include "ps1common.h"

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

  DataContainer data_;
};

}

#endif // PS1_FILESYSTEM_H
