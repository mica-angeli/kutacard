#include <unordered_map>

#include "memorycardlistview.h"

namespace kutacard
{
MemoryCardListView::MemoryCardListView() :
  wxListView{}
{

}

MemoryCardListView::MemoryCardListView(wxWindow *parent,
                                       wxWindowID winid,
                                       const wxPoint& pos,
                                       const wxSize& size,
                                       long style,
                                       const wxValidator& validator,
                                       const wxString &name) :
  wxListView{parent, winid, pos, size, style, validator, name}
{
  AppendColumn("Block #");
  AppendColumn("Region");
  AppendColumn("Title");
  AppendColumn("Product Code");
  AppendColumn("Identifier");
}


void MemoryCardListView::update(const ps1::MemoryCard& mem_card)
{
  DeleteAllItems();

  int item_i = 0;
  for(int block = 1; block < mem_card.getBlocks(); block++)
  {
    const std::string block_num = std::to_string(block);
    std::string region;
    std::string title;
    std::string product_code;
    std::string identifier;

    switch(mem_card.getBlockType(block))
    {
      case BlockType::Initial:
        title = mem_card.getSaveTitle(block);
        region = region_to_str.at(mem_card.getRegion(block));
        product_code = mem_card.getProductCode(block);
        identifier = mem_card.getIdentifier(block);
        break;
      case BlockType::Medial:
        title = "Linked block (middle)";
        break;
      case BlockType::Final:
        title = "Linked block (final)";
        break;
      case BlockType::Formatted:
        title = "Free block";
        break;
      case BlockType::Identity:
      case BlockType::Reserved:
      case BlockType::Deleted_Initial:
      case BlockType::Deleted_Medial:
      case BlockType::Deleted_Final:
        title = "Unformatted block";
        break;
    }

    int col = 0;
    InsertItem(item_i, "");
    SetItem(item_i, col++, block_num);
    SetItem(item_i, col++, region);
    SetItem(item_i, col++, title);
    SetItem(item_i, col++, product_code);
    SetItem(item_i, col++, identifier);
    item_i++;
  }
}

}
