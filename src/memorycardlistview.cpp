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
  AppendColumn("Block Type");
  AppendColumn("Region");
  AppendColumn("Title");
  AppendColumn("Product Code");
  AppendColumn("Identifier");
}


void MemoryCardListView::update(const ps1::MemoryCard& mem_card)
{
  DeleteAllItems();

  for(int i = 0; i < mem_card.getBlocks(); i++)
  {
    using BlockType = ps1::Filesystem::BlockType;
    using Region = ps1::Filesystem::Region;

    std::string block_type;
    switch(mem_card.getBlockType(i))
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
    switch(mem_card.getRegion(i))
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

    int col = 0;
    InsertItem(i, "");
    SetItem(i, col++, std::to_string(i));
    SetItem(i, col++, block_type);
    SetItem(i, col++, region);
    SetItem(i, col++, mem_card.getSaveTitle(i));
    SetItem(i, col++, mem_card.getProductCode(i));
    SetItem(i, col++, mem_card.getIdentifier(i));
  }
}

}
