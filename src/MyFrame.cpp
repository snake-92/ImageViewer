#include "wx/wxprec.h"

#ifdef __BORLANDC__
    #pragma hdrstop
#endif

#ifndef WX_PRECOMP
    #include "wx/wx.h"
#endif

#include "wx/artprov.h"
#include "MyFrame.h"
#include <wx/wfstream.h>
#include "DialogFilterView.h"


BEGIN_EVENT_TABLE(MyFrame, wxFrame)
    EVT_MENU(wxID_EXIT,  MyFrame::OnQuit)
    EVT_MENU(wxID_HELP, MyFrame::OnHelp)
    EVT_MENU(wxID_FILE, MyFrame::OnLoadImage)

    EVT_MENU(TREATMENTS::ID_FILT_BLUR, MyFrame::FiltreBlur)
    EVT_MENU(TREATMENTS::ID_FILT_MEDIAN, MyFrame::FiltreMedian)
    EVT_MENU(TREATMENTS::ID_FILT_CONV, MyFrame::FiltreConvolution)
    EVT_MENU(TREATMENTS::ID_FILT_CANNY, MyFrame::FiltreCanny)
    EVT_MENU(TREATMENTS::ID_FILT_THRES, MyFrame::FiltreThresh)
    EVT_MENU(TREATMENTS::ID_FILT_ERODE, MyFrame::FiltreErode)
    EVT_MENU(TREATMENTS::ID_FILT_DILATE, MyFrame::FiltreDilate)

    EVT_TOOL(wxID_ZOOM_IN, MyFrame::OnZoomIn)
    EVT_TOOL(wxID_ZOOM_OUT, MyFrame::OnZoomOut)
END_EVENT_TABLE()




MyFrame::MyFrame(const wxString& title, const wxPoint& pos, const wxSize& size, long style)
: wxFrame(NULL, -1, title, pos, size, style)
{
    SetIcon(wxICON(monicone));
    SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_BTNFACE));

    // menu
    InitMenu();

    // toobar
    InitToolbar();

    // panel image
    auto sizer = new wxBoxSizer(wxHORIZONTAL);
    m_bitmap = new BufferedBitmap(this, wxID_ANY, wxBitmap(wxSize(1, 1)), wxDefaultPosition, FromDIP(wxSize(700, 500)));
    sizer->Add(m_bitmap, 1, wxEXPAND | wxALL, FromDIP(5));

    // liste des filtres
    m_panelFiltres = new wxPanel(this, wxID_ANY, wxDefaultPosition, wxSize(120,400));
    m_panelFiltres->SetBackgroundColour(wxColor(200,200,200));
    sizer->Add(m_panelFiltres, 0, wxEXPAND | wxALL, 10);
    m_SizerListFiltre = new wxBoxSizer(wxVERTICAL);

    this->SetSizerAndFit(sizer);

    m_viewModel = new ViewModelClass(m_Image);
}


MyFrame::~MyFrame()
{
    delete m_viewModel;
}


void MyFrame::InitMenu()
{
    m_menuFile = new wxMenu();

    // sous menu file
    wxMenuItem* fileItem = new wxMenuItem(m_menuFile, wxID_FILE);
    fileItem->SetBitmap(wxArtProvider::GetBitmap("wxART_FILE_OPEN"));
    m_menuFile->Append(fileItem);
    m_menuFile->AppendSeparator();
    wxMenuItem* quitItem = new wxMenuItem(m_menuFile, wxID_EXIT);
    quitItem->SetBitmap(wxArtProvider::GetBitmap("wxART_QUIT"));
    m_menuFile->Append(quitItem);

    // sous menu aide
    m_menuHelp = new wxMenu();
    wxMenuItem* helpItem = new wxMenuItem(m_menuHelp, wxID_HELP);
    helpItem->SetBitmap(wxArtProvider::GetBitmap("wxART_HELP_FOLDER"));
    m_menuHelp->Append(helpItem);

    // sous menu traitements
    m_menuTreatment = new wxMenu();
    m_menuTreatment->Append(TREATMENTS::ID_FILT_BLUR,"Blur");
    m_menuTreatment->Append(TREATMENTS::ID_FILT_MEDIAN,"Filtre Median");
    m_menuTreatment->Append(TREATMENTS::ID_FILT_CONV,"Filtre Convolution");
    m_menuTreatment->Append(TREATMENTS::ID_FILT_CANNY,"Canny");
    m_menuTreatment->Append(TREATMENTS::ID_FILT_THRES,"Threshold");
    m_menuTreatment->Append(TREATMENTS::ID_FILT_ERODE,"Erosion");
    m_menuTreatment->Append(TREATMENTS::ID_FILT_DILATE,"Dilatation");


    m_menuBarre = new wxMenuBar();
    m_menuBarre->Append(m_menuFile,("Fichier"));
    m_menuBarre->Append(m_menuTreatment,("Traitements"));
    m_menuBarre->Append(m_menuHelp,("?"));

    SetMenuBar(m_menuBarre);
}


void MyFrame::InitToolbar()
{
    wxToolBar* toolbar = CreateToolBar();
    toolbar->AddSeparator();

    toolbar->AddTool(wxID_UNDO,_("undo"),wxArtProvider::GetBitmap("wxART_UNDO"));
    toolbar->AddTool(wxID_REDO,_("redo"),wxArtProvider::GetBitmap("wxART_REDO"));

    toolbar->SetToolShortHelp(wxID_UNDO,_("undo"));
    toolbar->SetToolShortHelp(wxID_REDO,_("redo"));

    toolbar->AddSeparator();

    toolbar->AddTool(wxID_ZOOM_IN,_("zoom"),wxArtProvider::GetBitmap("wxART_PLUS"));
    toolbar->AddTool(wxID_ZOOM_OUT,_("dezoom"),wxArtProvider::GetBitmap("wxART_MINUS"));

    toolbar->SetToolShortHelp(wxID_ZOOM_IN,_("zoom"));
    toolbar->SetToolShortHelp(wxID_ZOOM_OUT,_("dezoom"));

    toolbar->Realize();
}


