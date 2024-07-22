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


void ViewModelClass::CopyCvMatToWxImage(const cv::Mat& img_, bool bOneChannel)
{
    if(bOneChannel)
    {
        for(int y=0;y<img_.rows;y++)
        {
            for(int x=0;x<img_.cols;x++)
            {
                m_Image.SetRGB(x, y, img_.at<uchar>(y,x), img_.at<uchar>(y,x), img_.at<uchar>(y,x));
            }
        }
    }
    else
    {
        for(int y=0;y<img_.rows;y++)
        {
            for(int x=0;x<img_.cols;x++)
            {
                m_Image.SetRGB(x, y, img_.at<cv::Vec3b>(y,x)[2], img_.at<cv::Vec3b>(y,x)[1], img_.at<cv::Vec3b>(y,x)[0]);
            }
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



cv::Mat ViewModelClass::GetKernelConv(int type_)
{
    CONVOLUTION nameType = static_cast<CONVOLUTION>(type_);

    cv::Mat kernel;

    switch(nameType)
    {
        case CONVOLUTION::MEAN : kernel = cv::Mat::ones(3,3,CV_32F)/9.0f;
                                 break;
        case CONVOLUTION::SHARPEN :  kernel = cv::Mat::ones(3,3,CV_32F);
                                    kernel.at<float>(0,1) = -3;
                                    kernel.at<float>(1,0) = -3;     // 1, -3, 1,
                                    kernel.at<float>(2,1) = -3;     // -3, 9, -3,
                                    kernel.at<float>(1,2) = -3;     // 1, -3, 1
                                    kernel.at<float>(1,1) = 9;
                                    break;
        case CONVOLUTION::LAPLACIEN : kernel = cv::Mat::zeros(3,3,CV_32F);
                                    kernel.at<float>(0,1) = 1;
                                    kernel.at<float>(1,0) = 1;      // 0, 1, 0,
                                    kernel.at<float>(2,1) = 1;      // 1, -4, 1,
                                    kernel.at<float>(1,2) = 1;      // 0, 1, 0
                                    kernel.at<float>(1,1) = -4;
                                    break;
        case CONVOLUTION::SOBEL_X : kernel = cv::Mat::zeros(3,3,CV_32F);
                                    kernel.at<float>(0,0) = -1;
                                    kernel.at<float>(0,1) = -2;         // -1, -2, -1,
                                    kernel.at<float>(0,2) = -1;         // 0, 0, 0,
                                    kernel.at<float>(2,0) = 1;          // 1, 2, 1
                                    kernel.at<float>(2,1) = 2;
                                    kernel.at<float>(2,2) = 1;
                                    break;
        case CONVOLUTION::SOBEL_Y : kernel = cv::Mat::zeros(3,3,CV_32F);
                                    kernel.at<float>(0,0) = -1;
                                    kernel.at<float>(1,0) = -2;         // -1, 0, 1,
                                    kernel.at<float>(2,0) = -1;         // -2, 0, 2,
                                    kernel.at<float>(0,2) = 1;          // -1, 0, 1
                                    kernel.at<float>(1,2) = 2;
                                    kernel.at<float>(2,2) = 1;
                                    break;
        default:
            //throw std::exception("filtre convolution non existant");
            break;
    };

    return kernel;
}


void ViewModelClass::Convolution(int type_)
{
    // conversion en cv::Mat
    cv::Mat img = ConvertWxImageToCvMat(m_Image);

    // construction du kernel
    cv::Mat kernel = GetKernelConv(type_);

    cv::Mat imOut = m_model->Convolution(img, kernel);

    // convertion en wxImage
    CopyCvMatToWxImage(imOut);
}


void ViewModelClass::CannyFilt(int thresh1, int thresh2)
{
     // conversion en cv::Mat
    cv::Mat img = ConvertWxImageToCvMat(m_Image);

    cv::Mat imOut = m_model->CannyFilter(img, thresh1, thresh2);

    // convertion en wxImage
    CopyCvMatToWxImage(imOut, true);
}
