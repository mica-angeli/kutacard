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
  mem_card_lv_ = new wxListView(this);
  mem_card_lv_->AppendColumn("Block #");
  mem_card_lv_->AppendColumn("Block Type");
  mem_card_lv_->AppendColumn("Region");
  mem_card_lv_->AppendColumn("Title");
  mem_card_lv_->AppendColumn("Product Code");
  mem_card_lv_->AppendColumn("Identifier");


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
  mem_card_.loadFile(path);

  if (!mem_card_.checkData()) {
    return;
  }

  std::ostringstream log_oss;
  log_oss.imbue(std::locale(""));
  log_oss << "Successfully loaded memory card of size " << std::fixed << mem_card_.size() << " bytes";
  wxLogMessage(wxString(log_oss.str()));

  mem_card_lv_->DeleteAllItems();
  for(int i = 0; i < mem_card_.dir_frames_.size(); i++)
  {
    using BlockType = MemoryCard::DirectoryFrame::BlockType;
    using TerritoryCode = MemoryCard::DirectoryFrame::TerritoryCode;

    const auto& dir_frame = mem_card_.dir_frames_[i];

    std::string block_type;
    switch(dir_frame.block_type)
    {
      case BlockType::Initial:
        block_type = "INIT";
        break;
      case BlockType::Identity:
        block_type = "IDENT";
        break;
      case BlockType::Medial:
        block_type = "MEDIAL";
        break;
      case BlockType::Final:
        block_type = "FINAL";
        break;
      case BlockType::Formatted:
        block_type = "FORMAT";
        break;
      case BlockType::Reserved:
        block_type = "RESRVD";
        break;
      case BlockType::Deleted_Initial:
      case BlockType::Deleted_Medial:
      case BlockType::Deleted_Final:
        block_type = "DELETED";
        break;
    }

    std::string region;
    switch(dir_frame.territory)
    {
      case TerritoryCode::American:
        region = "US";
        break;

      case TerritoryCode::European:
        region = "EU";
        break;

      case TerritoryCode::Japanese:
        region = "JP";
        break;
    }

    int col = 0;
    mem_card_lv_->InsertItem(i, "");
    mem_card_lv_->SetItem(i, col++, std::to_string(i));
    mem_card_lv_->SetItem(i, col++, block_type);
    mem_card_lv_->SetItem(i, col++, region);
    mem_card_lv_->SetItem(i, col++, mem_card_.getSaveTitle(i));
    mem_card_lv_->SetItem(i, col++, dir_frame.license_code);
    mem_card_lv_->SetItem(i, col++, dir_frame.save_code);
  }
}

void MainFrame::OnAbout(wxCommandEvent &event)
{
  wxMessageBox("https://github.com/rangeli/kutacard", "Kutacard v0.0.0", wxOK | wxICON_INFORMATION);
}

}
