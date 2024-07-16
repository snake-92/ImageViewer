#include "DialogFilterView.h"
#include <wx/valnum.h>

#define VAL_MAX 1000
#define VAL_MIN 1


DialogFilterView::DialogFilterView(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint &pos, const wxSize &size, long style)
 : wxDialog(parent, id, title, pos, size, style)
{
    //ctor
    m_sizeX = 3;
    m_sizeY = 3;
    m_sizeMed = 3;
    m_type_convolution=std::make_pair(1,"moyenneur");
    m_thresholds=std::make_pair(100,200);
}


/** \brief Fenetre de dialog pour le filtre gaussien
 */
void DialogFilterView::DialogBlur()
{
    // permet d'autoriser uniquement la saisi de nombre dans wxTextCtrl
    wxFloatingPointValidator<int> val_x(0,&m_sizeX,wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<int> val_y(0,&m_sizeY,wxNUM_VAL_DEFAULT);
    val_x.SetRange(VAL_MIN, VAL_MAX);
    val_y.SetRange(VAL_MIN, VAL_MAX);

    auto ctrlX = new wxTextCtrl(this, wxID_ANY, "3", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, val_x, wxTextCtrlNameStr);
    auto ctrlY = new wxTextCtrl(this, wxID_ANY, "3", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, val_y, wxTextCtrlNameStr);

    auto txt_x = new wxStaticText(this,wxID_ANY,"size x ");
    auto txt_y = new wxStaticText(this,wxID_ANY,"size y ");

    auto mainHorX = new wxBoxSizer(wxHORIZONTAL);
    mainHorX->Add(txt_x, 0, wxEXPAND | wxRIGHT, 10);
    mainHorX->Add(ctrlX, 0, wxEXPAND | wxALL, 1);

    auto mainHorY = new wxBoxSizer(wxHORIZONTAL);
    mainHorY->Add(txt_y, 0, wxEXPAND | wxRIGHT, 10);
    mainHorY->Add(ctrlY, 0, wxEXPAND | wxALL, 1);

    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(mainHorX, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(mainHorY, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_CENTER | wxALL, FromDIP(10));
    this->SetSizerAndFit(mainSizer);
}

int DialogFilterView::GetSizeX()
{
    return m_sizeX;
}

int DialogFilterView::GetSizeY()
{
    return m_sizeY;
}


/** \brief Fenetre de dialog pour le filtre median
 */
void DialogFilterView::DialogMedian()
{
    // permet d'autoriser uniquement la saisi de nombre dans wxTextCtrl
    wxFloatingPointValidator<int> val(0,&m_sizeMed,wxNUM_VAL_DEFAULT);
    val.SetRange(VAL_MIN, VAL_MAX);

    auto ctrlSize = new wxTextCtrl(this, wxID_ANY, "3", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, val, wxTextCtrlNameStr);

    auto txt_size = new wxStaticText(this,wxID_ANY,"size ");

    auto mainHor = new wxBoxSizer(wxHORIZONTAL);
    mainHor->Add(txt_size, 0, wxEXPAND | wxRIGHT, 10);
    mainHor->Add(ctrlSize, 0, wxEXPAND | wxALL, 1);

    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(mainHor, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_CENTER | wxALL, FromDIP(10));
    this->SetSizerAndFit(mainSizer);
}

int DialogFilterView::GetSize()
{
    return m_sizeMed;
}


/** \brief Fenetre de dialog pour le filtre de convolution
 */
void DialogFilterView::DialogConvolution()
{

    auto rationMean = new wxRadioButton(this, 1, "Moyenneur");
    rationMean->Bind(wxEVT_RADIOBUTTON, &DialogFilterView::ClickRatioButton, this);

    auto rationSharpen = new wxRadioButton(this, 2, "Sharpen");
    rationSharpen->Bind(wxEVT_RADIOBUTTON, &DialogFilterView::ClickRatioButton, this);

    auto rationLaplacien = new wxRadioButton(this, 3, "Laplacien");
    rationLaplacien->Bind(wxEVT_RADIOBUTTON, &DialogFilterView::ClickRatioButton, this);

    auto rationSobelX = new wxRadioButton(this, 4, "Sobel horizontal");
    rationSobelX->Bind(wxEVT_RADIOBUTTON, &DialogFilterView::ClickRatioButton, this);

    auto rationSobelY = new wxRadioButton(this, 5, "Sobel vertical");
    rationSobelY->Bind(wxEVT_RADIOBUTTON, &DialogFilterView::ClickRatioButton, this);

    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(rationMean, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(rationSharpen, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(rationLaplacien, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(rationSobelX, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(rationSobelY, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_CENTER | wxALL, FromDIP(10));
    this->SetSizerAndFit(mainSizer);
}

void DialogFilterView::ClickRatioButton(wxCommandEvent& event)
{
    switch(event.GetId())
    {
    case 1: m_type_convolution = std::make_pair(event.GetId(), "Moyenneur");
        break;
    case 2: m_type_convolution = std::make_pair(event.GetId(), "Sharpen");
        break;
    case 3: m_type_convolution = std::make_pair(event.GetId(), "Laplacien");
        break;
    case 4: m_type_convolution = std::make_pair(event.GetId(), "Sobel horizontal");
        break;
    case 5: m_type_convolution = std::make_pair(event.GetId(), "Sobel vertical");
        break;
    };

    //wxMessageBox(wxString::Format(wxT("%i"), event.GetId()), "Erreur", wxOK);
}

std::pair<int,wxString> DialogFilterView::GetTypeConvolution()
{
    return m_type_convolution;
}


/** \brief Fenetre de dialog pour le filtre de Canny
 */
void DialogFilterView::DialogCanny()
{
    // permet d'autoriser uniquement la saisi de nombre dans wxTextCtrl
    wxFloatingPointValidator<int> val_x(0,&m_thresholds.first,wxNUM_VAL_DEFAULT);
    wxFloatingPointValidator<int> val_y(0,&m_thresholds.second,wxNUM_VAL_DEFAULT);
    val_x.SetRange(VAL_MIN, VAL_MAX);
    val_y.SetRange(VAL_MIN, VAL_MAX);

    auto ctrlX = new wxTextCtrl(this, wxID_ANY, "100", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, val_x, wxTextCtrlNameStr);
    auto ctrlY = new wxTextCtrl(this, wxID_ANY, "200", wxDefaultPosition, wxDefaultSize, wxTE_PROCESS_ENTER, val_y, wxTextCtrlNameStr);

    auto txt_x = new wxStaticText(this,wxID_ANY,"Threshold 1 ");
    auto txt_y = new wxStaticText(this,wxID_ANY,"Threshold 2 ");

    auto mainHorX = new wxBoxSizer(wxHORIZONTAL);
    mainHorX->Add(txt_x, 0, wxEXPAND | wxRIGHT, 10);
    mainHorX->Add(ctrlX, 0, wxEXPAND | wxALL, 1);

    auto mainHorY = new wxBoxSizer(wxHORIZONTAL);
    mainHorY->Add(txt_y, 0, wxEXPAND | wxRIGHT, 10);
    mainHorY->Add(ctrlY, 0, wxEXPAND | wxALL, 1);

    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(mainHorX, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(mainHorY, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_CENTER | wxALL, FromDIP(10));
    this->SetSizerAndFit(mainSizer);
}

std::pair<int,int> DialogFilterView::Get2Threshold()
{
    return m_thresholds;
}
