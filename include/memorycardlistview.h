#ifndef KUTACARD_MEMORY_CARD_LIST_VIEW_H
#define KUTACARD_MEMORY_CARD_LIST_VIEW_H

#include <memory>
#include <wx/listctrl.h>
#include <wx/wx.h>

#include "memorycard.h"

namespace kutacard
{

class MemoryCardListView : public wxListView
{
public:
  MemoryCardListView();

  MemoryCardListView(wxWindow *parent,
                     wxWindowID winid = wxID_ANY,
                     const wxPoint& pos = wxDefaultPosition,
                     const wxSize& size = wxDefaultSize,
                     long style = wxLC_REPORT,
                     const wxValidator& validator = wxDefaultValidator,
                     const wxString &name = wxListCtrlNameStr);

  void update();

  void openMemoryCard(const std::string &path);

private:
  using BlockType = ps1::Filesystem::BlockType;
  using Region = ps1::Filesystem::Region;
  using MemoryCard = ps1::MemoryCard;

  enum {
    COLUMN_BLOCK_NUM,
    COLUMN_REGION,
    COLUMN_TITLE,
    COLUMN_PRODUCT_CODE,
    COLUMN_IDENTIFIER,
    COLUMN_SIZE
  };

  enum {
    ID_ExportSave = 1
  };

  static inline const std::unordered_map<Region, std::string> region_to_str = {
    {Region::American, "USA"}, {Region::European, "EUR"}, {Region::Japanese, "JPN"}};
  
  void OnContextMenu(wxContextMenuEvent& event);

  void OnExportSave(wxCommandEvent &event);

  void ShowContextMenu(const wxPoint& pos, long item);

  std::unique_ptr<MemoryCard> mem_card;
};

}

#endif // KUTACARD_MEMORY_CARD_LIST_VIEW_H