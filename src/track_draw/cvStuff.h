#ifndef CVSTUFF_H_IS_INCLUDED
#define CVSTUFF_H_IS_INCLUDED

#include <ctype.h>
#include "opencv2/aruco.hpp"
#include "opencv2/calib3d.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include <fslazywindow.h>

using namespace cv;

class openCVcam{
public:
    bool oimage = false;
    double totalTime = 0;
    int totalIterations = 0;
    int dictionaryId;
    bool showRejected;
    bool estimatePose;
    float markerLength;
    Ptr<aruco::Dictionary> dictionary;
    Ptr<aruco::DetectorParameters> detectorParams;
    Mat camMatrix, distCoeffs;
    Mat camMatrix1;
    Mat oglCam = cv::Mat::eye(4,4,CV_64F);
    Mat viewMatrix = cv::Mat::eye(4, 4, CV_64F);
    VideoCapture inputVideo;
    bool intext = false;
    int fwid = 1280;
    int fhei = 720;
    Mat osimage;
    void Setup();
    bool readInCameraParam(int argc, char*argv[],bool &onlyimage);
    void camerainit();
    bool interval(GLuint& imageTexture);
};

#endif