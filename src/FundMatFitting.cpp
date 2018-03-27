#include "FundMatFitting.hpp"

void FundMatFitting::setData(std::vector<cv::Point2d> pts1, std::vector<cv::Point2d> pts2){

    for (int i = 0; i < pts1.size(); i++){
        correspondences.push_back(Correspondence(pts1[i],pts2[i]));
    }
}

double FundMatFitting::estimErrorForSample(int i)
{
    double x2 = correspondences[i].second.x;
    double y2 = correspondences[i].second.y;
    double x1 = correspondences[i].first.x;
    double y1 = correspondences[i].first.y;

    double error = (a*x2 + b*y2 +c*x1 + d*y1 + 1) / sqrt(a*a + b*b + c*c + d*d); //H&Z, p.350
    return error;
}

void FundMatFitting::estimModelFromSamples(std::vector<int> samplesIdx)
{
    int nbPts = 0;
    nbPts = (int)samplesIdx.size();

    cv::Mat W = cv::Mat::zeros(4, nbPts,  cv::DataType<double>::type);

    for (int i = 0; i < nbPts; i++){
        W.at<double>(0,i) = correspondences[samplesIdx[i]].second.x;
        W.at<double>(1,i) = correspondences[samplesIdx[i]].second.y;
        W.at<double>(2,i) = correspondences[samplesIdx[i]].first.x;
        W.at<double>(3,i) = correspondences[samplesIdx[i]].first.y;
    }

    cv::Mat meanW = cv::Mat::zeros(4, 1, cv::DataType<double>::type);

    meanW.at<double>(0) = cv::mean(W.row(0))[0];
    meanW.at<double>(1) = cv::mean(W.row(1))[0];
    meanW.at<double>(2) = cv::mean(W.row(2))[0];
    meanW.at<double>(3) = cv::mean(W.row(3))[0];

    for (int i = 0; i < nbPts; i++){
        for (int j = 0; j < 4; j++){
            W.at<double>(j,i) = W.at<double>(j,i) - meanW.at<double>(j);
        }
    }

    cv::Mat S, U, Vt;
    cv::SVD::compute(W.t(), S, U, Vt);
    cv::Mat V = Vt.t();
    cv::Mat N = V.col(V.cols - 1);

    double e = cv::Mat(-1*N.t()*meanW).at<double>(0,0);
    a = N.at<double>(0) / e;
    b = N.at<double>(1) / e;
    c = N.at<double>(2) / e;
    d = N.at<double>(3) / e;
}

bool FundMatFitting::isDegenerate(std::vector<int> samplesIdx)
{
    return false;
}
