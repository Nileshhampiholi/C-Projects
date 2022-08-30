#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>
#include<tuple>

using namespace cv;
using namespace std;


/////////////////  Images  //////////////////////

static Mat readImage(string path) {

	ifstream file(path);
	if (file){
		Mat img = imread(path);
		return img;
	}
	else {
		cout << "File does not exist" << endl;
		exit(0);
	}

}

/////////////////  Video  //////////////////////

static void playVideo(string path) {

	ifstream file(path);
	if (file) {
		VideoCapture cap(path);
		Mat img;

		while (true) {
			cap.read(img);
			if (img.empty()) {
				cap.release();
				break;
			}
			imshow("video", img);
			waitKey(20);
		}
	}
	else {
		cout << "File does not exist" << endl;
		exit(0);
	}
}


/////////////////  webcam  //////////////////////

static void streamWebcam() {

    VideoCapture cap(0);
    Mat img;

    while (true) {

        cap.read(img);
        imshow("Webcam", img);
        char c = (char)waitKey(25);
        if (c == 27) {
            cap.release();
            break;
        }
    }
}


/////////////////  Segmentation operations  //////////////////////

static tuple <Mat, Mat, Mat, Mat, Mat, Mat> basicSegmentationOperations(Mat img, bool show) {
	Mat imgGray, imgBlur, imgCanny, imgDil, imgErode;


	// Gray image 
	cvtColor(img, imgGray, COLOR_BGR2GRAY);

	// Blur image
	GaussianBlur(img, imgBlur, Size(7, 7), 5, 0);


	// Edge detection 
	Canny(img, imgCanny, 50, 150);

	// Dilation 
	Mat kernel_dilate = getStructuringElement(MORPH_RECT, Size(7, 7));
	dilate(imgCanny, imgDil, kernel_dilate);


	// Erosion
	Mat kernel_erode = getStructuringElement(MORPH_RECT, Size(5, 5));
	erode(imgDil, imgErode, kernel_erode);

	if (show == true) {
		imshow("Image", img);
		imshow("Gray Image", imgGray);
		imshow("Blur Image", imgBlur);
		imshow("Edge Detection", imgCanny);
		imshow("Dilated Image", imgDil);
		imshow("Eroded Image", imgErode);
		waitKey(0);
	}
	
	return make_tuple(img, imgGray, imgBlur, imgCanny, imgDil, imgErode);

}


/////////////////  Sizing operations  //////////////////////

static void basicImageOperations(String path) {

	Mat img, imgResize, imgScale, imgCrop;

	// Read image 
	img = readImage(path);
	cout << img.size() << endl;

	// Resize image 
	resize(img, imgResize, Size(640, 480));

	// Scale image 
	resize(img, imgScale, Size(), 0.5, 0.5);

	// Crop image
	Rect roi(200, 100, 300, 300);
	imgCrop = img(roi);

	imshow("Image", img);
	imshow("Resized Image", imgResize);
	imshow("Scaled Image", imgScale);
	imshow("Cropped Image", imgCrop);
	waitKey(0);

}

/////////////////  Drawing  //////////////////////

static void drawShapes() {
	Mat img(512, 512, CV_8UC3, Scalar(255, 255, 255));

	circle(img, Point(256, 256), 155, Scalar(0, 69, 255), FILLED);

	rectangle(img, Point(130, 226), Point(382, 286), Scalar(255, 255, 255), FILLED);

	line(img, Point(130, 296), Point(382, 296), Scalar(255, 255, 255), 2);

	putText(img, "Open CV Tutorial", Point(137, 262), FONT_HERSHEY_DUPLEX, 0.75, Scalar(0, 65, 255), 2);

	imshow("Image with shapes", img);
	waitKey(0);

}



/////////////////  Image wrapping  //////////////////////

static void wrapImages() {

	string cardImagePath = "Resources/cards.jpg";
	float w = 250, h = 350;
	Mat matrix, imgWrap;

	Mat cardImage = imread(cardImagePath);

	Point2f src[4] = { {529,142}, {771,190}, {405,395}, {674,457} };
	Point2f dst[4] = { {0.0f,0.0f}, {w,0.0f}, {0.0f,h}, {w,h} };

	matrix = getPerspectiveTransform(src, dst);
	warpPerspective(cardImage, imgWrap, matrix, Point(w, h));

	for (int i = 0; i < 4; i++) {
		circle(cardImage, src[i], 10, Scalar(0, 0, 255), FILLED);
	}
	imshow("Card Image", cardImage);
	imshow("Wrapped Image", imgWrap);
	waitKey(0);
}

