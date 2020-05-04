/**************************************************************************************
* File			: GearInspector.h
* Project		: GearScanner
* Author		: Amal P. Anand
* Description	: File for declaring GearInspector Class, Teeth and Gear DataStructures
*				  This class finds the addendum,dedendum,tooth profile and
*                 inner circles of the gear
*
*				  Algorithm is as follows:
*				  1) Input the source image containing gear
*				  2) Preprocess the image - noise removal using Gaussian Blur
*				  3) Threshold using otsu
*				  4) Find contours.
*				  5) Find minimum enclosing circle to get the addendum
*                 6) Apply convex hull on outer contour to get tooth profile
*				  7) Fit a circle on convexity defects to get the dedendum
*                 8) Calculate the 3 inner circles of the gear
*
* Creation Date	: 05/06/2019
* Company		: Rapid Techs
* Version		: 1.0
* Dependencies	: Uses OpenCV library with a FREEBSD license
* Modifications	:
*
*
*
***************************************************************************************/

#ifndef GEARINSPECTOR_H
#define GEARINSPECTOR_H

#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

/***************************************************************************************
* Struct		: Teeth
* Base Struct	: None
* Description	: DataStructure to hold teeth properties
* Members		: Teeth() - default constructor
*				  mContour - contour of teeth
*				  mWidth - width of teeth
*				  mHeight - height of teeth
*				  mArea - area of teeth
*                 mRect - bounding rect of teeth
*				  mImage - extracted teeth image
*
***************************************************************************************/
struct Teeth
{
    // default constructor
    Teeth():mWidth(0),mHeight(0),mArea(0){}
    // contour of teeth
    std::vector<cv::Point> mContour;
    // width of teeth
    int mWidth;
    // height of teeth
    int mHeight;
    // area of teeth
    int mArea;
    // bounding rect of teeth
    cv::Rect mRect;
    // extracted teeth image
    cv::Mat mImage;
};


/***************************************************************************************
* Struct		: Gear
* Base Struct	: None
* Description	: DataStructure to hold gear properties
* Members		: Gear() - default constructor
*				  mAddendum - area of the pipe
*				  mDedundum - perimeter of the pipe
*				  mContToothProfile - boundary of the pipe
*				  mUnWoundTP - bounding box of the pipe
*                 mCentre - centre of gear
*                 mInnerCircle - innermost circle of gear
*                 mMidCircle - middle circle of gear
*                 mOuterCircle - outer circle of gear
*                 mNumTeeth - number of teeth
*                 mToothProfile - tooth profile with Individual teeths in a list
*                 mRect - bounding box of the gear
*				  mImage - extracted gear image
*
***************************************************************************************/
struct Gear
{
    // default constructor
    Gear():mNumTeeth(0){}
    //contour size
    size_t mContourSize;
    // addendum of gear
    std::vector<cv::Point> mAddendum;
    // radius of addendum
    float mRadAddendum;
    // center of addendum
    cv::Point2f mCenterAddm;
    //dedundum of gear
    std::vector<cv::Point> mDedundum;
    // radius of dedundum
    float mRadDedundum;
    // center of dedundum
    cv::Point2f mCenterDedm;

    int mNumTeeth;
    int mIndex;
    std::vector<std::vector<cv::Point>> mContours;
    //hull
    std::vector<std::vector<int>> mhull{1000};
    //hull points
    std::vector<std::vector<cv::Point>> mHullPoints{1000};
    // all defects
    std::vector<std::vector<cv::Point>> mDefectsAll{1000};
    std::vector<std::vector<cv::Point>> mDefectsPoints{1000};

    // filtered defects
    std::vector<std::vector<cv::Vec4i>> mDefects{1000};
    // contour of tooth profile of gear
    std::vector<std::vector<cv::Point>> mCtrToothProfile;
    std::vector<std::vector<cv::Point>>mPolyContours{1000};
    // unwounded tooth profile of gear
    std::vector<float> mUnWoundTP;
    // centre of gear
    cv::Point2f mCenter;
    // 3 circles in the inner portion of the gear
    // innermost circle of gear
    std::vector<cv::Point> mInnerCircle;
    // middle circle of gear
    std::vector<cv::Point> mMidCircle;
    // outer circle of gear
    std::vector<cv::Point> mOuterCircle;
    // number of teeth
   // int mNumTeeth;
    // tooth profile with Individual teeths in a list
    std::vector<Teeth> mToothProfile;
    // bounding box of the gear
    cv::Rect mRect;
    // extracted gear image
    cv::Mat mImage;
    // source image
    cv::Mat mSrc;

};
/***************************************************************************************
* Class	    	: Gear
* Description	: DataStructure to hold gear properties
* Members		: GearInspector() - default constructor
*				  GearInspector(cv::Mat& src) - default constructor
*				  calcParmeters() - perimeter of the pipe
*				  mContToothProfile - boundary of the pipe
*				  mUnWoundTP - bounding box of the pipe
*                 mCentre - centre of gear
*                 mInnerCircle - innermost circle of gear
*                 mMidCircle - middle circle of gear
*                 mOuterCircle - outer circle of gear
*                 mNumTeeth - number of teeth
*                 mToothProfile - tooth profile with Individual teeths in a list
*                 mRect - bounding box of the gear
*				  mImage - extracted gear image
*
***************************************************************************************/


class GearInspector
{
public:
    GearInspector();
    // Constructor that takes source image
    GearInspector(cv::Mat& src);
    // calculate the top gear addendum, dedundum and tooth profile
    void calcParameters(void);


private:
    // any private methods go here
    // find addendum
    void calcAddendum(cv::Mat img);
    // find dedundum
    void calcDedundum(cv::Mat img);
    // un wound tooth profile
    void unWoundTP(Gear &gear);



private:
   // source gear image
    cv::Mat mSrc;
    // cropped gear image
    cv::Mat mCrop;
    // output gear image
    cv::Mat mDst;
    // extracted gears
    Gear mTopOuterGear;
    Gear mTopInnerGear;
    Gear mBotOuterGear;
    Gear mBotInnerGear;

};

#endif // GEARINSPECTOR_H
