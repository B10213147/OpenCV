#include <cv.hpp>
#include <iostream>

using namespace cv;
using namespace std;

string intToString(int number){


	std::stringstream ss;
	ss << number;
	return ss.str();
}

int main(int argc, char* argv[])
{
    bool recording = false;
    bool newRecord = false;
    bool firstRun = true;
    int videoNumber = 0;

	VideoCapture cap(0); // open the video camera no. 0

	if (!cap.isOpened())  // if not success, exit program
    {
        cout << "ERROR INITIALIZING VIDEO CAPTURE" << endl;
        return -1;
    }

	char* windowName = "Webcam Feed";
	namedWindow(windowName,CV_WINDOW_AUTOSIZE); //create a window to display our webcam feed

    VideoWriter writer;

    //fourcc int
    int fcc = CV_FOURCC('D','I','V','4');

    //fps double
    double fps = 29.94;

    //frame size
	Size framesize(cap.get(CV_CAP_PROP_FRAME_WIDTH),
                cap.get(CV_CAP_PROP_FRAME_HEIGHT));

	while (1) {
		if(newRecord == true){
            newRecord = false;
            recording = true;
            videoNumber++;

            //filename string
            string filename = "I:/myvideo"+ intToString(videoNumber) +".avi";

            writer = VideoWriter(filename, fcc, fps, framesize);

            if(!writer.isOpened()){
                cout<<"ERROR OPENING FILE FOR WRITE"<<endl;
                getchar();
                return -1;
            }
		}

		Mat frame;

		bool bSuccess = cap.read(frame); // read a new frame from camera feed

		if (!bSuccess) //test if frame successfully read
		{
			cout << "ERROR READING FRAME FROM CAMERA FEED" << endl;
			getchar();
			return -1;
		}

        if(recording == true){
            writer.write(frame);
            putText(frame, "REC", Point(0,60),2,2,Scalar(0,0,255));
        }

        imshow(windowName, frame); //show the frame in "MyVideo" window

		//listen for 10ms for a key to be pressed
		switch(waitKey(10)){

		case 27:
			//'esc' has been pressed (ASCII value for 'esc' is 27)
			//exit program.
			return 0;

        case 114:
            //'r' has been pressed
            //toggle the record
            recording = !recording;

            if(recording) cout<<"Start recording."<<endl;
            else cout<<"Pause record."<<endl;

            if(firstRun == true){
                recording = true;
                firstRun = false;

                //filename string
                string filename = "I:/myvideo0.avi";

                writer = VideoWriter(filename, fcc, fps, framesize);

                if(!writer.isOpened()){
                    cout<<"ERROR OPENING FILE FOR WRITE"<<endl;
                    getchar();
                    return -1;
                }
            }

            break;

        case 110:
            //'n' has been pressed
            //create a new record
            newRecord = true;

            if(firstRun) firstRun = false;

            cout<<"New record started."<<endl;

            break;
		}
	}

	return 0;
}
////////////////////////////////////////////////////////////////////////////////////////////
