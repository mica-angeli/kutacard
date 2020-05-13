#ifndef PS1_DIRECTORY_FRAME_H
#define PS1_DIRECTORY_FRAME_H

#include <string>
#include "ps1common.h"

namespace ps1
{

class DirectoryFrame
{
public:
  DirectoryFrame(const DataContainer& frame_data) :
    data_{frame_data}
  {
  };

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

  inline BlockType getBlockType() const
  {
    auto it = data_.begin();
    return static_cast<BlockType>(getValue<uint32_t>(it));
  }
  
  inline uint32_t getSaveSize() const
  {
    auto it = std::next(data_.begin(), 4);
    return getValue<uint32_t>(it);
  }

  inline uint16_t getNextBlock() const
  {
    auto it = std::next(data_.begin(), 8);
    return getValue<uint16_t>(it);
  }

  inline Region getRegion() const
  {
    auto it = std::next(data_.begin(), 10);
    return static_cast<Region>(getValue<uint16_t>(it));
  }

  inline std::string getProductCode() const
  {
    auto it = std::next(data_.begin(), 12);
    return std::string(it, std::next(it, 10));
  }

  inline std::string getIdentifier() const
  {
    auto it = std::next(data_.begin(), 22);
    return std::string(&*it);
  }

  inline bool isSaveGame() const
  {
    const auto type = getBlockType();
    return type == BlockType::Initial || type == BlockType::Medial || type == BlockType::Final;
  }

private:
  DataContainer data_;
};

}

#endif // PS1_DIRECTORY_FRAME_H
