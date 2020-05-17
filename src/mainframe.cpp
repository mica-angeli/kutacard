#include <sstream>
#include <string>
#include <locale>
#include "mainframe.h"

namespace kutacard
{

MainFrame::MainFrame() :
  wxFrame{nullptr, wxID_ANY, "Kutacard"},
  save_buffer_{},
  current_lv_{nullptr},
  mem_card_lvs_{}
{
  // Add menus
  auto *menuFile = new wxMenu;
  menuFile->Append(wxID_OPEN);
  menuFile->AppendSeparator();
  menuFile->Append(wxID_EXIT);

  auto menuHelp = new wxMenu;
  menuHelp->Append(wxID_ABOUT);

  auto menuBar = new wxMenuBar;
  menuBar->Append(menuFile, "&File");
  menuBar->Append(menuHelp, "&Help");

  SetMenuBar(menuBar);

  // Add status bar
  CreateStatusBar();
  SetStatusText("");

  // Add widgets in window
  current_lv_ = new MemoryCardListView(this);
  mem_card_lvs_.push_back(current_lv_);

  // Connect event handlers
  Bind(wxEVT_MENU, &MainFrame::OnOpen, this, wxID_OPEN);
  Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
  Bind(wxEVT_MENU, [this](wxCommandEvent &) { Close(true); }, wxID_EXIT);

  // TODO: Remove this.  Only for debug!
  current_lv_->openMemoryCard("/Users/ricardo/Documents/original.mcr");
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
    current_lv_->openMemoryCard(path);
  }
}

void MainFrame::OnAbout(wxCommandEvent &event)
{
  wxMessageBox("https://github.com/rangeli/kutacard", "Kutacard v0.0.0", wxOK | wxICON_INFORMATION);
}

}
