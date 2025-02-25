#include "ModelClass.h"

ModelClass::ModelClass()
{
}


ModelClass::~ModelClass()
{
    Init();
}


/** \brief Initialisation du vecteur contenant les informations images + filtre
 */
void ModelClass::Init()
{
    m_listImages.clear();
}


/** \brief Ajout d'une paire traitement + image dans "m_listImages"
 * \param treatment_ : traitement associ� � l'image
 * \param img_ : image
 */
void ModelClass::AddImageInList(TREATMENTS treatment_, cv::Mat img_)
{
    m_listImages.push_back({img_,treatment_,true});
}

void ModelClass::AddImageInList(DataImage data_)
{
    m_listImages.push_back(data_);
}


/** \brief permet de retourner l'image sans le traitement � la position idx_ activ�
 *
 * \param idx_ index du traitement dans la liste
 * \param bhide_ true pour cacher un traitement, false pour remettre le traitement
 * \return image � la fin de la liste sans un traitement
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


/** \brief supprime un �l�ment dans "m_listImages"
 * \param idx_ : index � supprimer
 */
cv::Mat ModelClass::RemoveTreatement(int idx_)
{
    if(idx_ >= m_listImages.size() || m_listImages.empty())
        throw std::invalid_argument("index hors de la liste ou liste d'image vide !");

    // TODO:
}


/** \brief Mise � jour de l'image visible. On reparcours tous les traitements et on applique pas les traitements en hide
 * \return image � afficher
 */
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


/** \brief Applique les filtres avec leur r�glage sur l'image dans DataImage
 * \param
 * \return
 */
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
    case TREATMENTS::ID_FILT_CANNY : im_out = CannyFilter(data_.image, data_.thresh1, data_.thresh2, true);
        break;
    case TREATMENTS::ID_FILT_THRES : im_out = Threshold(data_.image, true);
        break;
    default: return m_listImages[0].image;
        break;
    }

    return im_out;
}


/** \brief Filtre gaussien
 *
 * \param image
 * \param size_x
 * \param size_y
 * \return image filtr�e
 *
 */
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


/** \brief Filtre m�dian
 *
 * \param image
 * \param size_
 * \return image filtr�e
 *
 */
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


/** \brief Filtre convolution
 *
 * \param image
 * \param kernel : noyau de filtrage
 * \return image filtr�e
 *
 */
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


/** \brief
 *
 * \param image d'entree
 * \param thresh1 : seuil 1
 * \param thresh2 : seuil 2
 * \return image filtee
 *
 */
cv::Mat ModelClass::CannyFilter(const cv::Mat& im_in, int thresh1, int thresh2, bool brefresh_)
{
    cv::Mat im_out, greyMat;

    cv::cvtColor(im_in, greyMat, cv::COLOR_BGR2GRAY);
    cv::Canny(greyMat, im_out, thresh1, thresh2);

    if(!brefresh_)
    {
        DataImage data;
        data.filtre = TREATMENTS::ID_FILT_CANNY;
        data.image = im_out;
        data.visible = true;
        data.thresh1 = thresh1;
        data.thresh2 = thresh2;
        AddImageInList(data);
    }

    return im_out;
}


/** \brief Seuillage de l'image
 *
 * \param
 * \param
 * \return
 *
 */
cv::Mat ModelClass::Threshold(const cv::Mat& im_in, bool brefresh_)
{
    cv::Mat im_out, greyMat, im_8bits;
    cv::cvtColor(im_in, greyMat, cv::COLOR_BGR2GRAY); // conversion color -> niveau de gris

    greyMat.convertTo(im_8bits, CV_8UC1); // conversion en image 8 bits
    cv::threshold(im_8bits, im_out, 0, 255, cv::THRESH_OTSU);

    if(!brefresh_)
    {
        DataImage data;
        data.filtre = TREATMENTS::ID_FILT_THRES;
        data.image = im_out;
        data.visible = true;
        AddImageInList(data);
    }

    return im_out;
}
