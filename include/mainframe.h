#ifndef KUTACARD_MAIN_FRAME_H
#define KUTACARD_MAIN_FRAME_H

#include "memorycard.h"
#include <string>
#include <wx/listctrl.h>
#include <wx/wx.h>

namespace kutacard
{

class MainFrame : public wxFrame {
public:
  MainFrame();

private:
  enum {
    ID_Open = 1
  };

  void OnOpen(wxCommandEvent &event);

  void OnAbout(wxCommandEvent &event);

  void openMemoryCard(const std::string& path);

  MemoryCard mem_card_;
  wxListView* mem_card_lv_;
};

}
#endif //KUTACARD_MAIN_FRAME_H
