#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>
#include <argparse.hpp>

#include "memorycard.h"

bool list(const std::string& path)
{
  ps1::MemoryCard mem_card;
  mem_card.loadFile(path);

  if (!mem_card.checkData()) {
    return false;
  }

  // Print header
  std::cout << "\033[1m #   Type Region   Product ID    Save ID Title\033[0m" << std::endl; 

  // Process blocks in memory card
  for(int i = 0; i < mem_card.getDirectoryFrames().size(); i++)
  {
    using BlockType = ps1::DirectoryFrame::BlockType;
    using Region = ps1::DirectoryFrame::Region;

    const auto& dir_frame = mem_card.getDirectoryFrames()[i];

    std::string block_type;
    switch(dir_frame.getBlockType())
    {
      case BlockType::Initial:
        block_type = "INIT";
        break;
      case BlockType::Identity:
        block_type = "IDENT";
        break;
      case BlockType::Medial:
        block_type = "MEDIAL";
        break;
      case BlockType::Final:
        block_type = "FINAL";
        break;
      case BlockType::Formatted:
        block_type = "FORMAT";
        break;
      case BlockType::Reserved:
        block_type = "RESRVD";
        break;
      case BlockType::Deleted_Initial:
      case BlockType::Deleted_Medial:
      case BlockType::Deleted_Final:
        block_type = "DELETED";
        break;
    }

    std::string region;
    switch(dir_frame.getRegion())
    {
      case Region::American:
        region = "US";
        break;

      case Region::European:
        region = "EU";
        break;

      case Region::Japanese:
        region = "JP";
        break;
    }

    std::ostringstream oss;
    oss << std::setw(2) << i << " ";
    oss << std::setw(6) << block_type << " ";
    oss << std::setw(6) << region << " ";
    oss << std::setw(12) << dir_frame.getProductCode() << " ";
    oss << std::setw(10) << dir_frame.getIdentifier() << " ";
    oss << mem_card.getSaveTitle(i) << " ";

    std::cout << oss.str() << std::endl;
  }

  return true;
}

int main(int argc, char *argv[])
{
  argparse::ArgumentParser parser("kutacli");
  parser.add_argument("command")
    .help("Command to run.  Choices are <list>.")
    .action([](const std::string& v) {return v;});

  parser.add_argument("file")
    .help("Memory card file to process.  Must have *.mcr extension")
    .action([](const std::string& v) {return v;});
  
  parser.parse_args(argc, argv);

  const std::string command = parser.get<std::string>("command");
  if(command == "list")
  {
    const std::string file = parser.get<std::string>("file");
    list(file);
  }

  return 0;
}
