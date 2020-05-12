#include <exception>
#include <sstream>

#include "mainframe.h"
#include "mainapp.h"

namespace kutacard
{

bool MainApp::OnInit() {
  auto frame = new MainFrame;
  frame->Show(true);
  return true;
}

bool MainApp::OnExceptionInMainLoop()
{
  std::exception_ptr p = std::current_exception();
  try {
    std::rethrow_exception(p);
  } catch (const std::exception& e) {
    std::ostringstream oss;
    oss << "Detected exception in main loop with description: \n\"" << e.what() << "\"";
    wxLogError(wxString(oss.str()));
  }

  return false;
}

void MainApp::OnUnhandledException()
{
  wxLogError("Detected unhandled exception!");
}

}

// Macro containing main()
wxIMPLEMENT_APP(kutacard::MainApp);
