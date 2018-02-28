#ifndef RECTIFIER_AFFINE_H
#define RECTIFIER_AFFINE_H

#include <iostream>
#include <opencv2/opencv.hpp>

class RectifierAffine
{
public:
    Rectifier();
    Rectifier(cv::Mat* im1, cv::Mat* im2, FundMat * F, std::vector<cv::Point2d> * inliers1, std::vector<cv::Point2d> * inliers2);
    ~Rectifier();

    void init(cv::Mat* im1, cv::Mat* im2, FundMat * F, std::vector<cv::Point2d> * inliers1, std::vector<cv::Point2d> * inliers2);

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

private:

    cv::Mat* _imL;
    cv::Mat* _imR;
    FundMat *_F;

    std::vector<cv::Point2d> *_inliers1;
    std::vector<cv::Point2d> *_inliers2;
};

#endif // AFFINE_RECTIFIER_H