void MyFrame::UpdateBitmapImage(const wxImage &image)
{
    m_bitmap->SetBitmap(wxBitmap(image));
    this->Layout();
}


void MyFrame::OnZoomIn(wxCommandEvent &event)
{
    m_bitmap->ZoomIn();
}


void MyFrame::OnZoomOut(wxCommandEvent &event)
{
    m_bitmap->ZoomOut();
}


void MyFrame::OnLoadImage(wxCommandEvent& WXUNUSED(event))
{
    wxFileDialog openFileDialog(this, _("Charger une image"), "", "",
                       "images (*.bmp;*.gif;*.tif;*.png;*.jpg;*.jpeg)|*.bmp;*.gif;*.tif;*.png;*.jpg;*.jpeg", wxFD_OPEN|wxFD_FILE_MUST_EXIST);

    if (openFileDialog.ShowModal() == wxID_CANCEL)
        return;

    if (!m_Image.LoadFile(openFileDialog.GetPath()))
    {
        wxMessageBox("Erreur d'ouverture de l'image", "Erreur", wxOK | wxICON_ERROR);
        return;
    }

    if(m_Image.HasAlpha())
        m_Image.ClearAlpha();

    UpdateBitmapImage(m_Image);
    m_viewModel->SetImageBase();
}


void MyFrame::UpdateListFiltre(wxString name)
{
    int id=0; // id checkbox
    if(!m_listFiltre.empty())
        id = m_listFiltre.size();

    wxCheckBox* check = new wxCheckBox(m_panelFiltres,id,name);
    check->SetValue(true);
    check->Bind(wxEVT_CHECKBOX, &MyFrame::ClickCheckBox, this);
    m_SizerListFiltre->Add(check, 1, wxEXPAND | wxTop | wxBottom, 5);
    m_listFiltre.push_back(check);
    m_panelFiltres->SetSizerAndFit(m_SizerListFiltre);
}


void MyFrame::ClickCheckBox(wxCommandEvent& evt)
{
    //wxMessageBox(wxString::Format(wxT("%i"), evt.GetId()), "Erreur", wxOK);

    int idx = evt.GetId()+1; // parceque la première image est l'image de base sans filtre dans la liste des images (classe model)
    m_viewModel->HideFilter(idx,!evt.IsChecked());
    UpdateBitmapImage(m_Image);
}


void MyFrame::FiltreBlur(wxCommandEvent& WXUNUSED(event))
{
    if(!m_Image.IsOk())
        return;

    DialogFilterView dlg(this, wxID_ANY, "Configuration filtre gaussien");
    dlg.DialogBlur();
    if(dlg.ShowModal() == wxID_OK)
    {
        int x = dlg.GetSizeX();
        int y = dlg.GetSizeY();

        UpdateListFiltre("Blur");
        m_viewModel->Blur(x,y);
        UpdateBitmapImage(m_Image);
    }
}


void MyFrame::FiltreMedian(wxCommandEvent& WXUNUSED(event))
{
    if(!m_Image.IsOk())
        return;

    DialogFilterView dlg(this, wxID_ANY, "Configuration filtre median");
    dlg.DialogMedian();
    if(dlg.ShowModal() == wxID_OK)
    {
        int siz = dlg.GetSize();

        UpdateListFiltre("Median");
        m_viewModel->MedianBlur(siz);
        UpdateBitmapImage(m_Image);
    }
}


void MyFrame::FiltreConvolution(wxCommandEvent& event)
{
    if(!m_Image.IsOk())
        return;

    DialogFilterView dlg(this, wxID_ANY, "Configuration filtre Convolution");
    dlg.DialogConvolution();
    if(dlg.ShowModal() == wxID_OK)
    {
        std::pair<int, wxString> res = dlg.GetTypeConvolution();
        UpdateListFiltre("Convolution "+res.second);
        m_viewModel->Convolution(res.first);
        UpdateBitmapImage(m_Image);
    }

}


void MyFrame::FiltreCanny(wxCommandEvent& WXUNUSED(event))
{
    if(!m_Image.IsOk())
        return;

    DialogFilterView dlg(this, wxID_ANY, "Configuration filtre de Canny");
    dlg.DialogCanny();
    if(dlg.ShowModal() == wxID_OK)
    {
        std::pair<int, int> res = dlg.Get2Threshold();
        UpdateListFiltre("Canny");
        m_viewModel->CannyFilt(res.first,res.second);
        UpdateBitmapImage(m_Image);
    }
}


void MyFrame::FiltreThresh(wxCommandEvent& WXUNUSED(event))
{
    if(!m_Image.IsOk())
        return;

    UpdateListFiltre("Threshold");
    m_viewModel->Threshold();
    UpdateBitmapImage(m_Image);
}


void MyFrame::FiltreErode(wxCommandEvent& event)
{
    if(!m_Image.IsOk())
        return;

    UpdateListFiltre("Erode");
}


void MyFrame::FiltreDilate(wxCommandEvent& event)
{
    if(!m_Image.IsOk())
        return;

    UpdateListFiltre("Dilate");
}


void MyFrame::OnHelp(wxCommandEvent& WXUNUSED(event))
{
    //SetTitle("Nouveau Titre");
}


void MyFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
    int answer = wxMessageBox(_("Voulez-vous vraiment quitter l'application ?"), _("Fermeture"), wxYES_NO);
    if(answer == wxYES)
    {
        Close(true);
    }
}
