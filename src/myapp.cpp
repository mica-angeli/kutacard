#include "myframe.h"
#include "myapp.h"

bool MyApp::OnInit() {
  auto frame = new MyFrame;
  frame->Show(true);
  return true;
}

// Macro containing main()
wxIMPLEMENT_APP(MyApp);
