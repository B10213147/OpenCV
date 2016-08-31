#include <iostream>
#include <cv.hpp>

using namespace std;
using namespace cv;

void find_object_contours(Mat img);

int theObject[4];
Rect objectBondingRectangle;
Mat noObjectImage;
RNG rng(12345);

int main()
{
    Mat grayImage1;
    Mat differenceImage;
    Mat thresholdImage;

    Mat img=imread("1.png");

    if(img.empty()){
        cout<<"Cannot read the image!"<<endl;
        return -1;
    }

    imshow("1.png",img);

    noObjectImage = img.clone();
    noObjectImage.setTo(Scalar(255, 255, 255));

    find_object_contours(img);


    waitKey(0);
    destroyAllWindows();

    return 0;
}

void find_object_contours(Mat img){
    Mat grayImage1, grayImage2;
    Mat differenceImage;
    Mat thresholdImage;
    //absdiff function need two gray scalar input
    cvtColor(noObjectImage, grayImage1, CV_RGB2GRAY);

    cvtColor(img, grayImage2, CV_RGB2GRAY);

    absdiff(grayImage1, grayImage2, differenceImage);

    threshold(differenceImage,thresholdImage,50,255,THRESH_BINARY);
    blur(thresholdImage,thresholdImage,Size(10,10));
    threshold(thresholdImage,thresholdImage,20,255,THRESH_BINARY);

    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;

    //find contours of filtered image using openCV findContours function
    findContours(thresholdImage,
                 contours,
                 hierarchy,
                 CV_RETR_TREE,
                 CV_CHAIN_APPROX_SIMPLE);// retrieves external contours

    /// Find the rotated rectangles for each contour
    vector<RotatedRect> minRect( contours.size() );

    for( int i = 0; i < contours.size(); i++ )
        minRect[i] = minAreaRect( Mat(contours[i]) );

    /// Draw contours + rotated rects + ellipses
    Mat drawing = Mat::zeros( img.size(), CV_8UC3 );
    for( int i = 0; i< contours.size(); i++ )
    {
       Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
       // contour
       drawContours( drawing, contours, i, color, 1, 8, vector<Vec4i>(), 0, Point() );
       // rotated rectangle
       Point2f rect_points[4]; minRect[i].points( rect_points );
       for( int j = 0; j < 4; j++ )
          line( drawing, rect_points[j], rect_points[(j+1)%4], color, 1, 8 );
//        // Check if rotated rect is horizontal or not
//        float rect_slope = (rect_points[2].y - rect_points[1].y) /
//                            (rect_points[2].x - rect_points[1].x);
//        if(rect_slope * 100 == 0) MyLine_Color = Scalar(0, 0, 255);
//        else MyLine_Color = Scalar(0, 255, 0);
    }

    /// Show in a window
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );
}
