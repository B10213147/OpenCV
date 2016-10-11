#include <iostream>
#include <cv.hpp>

using namespace std;
using namespace cv;

void MyLine(Mat img);
void find_object_contours(Mat img);

int theObject[4];
Rect objectBondingRectangle;
Mat noObjectImage;
RNG rng(12345);

int main()
{
    Mat frame;

    VideoCapture cap(0);

    if(!cap.isOpened()){
        cout<<"Error:Opening the camera"<<endl;
        return -1;
    }
    cap.read(frame);

    //create a new window
    namedWindow("cam1", CV_WINDOW_AUTOSIZE);

    noObjectImage = frame.clone();
    noObjectImage.setTo(Scalar(200,200,200));

    while(1){
        switch(waitKey(100)){
        case 27:    //press ESC
            cout<<"ESC is pressed by user"<<endl;
            destroyAllWindows();
            return 0;
        case 'T':
            bool bSuccess = imwrite("TestImage.jpg", frame); //write the image to file
            if(!bSuccess){
                cout<<"Cannot read the frame"<<endl;
                break;
            }
        }

        bool bSuccess = cap.read(frame);
        if(!bSuccess){
                cout<<"Cannot read the frame"<<endl;
                break;
        }
        find_object_contours(frame);

        MyLine(frame);

        imshow("cam1",frame);
    }

    return 0;
}

Scalar MyLine_Color;
Point pCenter;
void MyLine(Mat img){
    pCenter = Point(img.cols/2, img.rows/2);

    rectangle(img,
              Point(pCenter.x-50, pCenter.y-50),
              Point(pCenter.x+50, pCenter.y+50),
              MyLine_Color);
    line(img,
         Point(pCenter.x-25, pCenter.y),
         Point(pCenter.x+25, pCenter.y),
         MyLine_Color);
    line(img,
         Point(pCenter.x, pCenter.y-25),
         Point(pCenter.x, pCenter.y+25),
         MyLine_Color);
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
        // Check if rotated rect is center or not
        Point rect_Center = Point((rect_points[2].x - rect_points[0].x) / 2 + rect_points[0].x,
                             (rect_points[2].y - rect_points[0].y) / 2 + rect_points[0].y);
        if(rect_Center == pCenter) MyLine_Color = Scalar(0, 0, 255);
        else MyLine_Color = Scalar(0, 255, 0);
        line(drawing,
         Point(pCenter.x-25, pCenter.y),
         Point(pCenter.x+25, pCenter.y),
         MyLine_Color);
        line(drawing,
         Point(pCenter.x, pCenter.y-25),
         Point(pCenter.x, pCenter.y+25),
         MyLine_Color);

        line(drawing,
         Point(rect_Center.x-25, rect_Center.y),
         Point(rect_Center.x+25, rect_Center.y),
         MyLine_Color);
        line(drawing,
         Point(rect_Center.x, rect_Center.y-25),
         Point(rect_Center.x, rect_Center.y+25),
         MyLine_Color);
    }

    /// Show in a window
    namedWindow( "Contours1", CV_WINDOW_AUTOSIZE );
    imshow( "Contours1", drawing );
}

