#ifndef DIALOGFILTERVIEW_H
#define DIALOGFILTERVIEW_H

#include <wx/wx.h>
#include <utility>

class DialogFilterView : public wxDialog
{
    public:
        DialogFilterView(wxWindow* parent, wxWindowID id = wxID_ANY, const wxString& title = "Dialog", const wxPoint &pos = wxDefaultPosition, const wxSize &size = wxDefaultSize, long style = wxDEFAULT_DIALOG_STYLE);

        void DialogBlur();
        int GetSizeX();
        int GetSizeY();

        void DialogMedian();
        int GetSize();

        void DialogConvolution();
        std::pair<int,wxString> GetTypeConvolution();

        void DialogCanny();
        std::pair<int, int> Get2Threshold();

    private:
        int m_sizeX;
        int m_sizeY;

        int m_sizeMed;

        std::pair<int,wxString> m_type_convolution;

        std::pair<int,int> m_thresholds;

    private:
        void ClickRatioButton(wxCommandEvent& event);
};

#endif // DIALOGFILTERVIEW_H
