#include <sstream>
#include <string>
#include <locale>
#include "mainframe.h"

namespace kutacard
{

MainFrame::MainFrame() :
  wxFrame{nullptr, wxID_ANY, "Kutacard"},
  current_card_{0},
  cards_{},
  save_buffer_{},
  mem_card_lv_{}
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
  mem_card_lv_ = new MemoryCardListView(this);

  // Connect event handlers
  Bind(wxEVT_MENU, &MainFrame::OnOpen, this, wxID_OPEN);
  Bind(wxEVT_MENU, &MainFrame::OnAbout, this, wxID_ABOUT);
  Bind(
      wxEVT_MENU, [this](wxCommandEvent &) { Close(true); }, wxID_EXIT);

  // TODO: Remove this.  Only for debug!
  openMemoryCard("/Users/ricardo/Documents/original.mcr");
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
    openMemoryCard(path);
  }
}

void MainFrame::openMemoryCard(const std::string &path)
{
  if(cards_.empty())
  {
    cards_.emplace_back(path);
  }
  else
  {
    cards_[current_card_] = ps1::MemoryCard(path);
  }
  
  auto& mem_card = cards_[current_card_];

  if (!mem_card.checkData()) {
    return;
  }

  std::ostringstream log_oss;
  log_oss.imbue(std::locale(""));
  log_oss << "Successfully loaded memory card of size " << std::fixed << mem_card.size() << " bytes";
  wxLogMessage(wxString(log_oss.str()));

  mem_card_lv_->update(mem_card);
}

void MainFrame::OnAbout(wxCommandEvent &event)
{
  wxMessageBox("https://github.com/rangeli/kutacard", "Kutacard v0.0.0", wxOK | wxICON_INFORMATION);
}

}
