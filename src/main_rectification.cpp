/**
 *  @brief Line fitting to a set of 2D points
 *
 *  @author  Rahima Djahel (github:rahma24000)
 *  @date    27/03/2018
 *  @version 1.0
 */

#include <iostream>

#include "FundMatFitting.hpp"
#include "RectifierAffine.hpp"

cv::Mat plotStereoWithEpilines(cv::Mat img1,cv::Mat img2,cv::Mat F,std::vector<cv::Point2d> pts1,std::vector<cv::Point2d> pts2)
{
    cv::Mat epilines1;
    cv::Mat epilines2 ;

    cv::computeCorrespondEpilines(pts1,1, F,epilines2);
    cv::computeCorrespondEpilines(pts2, 2, F,epilines1);

    CV_Assert(img1.size() == img2.size() && img1.type() == img2.type());
    cv::Mat outImg(img1.rows, img1.cols*2, CV_8UC3);
    cv::Rect rect1(0,0, img1.cols, img1.rows);
    cv::Rect rect2(img1.cols, 0, img2.cols, img2.rows);

    if (img1.type() == CV_8U)
    {
        cv::cvtColor(img1, outImg(rect1), CV_GRAY2BGR);
        cv::cvtColor(img2, outImg(rect2), CV_GRAY2BGR);
    }
    else
    {
        img1.copyTo(outImg(rect1));
        img2.copyTo(outImg(rect2));
    }

    cv::RNG rng;

    for(int i=0; i<pts1.size(); i++)
    {
        cv::Scalar color = cv::Scalar(rng.uniform(0,255), rng.uniform(0, 255), rng.uniform(0, 255));

        std::vector<cv::Point> intesecPoints;
        intesecPoints.push_back(cv::Point(0,-epilines2.at<double>(i,2)/epilines2.at<double>(i,1)));
        intesecPoints.push_back(cv::Point(img2.cols,-(epilines2.at<double>(i,2)+epilines2.at<double>(i,0)*img2.cols)/epilines2.at<double>(i,1)));
        intesecPoints.push_back(cv::Point(-epilines2.at<double>(i,2)/epilines2.at<double>(i,0),0));
        intesecPoints.push_back(cv::Point(-(epilines2.at<double>(i,2)+epilines2.at<double>(i,1)*img2.rows)/epilines2.at<double>(i,0),img2.rows));

        if ( intesecPoints[3].x < 0 || intesecPoints[3].x > img2.cols )
            intesecPoints.erase( intesecPoints.begin() + 3);
        if ( intesecPoints[2].x < 0 || intesecPoints[2].x > img2.cols )
            intesecPoints.erase( intesecPoints.begin() + 2);
        if ( intesecPoints[1].y < 0 || intesecPoints[1].y > img2.rows )
            intesecPoints.erase( intesecPoints.begin() + 1);
        if ( intesecPoints[0].y < 0 || intesecPoints[0].y > img2.rows )
            intesecPoints.erase( intesecPoints.begin() + 0);

        cv::line(outImg(rect2),
                 intesecPoints[0],
                intesecPoints[1],
                color, 1, CV_AA);
        cv::circle(outImg(rect2), pts2[i], 3, color, -1, CV_AA);

        intesecPoints.clear();

        intesecPoints.push_back(cv::Point(0,-epilines1.at<double>(i,2)/epilines1.at<double>(i,1)));
        intesecPoints.push_back(cv::Point(img1.cols,-(epilines1.at<double>(i,2)+epilines1.at<double>(i,0)*img1.cols)/epilines1.at<double>(i,1)));
        intesecPoints.push_back(cv::Point(-epilines1.at<double>(i,2)/epilines1.at<double>(i,0),0));
        intesecPoints.push_back(cv::Point(-(epilines1.at<double>(i,2)+epilines1.at<double>(i,1)*img1.rows)/epilines1.at<double>(i,0),img1.rows));

        if ( intesecPoints[3].x < 0 || intesecPoints[3].x > img1.cols )
            intesecPoints.erase( intesecPoints.begin() + 3);
        if ( intesecPoints[2].x < 0 || intesecPoints[2].x > img1.cols )
            intesecPoints.erase( intesecPoints.begin() + 2);
        if ( intesecPoints[1].y < 0 || intesecPoints[1].y > img1.rows )
            intesecPoints.erase( intesecPoints.begin() + 1);
        if ( intesecPoints[0].y < 0 || intesecPoints[0].y > img1.rows )
            intesecPoints.erase( intesecPoints.begin() + 0);

        cv::line(outImg(rect1),
                 intesecPoints[0],
                intesecPoints[1],
                color, 1, CV_AA);
        cv::circle(outImg(rect1), pts1[i], 3, color, -1, CV_AA);

        intesecPoints.clear();
    }

    return outImg;
}

