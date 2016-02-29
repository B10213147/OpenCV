#include <iostream>
#include <cv.hpp>

using namespace std;
using namespace cv;

int main()
{
    Mat img=imread("IMG_2687.jpg",CV_LOAD_IMAGE_COLOR);

    if(img.empty()){
        cout<<"image cannot be read"<<endl;
        return -1;
    }

    resize(img,img,Size(750,500));

    namedWindow("Original Image",CV_WINDOW_AUTOSIZE);
    imshow("Original Image",img);

    vector<Mat> channels;
    Mat img_hist_equalized;

    //Histogram Equalization of Color image
    cvtColor(img,img_hist_equalized,CV_BGR2YCrCb);  //convert to YCrCb form

    split(img_hist_equalized,channels); //split the image into channels

    equalizeHist(channels[0],channels[0]);  //equalize histogram on 1st channel(Y)

    merge(channels,img_hist_equalized); //merge 3 channels into one image

    cvtColor(img_hist_equalized,img_hist_equalized,CV_YCrCb2BGR);   //convert back to BGR form

    //create window
    namedWindow("Histogram Equalized(color)",CV_WINDOW_AUTOSIZE);
    imshow("Histogram Equalized(color)",img_hist_equalized);

    //Histogram Equalization of Grayscale image
    cvtColor(img,img,CV_BGR2GRAY);

    equalizeHist(img,img_hist_equalized);

    //create windows
    namedWindow("Grayscale",CV_WINDOW_AUTOSIZE);
    namedWindow("Histogram Equalized(gray)",CV_WINDOW_AUTOSIZE);

    imshow("Grayscale",img);
    imshow("Histogram Equalized(gray)",img_hist_equalized);

    waitKey(0);

    destroyAllWindows();

    return 0;
}
