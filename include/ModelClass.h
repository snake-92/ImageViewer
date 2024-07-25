#ifndef MODELCLASS_H
#define MODELCLASS_H

#include <opencv2/opencv.hpp>
#include <vector>
#include "ListEnumTraitements.h"

class ModelClass
{
    public:
        // structure info filtre + parametre sur image
        // tous les parametres de tous les filtres doivent être ajouté ici
        struct DataImage
        {
            cv::Mat image;
            TREATMENTS filtre;
            bool visible;

            // filtre gaussien
            int size_x;
            int size_y;
            // filtre median
            int size_Med;
            // noyau filtre convolution
            cv::Mat kernel;
            // filtre canny
            int thresh1;
            int thresh2;
        };

        ModelClass();
        ~ModelClass();

        void Init();

        cv::Mat ApplyFilter(DataImage);

        cv::Mat BlurFilter(const cv::Mat& im_in, int size_x=3, int size_y=3, bool brefresh=false);
        cv::Mat MedianFilter(const cv::Mat& im_in, int size_ = 3, bool brefresh=false);
        cv::Mat Convolution(const cv::Mat& im_in, const cv::Mat& kernel, bool brefresh=false);
        cv::Mat CannyFilter(const cv::Mat& im_in, int thresh1, int thresh2, bool brefresh=false);
        cv::Mat Threshold(const cv::Mat& im_in, bool brefresh_=false);

        void AddImageInList(TREATMENTS treatment_, cv::Mat img_);
        void AddImageInList(DataImage);
        cv::Mat HideTreatment(int idx_, bool bhide_);
        cv::Mat RemoveTreatement(int idx_);

    private:
        cv::Mat UpdateModifFilter();

        /**< liste de toutes les traitements et images */
        std::vector<DataImage> m_listImages;
};

#endif // MODELCLASS_H
