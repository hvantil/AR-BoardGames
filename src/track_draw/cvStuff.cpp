#include <ctype.h>

#include "opencv2/aruco.hpp"
#include "opencv2/calib3d.hpp"
#include <opencv2/core.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/highgui.hpp>
#include "cvStuff.h"
#include <stdio.h>
#include <iostream>
#include <glutil.h>
#include <stdlib.h>
#include <fslazywindow.h>

using namespace cv;
using namespace std;
namespace {
const char* about = "Basic marker detection";
const char* keys  =
        "{d        |       | dictionary: DICT_4X4_50=0, DICT_4X4_100=1, DICT_4X4_250=2,"
        "DICT_4X4_1000=3, DICT_5X5_50=4, DICT_5X5_100=5, DICT_5X5_250=6, DICT_5X5_1000=7, "
        "DICT_6X6_50=8, DICT_6X6_100=9, DICT_6X6_250=10, DICT_6X6_1000=11, DICT_7X7_50=12,"
        "DICT_7X7_100=13, DICT_7X7_250=14, DICT_7X7_1000=15, DICT_ARUCO_ORIGINAL = 16}"
        "{v        |       | Input from video file, if ommited, input comes from camera }"
        "{ci       | 0     | Camera id if input doesnt come from video (-v) }"
        "{c        |       | Camera intrinsic parameters. Needed for camera pose }"
        "{l        | 0.1   | Marker side lenght (in meters). Needed for correct scale in camera pose }"
        "{dp       |       | File of marker detector parameters }"
        "{r        |       | show rejected candidates too }";
}

static bool readCameraParameters(string filename, Mat &camMatrix, Mat &distCoeffs) {
    FileStorage fs(filename, FileStorage::READ);
    if(!fs.isOpened())
        return false;
    fs["camera_matrix"] >> camMatrix;
    fs["distortion_coefficients"] >> distCoeffs;
    return true;
}



/**
 */
static bool readDetectorParameters(string filename, Ptr<aruco::DetectorParameters> &params) {
    FileStorage fs(filename, FileStorage::READ);
    if(!fs.isOpened())
        return false;
    fs["adaptiveThreshWinSizeMin"] >> params->adaptiveThreshWinSizeMin;
    fs["adaptiveThreshWinSizeMax"] >> params->adaptiveThreshWinSizeMax;
    fs["adaptiveThreshWinSizeStep"] >> params->adaptiveThreshWinSizeStep;
    fs["adaptiveThreshConstant"] >> params->adaptiveThreshConstant;
    fs["minMarkerPerimeterRate"] >> params->minMarkerPerimeterRate;
    fs["maxMarkerPerimeterRate"] >> params->maxMarkerPerimeterRate;
    fs["polygonalApproxAccuracyRate"] >> params->polygonalApproxAccuracyRate;
    fs["minCornerDistanceRate"] >> params->minCornerDistanceRate;
    fs["minDistanceToBorder"] >> params->minDistanceToBorder;
    fs["minMarkerDistanceRate"] >> params->minMarkerDistanceRate;
    fs["cornerRefinementMethod"] >> params->cornerRefinementMethod;
    fs["cornerRefinementWinSize"] >> params->cornerRefinementWinSize;
    fs["cornerRefinementMaxIterations"] >> params->cornerRefinementMaxIterations;
    fs["cornerRefinementMinAccuracy"] >> params->cornerRefinementMinAccuracy;
    fs["markerBorderBits"] >> params->markerBorderBits;
    fs["perspectiveRemovePixelPerCell"] >> params->perspectiveRemovePixelPerCell;
    fs["perspectiveRemoveIgnoredMarginPerCell"] >> params->perspectiveRemoveIgnoredMarginPerCell;
    fs["maxErroneousBitsInBorderRate"] >> params->maxErroneousBitsInBorderRate;
    fs["minOtsuStdDev"] >> params->minOtsuStdDev;
    fs["errorCorrectionRate"] >> params->errorCorrectionRate;
    return true;
}


void openCVcam::Setup(){
    if(oimage){
        fwid = 1280;
        fhei = 720;
    }
    else{
        inputVideo.grab();
        fwid = inputVideo.get(3);
        fhei = inputVideo.get(4);
    }
    printf("%d %d\n",fwid,fhei);
    camMatrix1 = (Mat_<double>(3,3) << 1250,0,fwid/2,0,1250,fhei/2,0,0,1);
}

void openCVcam::camerainit(){
    double zNear = .1;
    double zFar = 2000;
    double frameW = fwid;
    double frameH = fhei;
    oglCam.setTo(0);
    oglCam.at<double>(0,0) = 1250.0/(fwid/2);
    oglCam.at<double>(1,1) = 1250.0/(fhei/2);
    oglCam.at<double>(2,2) = 1*(zNear + zFar)/(zNear - zFar);
    oglCam.at<double>(2,3) = 2*zNear*zFar/(zNear - zFar);
    oglCam.at<double>(3,2) = -1;
    oglCam = oglCam.t();
}

