#ifndef KUTACARD_MAIN_APP_H
#define KUTACARD_MAIN_APP_H

#include <wx/wx.h>

namespace kutacard
{

class MainApp : public wxApp {
public:
  bool OnInit() override;
};

}
#endif //KUTACARD_MAIN_APP_H
