#ifndef KUTACARD_MEMORY_CARD_H
#define KUTACARD_MEMORY_CARD_H

#include <string>
#include <vector>

namespace kutacard
{

class MemoryCard
{
public:
  MemoryCard() = default;

  void loadFile(const std::string& path);

  void printData();

  inline std::size_t size() { return data_.size(); };

private:
  std::vector<char> data_;
};

}

#endif // KUTACARD_MEMORY_CARD_H
