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

  Bind(wxEVT_CONTEXT_MENU, &MemoryCardListView::OnContextMenu, this);
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

    InsertItem(item_i, "");
    SetItem(item_i, COLUMN_BLOCK_NUM, block_num);
    SetItem(item_i, COLUMN_REGION, region);
    SetItem(item_i, COLUMN_TITLE, title);
    SetItem(item_i, COLUMN_PRODUCT_CODE, product_code);
    SetItem(item_i, COLUMN_IDENTIFIER, identifier);
    item_i++;
  }

  // Resize list columns
  SetColumnWidth(COLUMN_BLOCK_NUM, wxLIST_AUTOSIZE_USEHEADER);
  SetColumnWidth(COLUMN_REGION, wxLIST_AUTOSIZE_USEHEADER);
  SetColumnWidth(COLUMN_TITLE, wxLIST_AUTOSIZE);
  SetColumnWidth(COLUMN_PRODUCT_CODE, wxLIST_AUTOSIZE_USEHEADER);
  SetColumnWidth(COLUMN_IDENTIFIER, wxLIST_AUTOSIZE_USEHEADER);
}

void MemoryCardListView::OnContextMenu(wxContextMenuEvent& event)
{
    if (GetEditControl() == NULL)
    {
        wxPoint point = event.GetPosition();
        // If from keyboard
        if ( (point.x == -1) && (point.y == -1) )
        {
            wxSize size = GetSize();
            point.x = size.x / 2;
            point.y = size.y / 2;
        }
        else
        {
            point = ScreenToClient(point);
        }
        int flags;
        ShowContextMenu(point, HitTest(point, flags));
    }
    else
    {
        // the user is editing:
        // allow the text control to display its context menu
        // if it has one (it has on Windows) rather than display our one
        event.Skip();
    }
}

void MemoryCardListView::ShowContextMenu(const wxPoint& pos, long item)
{
    wxMenu menu;
    menu.Append(wxID_ANY, wxString::Format("Menu for item %ld", item));
    menu.Append(wxID_ABOUT, "&About");
    menu.AppendSeparator();
    menu.Append(wxID_EXIT, "E&xit");

    PopupMenu(&menu, pos.x, pos.y);
}

}
