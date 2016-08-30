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
    Mat grayImage1;
    Mat differenceImage;
    Mat thresholdImage;

    VideoCapture cap(1);

    if(!cap.isOpened()){
        cout<<"Error:Opening the camera"<<endl;
        return -1;
    }
    cap.read(noObjectImage);

    //create a new window
    namedWindow("cam", CV_WINDOW_AUTOSIZE);

    Mat frame;

    while(1){
        switch(waitKey(33)){
        case 27:    //press ESC
            cout<<"ESC is pressed by user"<<endl;
            destroyAllWindows();
            return 0;
        case 'T':
            bool bSuccess = cap.read(noObjectImage);
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

        imshow("cam",frame);


    }

    Mat white = frame.clone();
    white.setTo(Scalar(255,255,255));

    //absdiff function need two gray scalar input
    cvtColor(white,white,CV_BGR2GRAY);
    cvtColor(frame,grayImage1,CV_BGR2GRAY);

    absdiff(white,grayImage1,differenceImage);

    threshold(differenceImage,thresholdImage,50,255,THRESH_BINARY);
    blur(thresholdImage,thresholdImage,Size(10,10));
    threshold(thresholdImage,thresholdImage,20,255,THRESH_BINARY);

    //these two vectors needed for output of findContours
    vector< vector<Point> > contours;
    vector<Vec4i> hierarchy;

    //find contours of filtered image using openCV findContours function
    findContours(thresholdImage,contours,hierarchy,CV_RETR_EXTERNAL,CV_CHAIN_APPROX_SIMPLE );// retrieves external contours

    //if contours vector is not empty, we have found some objects
    for(unsigned int i=0; i<contours.size(); i++){

        objectBondingRectangle = boundingRect(contours.at(i));

        int objx = objectBondingRectangle.width;
        int objy = objectBondingRectangle.height;

        int xpos = objectBondingRectangle.x+objx/2;
        int ypos = objectBondingRectangle.y+objy/2;

        theObject[0] = xpos;
        theObject[1] = ypos;
        theObject[2] = objx;
        theObject[3] = objy;

        Mat croppedImage = Mat(frame,Rect(xpos-objx/2,ypos-objy/2,objx,objy)).clone();
        resize(croppedImage,croppedImage,Size(300,300));

        char imageName[10] = "test";
        imageName[4] = i+48;
        imshow(imageName,croppedImage);
    }

    waitKey(0);
    destroyAllWindows();

    return 0;
}

/*
switch(key){
    get_no_object()
}
draw_line()
//Have object
crop_center()
get_object_contour()
if(slope==0) line turn to red
//object leave
*/

void MyLine(Mat img){
    Point pCenter = Point(img.cols/2, img.rows/2);

    rectangle(img,
              Point(pCenter.x-50, pCenter.y-50),
              Point(pCenter.x+50, pCenter.y+50),
              Scalar(0, 255, 0));
    line(img,
         Point(pCenter.x-25, pCenter.y),
         Point(pCenter.x+25, pCenter.y),
         Scalar(0, 255, 0));
    line(img,
         Point(pCenter.x, pCenter.y-25),
         Point(pCenter.x, pCenter.y+25),
         Scalar(0, 255, 0));
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

    /// Find the rotated rectangles and ellipses for each contour
    vector<RotatedRect> minRect( contours.size() );
    vector<RotatedRect> minEllipse( contours.size() );

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
    }

    /// Show in a window
    namedWindow( "Contours", CV_WINDOW_AUTOSIZE );
    imshow( "Contours", drawing );
}
