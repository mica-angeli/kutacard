#ifndef MY_APP_MYFRAME_H
#define MY_APP_MYFRAME_H

#include <wx/wx.h>

class MyFrame : public wxFrame {
public:
  MyFrame();

private:
  enum {
    ID_Open = 1
  };

  void OnOpen(wxCommandEvent &event);

  void OnAbout(wxCommandEvent &event);
};

#endif //MY_APP_MYFRAME_H
