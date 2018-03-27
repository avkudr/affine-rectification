/**
 *  @brief Line fitting to a set of 2D points
 *
 *  @author  Andrey Kudryavtsev (avkudr.github.io)
 *  @author  Rahima Djahel (github:rahma24000)
 *  @date    27/03/2018
 *  @version 1.0
 */

#ifndef RECTIFIER_AFFINE_H
#define RECTIFIER_AFFINE_H

#include <opencv2/opencv.hpp>

class RectifierAffine
{
public:
    RectifierAffine();
    ~RectifierAffine();

    void init(cv::Mat *im1, cv::Mat *im2, cv::Mat  *F, std::vector<cv::Point2d>  *inliers1, std::vector<cv::Point2d> *inliers2);

    bool isRectificationDone() {return !(imLrect.empty()) && !(imRrect.empty());}

    cv::Mat imLrect;
    cv::Mat imRrect;

    double angleL = 0.0;
    double angleR = 0.0;
    int    shift  = 0;

    void rectify();
    bool isReady();
    cv::Mat get2DRotationMatrixLeft();
    cv::Mat get2DRotationMatrixRight();

    cv::Mat get2DShiftMatrix();

    cv::Mat get2DTransformationMatrixLeft()  { return get2DRotationMatrixLeft()  * get2DShiftMatrix(); }
    cv::Mat get2DTransformationMatrixRight() { return get2DRotationMatrixRight() * get2DShiftMatrix(); }

    void write(cv::FileStorage& fs) const;
    void read(const cv::FileNode& node);
    void getResult(cv::Mat & resL, cv::Mat & resR,std::vector<cv::Point2d> *ptsL,std::vector<cv::Point2d> *ptsR){

        resL = this->imLrect;
        resR = this->imRrect;
        ptsL=this->_inliers1;
        ptsR=this->_inliers2;
    }

private:

    cv::Mat *_imL;
    cv::Mat *_imR;
    cv::Mat *_F;

    std::vector<cv::Point2d> *_inliers1;
    std::vector<cv::Point2d> *_inliers2;
};

#endif // RECTIFIER_AFFINE_H
