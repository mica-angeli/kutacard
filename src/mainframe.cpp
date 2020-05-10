#include <sstream>
#include <string>
#include <locale>
#include "mainframe.h"

namespace kutacard
{

MainFrame::MainFrame() :
  wxFrame{nullptr, wxID_ANY, "Kutacard"},
  mem_card_{}
{
  auto *menuFile = new wxMenu;
  menuFile->Append(wxID_OPEN);
  // menuFile->Append(ID_Open, "&Open...\tCtrl-O", "Open memory card file");
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  auto menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT);

  auto menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuHelp, "&Help");

  SetMenuBar(menuBar);

  CreateStatusBar();
  SetStatusText("");

  // Connect event handlers
  Bind(wxEVT_MENU, &MainFrame::OnOpen, this, wxID_OPEN);
  Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
  Bind(
      wxEVT_MENU, [this](wxCommandEvent &) { Close(true); }, wxID_EXIT);
}

void MainFrame::OnOpen(wxCommandEvent &event)
{
  wxFileDialog openFileDialog(this,
                              _("Open memory card file"),
                              "",
                              "",
                              "ePSXe/PSEmu Pro Memory Card (*.mcr)|*.mcr|PS3 Virtual Memory Card (*.VM1)|*.VM1",
                              wxFD_OPEN | wxFD_FILE_MUST_EXIST);

  if (openFileDialog.ShowModal() != wxID_CANCEL)
  {
    const std::string path = openFileDialog.GetPath().ToStdString();
    mem_card_.loadFile(path);

    if (mem_card_.checkData())
    {
      std::ostringstream log_oss;
      log_oss.imbue(std::locale(""));
      log_oss << "Successfully loaded memory card of size " << std::fixed << mem_card_.size() << " bytes";
      wxLogMessage(wxString(log_oss.str()));
    }
  }
}

void MainFrame::OnAbout(wxCommandEvent &event)
{
  wxMessageBox("https://github.com/rangeli/kutacard", "Kutacard v0.0.0", wxOK | wxICON_INFORMATION);
}

}
