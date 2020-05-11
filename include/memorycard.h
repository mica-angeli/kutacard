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

public:
  MemoryCard() = default;

  void loadFile(const std::string& path);

  void printData();

  inline std::size_t size() { return data_.size(); };

  bool checkData();


  using DataContainer = std::vector<char>;

  struct DirectoryFrame
  {
    DirectoryFrame() = default;

    DirectoryFrame(DataContainer::const_iterator frame_it) :
      DirectoryFrame{}
    {
      load(frame_it);
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
    } block_type;

    enum class TerritoryCode: uint16_t {
      American = 0x4142,
      European = 0x4542,
      Japanese = 0x4942
    } territory;

    uint32_t save_size;
    uint16_t next_block;
    std::string license_code;
    std::string save_code;

    void load(DataContainer::const_iterator frame_it)
    {
      block_type = static_cast<BlockType>(getValue<uint32_t>(frame_it));
      std::advance(frame_it, sizeof(uint32_t));

      switch(block_type)
      {
        case BlockType::Identity:
        case BlockType::Reserved:
        case BlockType::Formatted:
          break;

        default:
        {
          save_size = getValue<uint32_t>(frame_it);
          std::advance(frame_it, sizeof(uint32_t));

          // TODO: Check size of next block
          next_block = getValue<uint16_t>(frame_it);
          std::advance(frame_it, sizeof(uint16_t));

          territory = static_cast<TerritoryCode>(getValue<uint16_t>(frame_it));
          std::advance(frame_it, sizeof(uint16_t));

          license_code = std::string(frame_it, std::next(frame_it, 10));
          std::advance(frame_it, 10);

          // Get null-terminated save_code
          save_code = std::string(&*frame_it);
          std::advance(frame_it, 8);
          break;
        }
      }
    }
  };

  std::vector<DirectoryFrame> dir_frames_;
private:
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
