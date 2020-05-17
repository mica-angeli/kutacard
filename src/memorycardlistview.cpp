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
  wxListView{parent, winid, pos, size, style | wxLC_SINGLE_SEL, validator, name}
{
  AppendColumn("Block #");
  AppendColumn("Region");
  AppendColumn("Title");
  AppendColumn("Product Code");
  AppendColumn("Identifier");

  Bind(wxEVT_CONTEXT_MENU, &MemoryCardListView::OnContextMenu, this);
  Bind(wxEVT_MENU, &MemoryCardListView::OnExportSave, this, ID_ExportSave);
}


void MemoryCardListView::update()
{
  DeleteAllItems();

  if(!mem_card)
    return;

  int item_i = 0;
  for(int block = 1; block < mem_card->getBlocks(); block++)
  {
    const std::string block_num = std::to_string(block);
    std::string region;
    std::string title;
    std::string product_code;
    std::string identifier;

    switch(mem_card->getBlockType(block))
    {
      case BlockType::Initial:
        title = mem_card->getSaveTitle(block);
        region = region_to_str.at(mem_card->getRegion(block));
        product_code = mem_card->getProductCode(block);
        identifier = mem_card->getIdentifier(block);
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
        const long item = HitTest(point, flags);
        Select(item);
        ShowContextMenu(point, item);
    }
    else
    {
        // the user is editing:
        // allow the text control to display its context menu
        // if it has one (it has on Windows) rather than display our one
        event.Skip();
    }
}

void MemoryCardListView::OnExportSave(wxCommandEvent &event)
{
  wxFileDialog exportSaveGameDialog(this,
                                    _("Export save game file"),
                                    "",
                                    "",
                                    "Single save (*.mcs)|*.mcs",
                                    wxFD_SAVE);

  if (exportSaveGameDialog.ShowModal() != wxID_CANCEL)
  {
    const std::string path = exportSaveGameDialog.GetPath().ToStdString();
    SaveGame save = mem_card->getSaveGame(getSelectedBlock());
    save.save(path);
  }

}

void MemoryCardListView::ShowContextMenu(const wxPoint& pos, long item)
{
    wxMenu menu;
    menu.Append(ID_FormatBlock, "&Format block");
    menu.AppendSeparator();
    menu.Append(ID_ExportSave, "&Export save...");
    menu.Append(ID_ImportSave, "&Import save...");

    const auto block_type = mem_card->getBlockType(getSelectedBlock(item));
    switch(block_type)
    {
      case BlockType::Initial:
        menu.Enable(ID_FormatBlock, true);
        menu.Enable(ID_ExportSave, true);
        menu.Enable(ID_ImportSave, false);
        break;
      case BlockType::Formatted:
        menu.Enable(ID_FormatBlock, false);
        menu.Enable(ID_ExportSave, false);
        menu.Enable(ID_ImportSave, true);
        break;
      case BlockType::Identity:
      case BlockType::Reserved:
        menu.Enable(ID_FormatBlock, false);
        menu.Enable(ID_ExportSave, false);
        menu.Enable(ID_ImportSave, false);
        break;
      case BlockType::Medial:
      case BlockType::Final:
      case BlockType::Deleted_Initial:
      case BlockType::Deleted_Medial:
      case BlockType::Deleted_Final:
        menu.Enable(ID_FormatBlock, true);
        menu.Enable(ID_ExportSave, false);
        menu.Enable(ID_ImportSave, false);
        break;
    }

    PopupMenu(&menu, pos.x, pos.y);
}

void MemoryCardListView::openMemoryCard(const std::string &path)
{
  mem_card = std::make_unique<MemoryCard>(path);

  if (!mem_card->checkData()) {
    return;
  }

  std::ostringstream log_oss;
  log_oss.imbue(std::locale(""));
  log_oss << "Successfully loaded memory card of size " << std::fixed << mem_card->size() << " bytes";
  wxLogMessage(wxString(log_oss.str()));

  update();
}

}
