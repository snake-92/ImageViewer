#include "ViewModelClass.h"


ViewModelClass::ViewModelClass(wxImage& image) : m_Image(image)
{
    m_model = new ModelClass();
}


ViewModelClass::~ViewModelClass()
{
    delete m_model;
}


void ViewModelClass::SetImageBase()
{
    cv::Mat img = ConvertWxImageToCvMat(m_Image);
    m_model->Init();
    m_model->AddImageInList(TREATMENTS::ID_FILT_NONE, img);
}


cv::Mat ViewModelClass::ConvertWxImageToCvMat(wxImage& image_)
{
    cv::Mat img = cv::Mat::zeros(image_.GetHeight(), image_.GetWidth(), CV_8UC3);

    for(int y=0;y<image_.GetHeight();y++)
    {
        for(int x=0;x<image_.GetWidth();x++)
        {
            img.at<cv::Vec3b>(y,x)[2] = image_.GetRed(x,y);
            img.at<cv::Vec3b>(y,x)[1] = image_.GetGreen(x,y);
            img.at<cv::Vec3b>(y,x)[0] = image_.GetBlue(x,y);
        }
    }

    return img;
}


void ViewModelClass::CopyCvMatToWxImage(const cv::Mat& img_)
{
    for(int y=0;y<img_.rows;y++)
    {
        for(int x=0;x<img_.cols;x++)
        {
            m_Image.SetRGB(x, y, img_.at<cv::Vec3b>(y,x)[2], img_.at<cv::Vec3b>(y,x)[1], img_.at<cv::Vec3b>(y,x)[0]);
        }
    }
}


void ViewModelClass::HideFilter(int idx_, bool bhide_)
{
    cv::Mat img = m_model->HideTreatment(idx_, bhide_);

    // convertion en wxImage
    CopyCvMatToWxImage(img);
}


void ViewModelClass::Blur(int size_x, int size_y)
{
    // conversion en cv::Mat
    cv::Mat img = ConvertWxImageToCvMat(m_Image);
    cv::Mat imOut = m_model->BlurFilter(img , size_x, size_y);

    // convertion en wxImage
    CopyCvMatToWxImage(imOut);
}


void ViewModelClass::MedianBlur(int size_)
{
    // conversion en cv::Mat
    cv::Mat img = ConvertWxImageToCvMat(m_Image);
    cv::Mat imOut = m_model->MedianFilter(img , size_);

    // convertion en wxImage
    CopyCvMatToWxImage(imOut);
}


void ViewModelClass::Convolution()
{
    // conversion en cv::Mat
    cv::Mat img = ConvertWxImageToCvMat(m_Image);

    // construction du kernel
    cv::Mat kernel(1, 3, CV_8U);
    kernel.at<uchar>(0,0) = 1;
    kernel.at<uchar>(0,1) = 0;
    kernel.at<uchar>(0,2) = -1;

    cv::Mat imOut = m_model->Convolution(img, kernel);

    // convertion en wxImage
    CopyCvMatToWxImage(imOut);
}
