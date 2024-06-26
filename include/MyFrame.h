#ifndef MYFRAME_H
#define MYFRAME_H

#include "BufferedBitmap.h"
#include <vector>
#include "ViewModelClass.h"
#include "ListEnumTraitements.h"

class MyFrame: public wxFrame
{
public:
    MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style = wxDEFAULT_FRAME_STYLE);
    virtual ~MyFrame();

    void OnQuit(wxCommandEvent& event);
    void OnLoadImage(wxCommandEvent& event);
    void OnHelp(wxCommandEvent& event);

    void ClickCheckBox(wxCommandEvent& evt);

private:
    ViewModelClass* m_viewModel;

    wxMenuBar *m_menuBarre;
    wxMenu *m_menuFile;
    wxMenu *m_menuTreatment;
    wxMenu *m_menuHelp;

    void InitMenu();
    void InitToolbar();

    BufferedBitmap *m_bitmap;
    wxImage m_Image;
    void UpdateBitmapImage(const wxImage &image);
    void OnZoomIn(wxCommandEvent &event);
    void OnZoomOut(wxCommandEvent &event);

    wxPanel* m_panelFiltres;
    wxBoxSizer* m_SizerListFiltre;
    std::vector<wxCheckBox*> m_listFiltre;
    void UpdateListFiltre(wxString name);
    void FiltreBlur(wxCommandEvent& event);
    void FiltreMedian(wxCommandEvent& event);
    void FiltreConvolution(wxCommandEvent& event);
    void FiltreCanny(wxCommandEvent& event);
    void FiltreThresh(wxCommandEvent& event);
    void FiltreErode(wxCommandEvent& event);
    void FiltreDilate(wxCommandEvent& event);

DECLARE_EVENT_TABLE()
};

#endif // MYFRAME_H
