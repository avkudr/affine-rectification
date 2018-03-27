/**
 *  @brief Line fitting to a set of 2D points
 *
 *  @author  Andrey Kudryavtsev (avkudr.github.io)
 *  @author  Rahima Djahel (github:rahma24000)
 *  @date    27/03/2018
 *  @version 1.0
 */

#ifndef FUND_MAT_FITTING_H
#define FUND_MAT_FITTING_H

#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

#include "robust_estim.hpp"

// ----------------------------------------------------------------------------
// ------- Class defining the Problem for RobustEstimator (RANSAC, LMedS)
// ------- for more details visit: /github.com/avkudr/robest
// ----------------------------------------------------------------------------

class FundMatFitting : public robest::EstimationProblem{
    typedef std::pair<cv::Point2d,cv::Point2d> Correspondence;
    typedef std::vector<Correspondence> CorrespondenceVector;
public:
    FundMatFitting(){
        setNbParams(4);
        setNbMinSamples(4);
    }

    void setData(std::vector<cv::Point2d> pts1, std::vector<cv::Point2d> pts2);

    double estimErrorForSample(int i);
    void   estimModelFromSamples(std::vector<int> samplesIdx);

    int getTotalNbSamples() const{
        return (int) correspondences.size();
    }

    long double a,b,c,d,e; // Params

    cv::Mat getResult(){
        cv::Mat F = (cv::Mat_<double>(3,3) << 0,0,a,0,0,b,c,d,1);
        if ( (!T1.empty()) && (!T2.empty())){
            F = T2.t() * F * T1;
        }
        F = F / cv::norm(F); // replaced with the norm: Peter Sturm suggestion
        return F;
    }
private:
    bool isDegenerate(std::vector<int> samplesIdx);
    cv::Mat T1, T2;
    CorrespondenceVector correspondences; // Data
};

#endif // FUND_MAT_FITTING_H
