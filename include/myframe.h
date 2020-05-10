#ifndef MY_APP_MYFRAME_H
#define MY_APP_MYFRAME_H

#include <wx/wx.h>

class MyFrame : public wxFrame {
public:
  MyFrame();

private:
  enum {
    ID_Hello = 1
  };

  void OnHello(wxCommandEvent &event);

  void OnAbout(wxCommandEvent &event);
};

#endif //MY_APP_MYFRAME_H
