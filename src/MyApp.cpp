#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "MyApp.h"
#include "MyFrame.h"

IMPLEMENT_APP(MyApp)

bool MyApp::OnInit()
{
    wxInitAllImageHandlers();

    MyFrame *frame = new MyFrame("image viewer", wxPoint(150, 150), wxSize(640, 512));
    frame->Show(true);

    return true;
}
