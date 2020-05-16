#ifndef KUTACARD_MAIN_FRAME_H
#define KUTACARD_MAIN_FRAME_H

#include <string>
#include <memory>
#include <vector>

#include <wx/listctrl.h>
#include <wx/wx.h>

#include "memorycard.h"
#include "savegame.h"

namespace kutacard
{

class MainFrame : public wxFrame {
public:
  MainFrame();

private:
  using MemoryCards = std::vector<ps1::MemoryCard>;
  enum {
    ID_Open = 1
  };

  void OnOpen(wxCommandEvent &event);

  void OnAbout(wxCommandEvent &event);

  void openMemoryCard(const std::string& path);

  int current_card_;

  MemoryCards cards_;

  std::unique_ptr<ps1::SaveGame> save_buffer_;

  wxListView* mem_card_lv_;
};

}
#endif //KUTACARD_MAIN_FRAME_H
