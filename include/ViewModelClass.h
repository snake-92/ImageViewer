#ifndef VIEWMODELCLASS_H
#define VIEWMODELCLASS_H

#include "ModelClass.h"
#include "wx/wx.h"

class ViewModelClass
{
    public:
        ViewModelClass(wxImage& image);
        ~ViewModelClass();

        void SetImageBase();
        void Blur(int size_x, int size_y);
        void MedianBlur(int size_);
        void Convolution();

        void HideFilter(int idx_, bool bhide_);

    protected:
        ModelClass* m_model;

    private:
        wxImage& m_Image;

        cv::Mat ConvertWxImageToCvMat(wxImage& image_);
        void CopyCvMatToWxImage(const cv::Mat& img_);
};

#endif // VIEWMODELCLASS_H
