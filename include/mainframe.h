#ifndef MY_APP_MainFrame_H
#define MY_APP_MainFrame_H

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
#endif //MY_APP_MainFrame_H
