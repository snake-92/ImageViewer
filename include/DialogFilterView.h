#ifndef DIALOGFILTERVIEW_H
#define DIALOGFILTERVIEW_H

#include <wx/wx.h>

class DialogFilterView : public wxDialog
{
    public:
        DialogFilterView(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = "Dialog", const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);

        void DialogBlur();
        int GetSizeX();
        int GetSizeY();

    protected:

    private:
        wxTextCtrl* m_sizeX;
        wxTextCtrl* m_sizeY;
};

#endif // DIALOGFILTERVIEW_H
