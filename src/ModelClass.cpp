#include "ModelClass.h"

ModelClass::ModelClass()
{
}


ModelClass::~ModelClass()
{
    Init();
}


void ModelClass::Init()
{
    m_listImages.clear();
}


/** \brief Ajout d'une paire traitement + image dans "m_listImages"
 *
 * \param
 * \param
 * \return
 *
 */
void ModelClass::AddImageInList(TREATMENTS treatment_, cv::Mat img_)
{
    m_listImages.push_back({img_,treatment_,true});
}

void ModelClass::AddImageInList(DataImage data_)
{
    m_listImages.push_back(data_);
}


/** \brief permet de retourner l'image sans le traitement à la position idx_ activé
 *
 * \param idx_ index du traitement dans la liste
 * \param bhide_ true pour cacher un traitement, false pour remettre le traitement
 * \return image à la fin de la liste sans un traitement
 *
 */
cv::Mat ModelClass::HideTreatment(int idx_, bool bhide_)
{
    if(idx_ >= m_listImages.size() || m_listImages.empty())
        throw std::invalid_argument("index hors de la liste ou liste d'image vide !");

    if(idx_ == 0) // image base
        return m_listImages[0].image.clone();

    m_listImages[idx_].visible = !bhide_;
    return UpdateModifFilter();
}


/** \brief Ajout d'une paire traitement + image dans "m_listImages"
 *
 * \param
 * \param
 * \return
 *
 */
cv::Mat ModelClass::RemoveTreatement(int idx_)
{
    if(idx_ >= m_listImages.size() || m_listImages.empty())
        throw std::invalid_argument("index hors de la liste ou liste d'image vide !");

    //if(idx_ == 0) // image base
    //    return;
}


cv::Mat ModelClass::UpdateModifFilter()
{
    if(m_listImages.empty())
        throw std::invalid_argument("liste d'image vide !");

    cv::Mat im_out;
    if(m_listImages.size()>1)
    {
        im_out = m_listImages[0].image.clone();

       for(int i=1; i<m_listImages.size(); i++)
       {
           if(m_listImages[i].visible)
           {
                DataImage data = m_listImages[i];
                data.image = im_out.clone();
                im_out = ApplyFilter(data);
            }
       }
    }

    return im_out;
}


cv::Mat ModelClass::ApplyFilter(DataImage data_)
{
    cv::Mat im_out;
    switch(data_.filtre)
    {
    case TREATMENTS::ID_FILT_BLUR : im_out = BlurFilter(data_.image, data_.size_x, data_.size_y, true);
        break;
    case TREATMENTS::ID_FILT_MEDIAN : im_out = MedianFilter(data_.image, data_.size_Med, true);
        break;
    case TREATMENTS::ID_FILT_CONV : im_out = Convolution(data_.image, data_.kernel, true);
        break;
    default: return m_listImages[0].image;
        break;
    }

    return im_out;
}


cv::Mat ModelClass::BlurFilter(const cv::Mat& im_in, int size_x, int size_y, bool brefresh_)
{
    if(size_x%2 == 0)
        size_x++;
    if(size_y%2 == 0)
        size_y++;

    cv::Mat im_out;
    cv::GaussianBlur(im_in, im_out, cv::Size(size_x, size_y), 3);

    if(!brefresh_)
    {
        DataImage data;
        data.filtre = TREATMENTS::ID_FILT_BLUR;
        data.image = im_out;
        data.visible = true;
        data.size_x = size_x;
        data.size_y = size_y;
        AddImageInList(data);
    }

    return im_out;
}


cv::Mat ModelClass::MedianFilter(const cv::Mat& im_in, int size_, bool brefresh_)
{
    if(size_%2 == 0)
        size_++;

    cv::Mat im_out;
    cv::medianBlur(im_in, im_out, size_);

    if(!brefresh_)
    {
        DataImage data;
        data.filtre = TREATMENTS::ID_FILT_MEDIAN;
        data.image = im_out;
        data.visible = true;
        data.size_Med = size_;
        AddImageInList(data);
    }

    return im_out;
}


cv::Mat ModelClass::Convolution(const cv::Mat& im_in, const cv::Mat& kernel, bool brefresh_)
{
    cv::Mat im_out;

    cv::filter2D(im_in, im_out, -1, kernel);

    if(!brefresh_)
    {
        DataImage data;
        data.filtre = TREATMENTS::ID_FILT_CONV;
        data.image = im_out;
        data.visible = true;
        data.kernel = kernel.clone();
        AddImageInList(data);
    }

    return im_out;
}
