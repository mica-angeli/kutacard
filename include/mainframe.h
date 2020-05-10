#ifndef KUTACARD_MAIN_FRAME_H
#define KUTACARD_MAIN_FRAME_H

#include "memorycard.h"
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

  MemoryCard mem_card_;
};

}
#endif //KUTACARD_MAIN_FRAME_H
