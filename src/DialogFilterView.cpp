#include "DialogFilterView.h"

DialogFilterView::DialogFilterView(wxWindow* parent, wxWindowID id, const wxString& title, const wxPoint &pos, const wxSize &size, long style)
 : wxDialog(parent, id, title, pos, size, style)
{
    //ctor
}


void DialogFilterView::DialogBlur()
{
    m_sizeX = new wxTextCtrl(this, wxID_ANY, "3");
    m_sizeY = new wxTextCtrl(this, wxID_ANY, "3");

    auto txt_x = new wxStaticText(this,wxID_ANY,"size x ");
    auto txt_y = new wxStaticText(this,wxID_ANY,"size y ");

    auto mainHorX = new wxBoxSizer(wxHORIZONTAL);
    mainHorX->Add(txt_x, 0, wxEXPAND | wxRIGHT, 10);
    mainHorX->Add(m_sizeX, 0, wxEXPAND | wxALL, 1);

    auto mainHorY = new wxBoxSizer(wxHORIZONTAL);
    mainHorY->Add(txt_y, 0, wxEXPAND | wxRIGHT, 10);
    mainHorY->Add(m_sizeY, 0, wxEXPAND | wxALL, 1);

    auto mainSizer = new wxBoxSizer(wxVERTICAL);
    mainSizer->Add(mainHorX, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(mainHorY, 0, wxEXPAND | wxALL, 10);
    mainSizer->Add(CreateButtonSizer(wxOK | wxCANCEL), 0, wxALIGN_CENTER | wxALL, FromDIP(10));
    this->SetSizerAndFit(mainSizer);
}

int DialogFilterView::GetSizeX()
{
    return wxAtoi(m_sizeX->GetValue());
}
int DialogFilterView::GetSizeY()
{
    return wxAtoi(m_sizeY->GetValue());
}
