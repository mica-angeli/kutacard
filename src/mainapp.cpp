#include "mainframe.h"
#include "mainapp.h"

namespace kutacard
{

bool MainApp::OnInit() {
  auto frame = new MainFrame;
  frame->Show(true);
  return true;
}

}

// Macro containing main()
wxIMPLEMENT_APP(kutacard::MainApp);
