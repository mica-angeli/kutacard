#ifndef KUTACARD_MAIN_FRAME_H
#define KUTACARD_MAIN_FRAME_H

#include <string>
#include <memory>
#include <vector>

#include <wx/wx.h>

#include "memorycardlistview.h"
#include "memorycard.h"
#include "savegame.h"

namespace kutacard
{

class MainFrame : public wxFrame {
public:
  MainFrame();

private:
  using MemoryCards = std::vector<ps1::MemoryCard>;

  void OnOpen(wxCommandEvent &event);

  void OnSave(wxCommandEvent &event);

  void OnSaveAs(wxCommandEvent &event);

  void OnAbout(wxCommandEvent &event);

  std::unique_ptr<ps1::SaveGame> save_buffer_;

  MemoryCardListView* current_lv_;

  std::vector<MemoryCardListView*> mem_card_lvs_;
};

}
#endif //KUTACARD_MAIN_FRAME_H
