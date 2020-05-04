


#include "gearinspector.h"
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <iostream>

using namespace cv;

using namespace std;

struct Gear gear;

GearInspector::GearInspector(cv::Mat& src)
{
    Mat src1;
    gear.mSrc=src.clone();
    resize(gear.mSrc,src1,Size(500,500));
    imshow("src",src1);
    waitKey(0);
    
}

void GearInspector::calcParameters()
{
    cout<<"inspection starts";
    Mat grayIm,blurIm,threshIm,testIm,morphIm,element;
    Mat src=gear.mSrc.clone();
    Mat add=gear.mSrc.clone();
    Mat dee=gear.mSrc.clone();
    Mat contourIm=Mat::zeros(gear.mSrc.size(),CV_8UC3);
    element =getStructuringElement(2,Size(7,7),Point(1,1));
    cvtColor(gear.mSrc,grayIm,COLOR_BGR2GRAY);


    blur(grayIm,blurIm,Size(3,3));
    //medianBlur(grayIm,blurIm,11);

    //GaussianBlur(grayIm,blurIm,Size(11,11),5,0);
    threshold(blurIm,threshIm,0,255,THRESH_BINARY|THRESH_OTSU);


    resize(threshIm,testIm,Size(500,500));
    imshow("f",testIm);
    imwrite("t.jpg",testIm);
    waitKey(0);


    findContours(threshIm,gear.mContours,RETR_TREE,CHAIN_APPROX_SIMPLE);
    Mat drawing = Mat::zeros( threshIm.size(), CV_8UC3 );
    Mat maxcontours = Mat::zeros( threshIm.size(), CV_8UC3 );
    Scalar color = Scalar(0,0,255);
    for( size_t i = 0; i< gear.mContours.size(); i++ )
    {

        //cout<<i;
        drawContours( drawing, gear.mContours, (int)i, color, 2, LINE_8,noArray(),0);
    }
    resize(drawing,drawing,Size(500,500));
    imshow("contours",drawing);
    waitKey(0);



    cout<<"addendum starts";
    // struct Gear gear;
    gear.mNumTeeth=0;
    cout<<gear.mContours.size()<<endl;
    double max=0;
    size_t index=0;

    for (size_t i=0;i<gear.mContours.size();i++)
    {


        if(contourArea(gear.mContours[i]) > max)
        {

            max=contourArea(gear.mContours[i]);

            index = i;
            cout<<index<<endl;

        }
    }
    Mat draw;
    drawContours(src, gear.mContours, index, color, 2, LINE_8,noArray(),0);
    resize(src,draw,Size(500,500));
    imshow("maxcontours",draw);
    waitKey(0);
    convexHull( gear.mContours[index], gear.mhull[index] ,false);
    convexityDefects(gear.mContours[index],gear.mhull[index],gear.mDefects[index]);
    cout<<"addendum starts";
    for(size_t k=0; k < gear.mhull[index].size() ; k++) {
        size_t ind=gear.mhull[index][k];
        cout<<ind<<endl;
        gear.mHullPoints[index].push_back(gear.mContours[index][ind]);
    }
    int count=0;

    for (size_t l=0;l<gear.mDefects[index].size();l++) {
        if( gear.mDefects[index][l][3] > 20*256 )
        {
            size_t ind=gear.mDefects[index][l][0];
            size_t ind1=gear.mDefects[index][l][2];
            circle(src,gear.mContours[index][ind],20,Scalar(0,255,255),5);
            circle(src,gear.mContours[index][ind1],20,Scalar(0,255,0),5);
            gear.mDefectsPoints[index].push_back(gear.mContours[index][ind1]);
            gear.mDefectsAll[index].push_back(gear.mContours[index][ind]);
            gear.mDefectsAll[index].push_back(gear.mContours[index][ind1]);
            count++;


        }
    }


    drawContours(src,gear.mContours,(int)index,Scalar(0,0,255),2,LINE_8,noArray(),0);
    cout<<"no.of teeths"<<count<<endl;
    gear.mNumTeeth=count;
    gear.mIndex=index;

    resize(src,draw,Size(500,500));

    imshow("hull",draw);
    waitKey(0);





    
    calcAddendum(add);
    calcDedundum(dee);
}


void GearInspector::calcAddendum(cv::Mat img)
{
    Mat draw;
  int index=gear.mIndex;
approxPolyDP(gear.mContours[index],gear.mPolyContours[index],3,true) ;
vector<Point2f>center(gear.mContours.size());
vector<float>radius(gear.mContours.size());
minEnclosingCircle(gear.mPolyContours[index],center[index],radius[index]);
circle(img,center[index],(int)radius[index],Scalar(255,255,255),2);
circle(img,center[index],3,Scalar(0,0,255),2);
gear.mCenterAddm=center[index];
gear.mRadAddendum=radius[index];
cout<<gear.mCenterAddm<<endl;
cout<<"radius"<<gear.mRadAddendum<<endl;
resize(img,draw,Size(500,500));

imshow("srcadd",draw);
waitKey(0);

    
}
void GearInspector::calcDedundum(cv::Mat img)
{
    Mat draw;
     int index=gear.mIndex;
    vector<Point2f>center(gear.mContours.size());
    vector<float>radius(gear.mContours.size());
    minEnclosingCircle(gear.mDefectsPoints[index],center[index],radius[index]);
    circle(img,center[index],(int)radius[index],Scalar(255,255,0),5);
    circle(img,center[index],3,Scalar(0,0,255),2);
    gear.mCenterDedm=center[index];
    gear.mRadDedundum=radius[index];
    cout<<gear.mCenterDedm<<endl;
    cout<<"radius"<<gear.mRadDedundum<<endl;
    resize(img,draw,Size(500,500));

    imshow("srcded",draw);
    waitKey(0);
}