bool openCVcam::interval(GLuint& imageTexture){
    if(oimage){
        if(osimage.empty()){
            printf("wtf\n ");
        }
    }
    else{
        if(inputVideo.grab()){
           inputVideo.retrieve(osimage);
        }
    }
        double tick = (double)getTickCount();
        vector< int > ids;
        vector< vector< Point2f > > corners, rejected;
        vector< Vec3d > rvecs, tvecs;
        aruco::detectMarkers(osimage, dictionary, corners, ids, detectorParams, rejected);
        if(estimatePose && ids.size() > 0){
            aruco::estimatePoseSingleMarkers(corners, markerLength, camMatrix1, distCoeffs, rvecs,
                                             tvecs);
            Mat newrvecs(rvecs[0]);
            Mat newtvec(tvecs[0]);
            Mat R(4, 4, CV_64F);
            Rodrigues(newrvecs,R);

            viewMatrix = cv::Mat::eye(4, 4, CV_64F);            
            for(unsigned int row=0; row<3; ++row)
            {
                for(unsigned int col=0; col<3; ++col)
                {
                    viewMatrix.at<double>(row, col) = R.at<double>(row, col);
                }
                viewMatrix.at<double>(row, 3) = newtvec.at<double>(row, 0);
            }
            viewMatrix.at<double>(3, 3) = 1.0f;
            Mat cvToGl= cv::Mat::zeros(4, 4, CV_64F);
            cvToGl.at<double>(0, 0) = 1.0;
            cvToGl.at<double>(1, 1) = -1.0; // Invert the y axis
            cvToGl.at<double>(2, 2) = -1.0; // invert the z axis
            cvToGl.at<double>(3, 3) = 1.0;
            viewMatrix = cvToGl * viewMatrix;
            viewMatrix = viewMatrix.t();
            newrvecs.release();
            newtvec.release();
            R.release();
            cvToGl.release();
        }
        double currentTime = ((double)getTickCount() - tick) / getTickFrequency();
        totalTime += currentTime;
        totalIterations++;
        if(totalIterations % 1 == 0) {
            //cout << "Detection Time = " << currentTime * 1000 << " ms "
             //    << "(Mean = " << 1000 * totalTime / double(totalIterations) << " ms)" << endl;
        }
       
        if(!intext){
         glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
         glGenTextures(1, &imageTexture);
         glBindTexture(GL_TEXTURE_2D, imageTexture);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
         glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
         cv::cvtColor(osimage, osimage, COLOR_RGB2BGR);
         glTexImage2D(GL_TEXTURE_2D,         // Type of texture
           0,                   // Pyramid level (for mip-mapping) - 0 is the top level
           GL_RGB,              // Internal colour format to convert to
           osimage.cols,          // Image width  i.e. 640 for Kinect in standard mode
           osimage.rows,          // Image height i.e. 480 for Kinect in standard mode
           0,                   // Border width in pixels (can either be 1 or 0)
           GL_RGB,              // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
           GL_UNSIGNED_BYTE,    // Image data type
            osimage.ptr());        // The actual image data itself
         intext = true;
        }
        else{
           glBindTexture(GL_TEXTURE_2D, imageTexture);
           if(!oimage){
           cv::cvtColor(osimage, osimage, COLOR_RGB2BGR);}
           glTexSubImage2D(GL_TEXTURE_2D,         // Type of texture
           0,                   // Pyramid level (for mip-mapping) - 0 is the top level
           0,
           0,              // Internal colour format to convert to
           osimage.cols,          // Image width  i.e. 640 for Kinect in standard mode
           osimage.rows,          // Image height i.e. 480 for Kinect in standard mode
           GL_RGB,              // Input image format (i.e. GL_RGB, GL_RGBA, GL_BGR etc.)
           GL_UNSIGNED_BYTE,    // Image data type
            osimage.ptr());        // The actual image data itself
        }
        if(!oimage){
            //osimage.release();
        }   
        return true;
}

bool openCVcam::readInCameraParam(int argc,char *argv[], bool &onlyimage){
    string imageName;
    CommandLineParser parser(argc, argv, keys);
    parser.about(about);

     if(argc < 2) {
       parser.printMessage();
    }
     dictionaryId = 4;
     showRejected = parser.has("r");
     estimatePose = true;
     markerLength =15;

    detectorParams = aruco::DetectorParameters::create();
    if(parser.has("dp")) {
        bool readOk = readDetectorParameters(parser.get<string>("dp"), detectorParams);
        if(!readOk) {
            cerr << "Invalid detector parameters file" << endl;
        }
    }
    detectorParams->cornerRefinementMethod = aruco::CORNER_REFINE_SUBPIX; // do corner refinement in markers
    int camId = 0;
    if(parser.has("ci")){
        camId = parser.get<int>("ci");
    }   
    
    

    int webcam = 0;
    if(parser.has("d")) {
        webcam = parser.get<int>("d");
   //     printf("woop33333333333\n");
        //SetMustTerminate(true);
    }
    String video;
    if(parser.has("c")) {
        imageName = parser.get<String>("c");
        oimage = true;
        onlyimage =true;
   //     printf("woop33333333333\n");
        //SetMustTerminate(true);
    }
    else{
        printf("yeh");
        video = "works.mp4";
        oimage = false;
        onlyimage = false;
    }
    
    if(!parser.check()) {
        parser.printErrors();
        return false;

    }
     dictionary =
        aruco::getPredefinedDictionary(aruco::PREDEFINED_DICTIONARY_NAME(dictionaryId));

    
    if(estimatePose) {
        bool readOk = readCameraParameters("toutput.txt", camMatrix, distCoeffs);
        if(!readOk) {
            cerr << "Invalid camera file" << endl;
            return false;
        }
    }

    
    int waitTime;
    if(oimage){
        osimage = imread(imageName); 
        printf("read it in\n");
    }
    else if(!video.empty() && webcam == 0){// && atoi(argv[2])){
   //     printf("woop");
        inputVideo.open(video);
        waitTime = 10;

    } else {
        inputVideo.open(camId);
        waitTime = 10;
    }
    return true;
}
