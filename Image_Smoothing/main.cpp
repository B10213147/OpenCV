#include <iostream>
#include <cv.hpp>

using namespace std;
using namespace cv;

int main(){
    VideoCapture cap(0);

    if(!cap.isOpened()){
        cout<<"Error:Opening the camera"<<endl;
        return -1;
    }

    //create a new window
    namedWindow("cam", CV_WINDOW_AUTOSIZE);

    Mat frame;

    while(1){
        bool bSuccess = cap.read(frame);
        if(!bSuccess){
            cout<<"Cannot read the frame"<<endl;
            break;
        }

        imshow("cam",frame);

        switch(waitKey(10)){
        case 27:    //press ESC
            cout<<"ESC is pressed by user"<<endl;
            return 0;

        case 13:    //press Enter
            cout<<"Take a picture"<<endl;
            cap>>frame;
            imwrite("I:/original.jpg",frame);

            //create a new window
            namedWindow("Original",CV_WINDOW_AUTOSIZE);
            imshow("Original",frame);

            Mat dst;
            char zBuffer[35];
            int k = 17;  //kernel size

            //copy the string into zBuffer
            snprintf(zBuffer,35,"Kernel:%d x %d",k,k);

            //smoothing function
            bilateralFilter(frame,dst,k,k,k);


            //show the string on image
            putText(dst,zBuffer,Point(frame.cols/4,frame.rows/8),CV_FONT_HERSHEY_COMPLEX,1,Scalar(255,255,255));

            //create a new window
            namedWindow("Smoothing",CV_WINDOW_AUTOSIZE);
            imshow("Smoothing",dst);

            imwrite("I:/smoothing.jpg",dst);

            break;
        }
    }

    return 0;
}