int main()
{
    //Step 1. Set parameters and load the images

    double detectionThres = 0.004;
    double nnMatchRatio = 0.5;
    cv::Mat img1 = imread("../data/potamogeton1.jpg", cv::IMREAD_GRAYSCALE);
    cv::Mat img2 = imread("../data/potamogeton2.jpg", cv::IMREAD_GRAYSCALE);

    //Step 2. Extract features AKAZE

    std::vector<cv::KeyPoint> kpts1, kpts2;
    cv::Mat desc1, desc2;
    cv::Ptr<cv::AKAZE> akaze = cv::AKAZE::create();
    akaze->setThreshold(detectionThres);

    akaze->detectAndCompute(img1, cv::noArray(), kpts1, desc1);
    akaze->detectAndCompute(img2, cv::noArray(), kpts2, desc2);

    cv::BFMatcher matcher(cv::NORM_HAMMING);
    std::vector<std::vector<cv::DMatch> > nn_matches;
    matcher.knnMatch(desc1, desc2, nn_matches, 2);
    std::vector<cv::KeyPoint> matched1, matched2;
    std::vector<cv::DMatch> good_matches;
    for(size_t i = 0; i < nn_matches.size(); i++) {
        cv::DMatch match = nn_matches[i][0];
        float dist1 = nn_matches[i][0].distance;
        float dist2 = nn_matches[i][1].distance;
        if(dist1 < nnMatchRatio * dist2) {
            good_matches.push_back(match);
            matched1.push_back(kpts1[match.queryIdx]);
            matched2.push_back(kpts2[match.trainIdx]);
        }
    }

    //step 3.Draw matches and display

    cv::Mat img_matches;
    cv::drawMatches( img1, kpts1, img2, kpts2, good_matches, img_matches );
    cv::namedWindow( "Matches", CV_WINDOW_FULLSCREEN);
    cv::imshow("Matches", img_matches );
    cv::waitKey(0);

    //step 4.Populate data

    std::vector<cv::Point2d> imgpts1, imgpts2;
    for( unsigned int i = 0; i < matched1.size(); i++ )
    {
        imgpts1.push_back(matched1[i].pt); // queryIdx is the "left" image
        imgpts2.push_back(matched2[i].pt); // trainIdx is the "right" image

    }
    //step 5. Estimation of Fumdamental matrix

    FundMatFitting * fmestim = new FundMatFitting();
    fmestim->setData(imgpts1,imgpts2);
    robest::LMedS * ransacSolver = new robest::LMedS();
    ransacSolver->solve(fmestim);

    cv::Mat FM = (cv::Mat_<double>(3,3));
    FM = fmestim->getResult();
    std::cout<<FM<<std::endl;

    //step 6. Epipolar lines with images (no rectified)

    cv::Mat outImage=plotStereoWithEpilines(img1,img2,FM,imgpts1,imgpts2);
    cv::namedWindow( "Epipolar lines", CV_WINDOW_FULLSCREEN );
    cv::imshow("Epipolar lines", outImage);
    cv::waitKey(0);

    //step 7. Rectification and plots

    RectifierAffine *rec=new RectifierAffine();
    rec->init(&img1 ,&img2, &FM, &imgpts1, &imgpts2);
    rec->rectify();

    cv::Mat Frect = (cv::Mat_<double>(3,3) << 0,0,0,0,0,-1.0,0,1.0,0);
    std::cout << Frect << std::endl;

    cv:: Mat input1, input2;
    rec->getResult(input1, input2,&imgpts1,&imgpts2);

    cv::Mat outImage1 = plotStereoWithEpilines(input1,input2,Frect,imgpts1,imgpts2);
    cv::namedWindow( "Epipolar lines: rectified", CV_WINDOW_FULLSCREEN );
    cv::imshow("Epipolar lines: rectified", outImage1);
    cv::waitKey(0);

    return 0;
}
