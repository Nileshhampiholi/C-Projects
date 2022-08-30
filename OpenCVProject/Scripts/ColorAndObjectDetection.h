#include "BasicOpenCVFunctions.h"


static void colorDetection(string path) {
	Mat img = readImage(path);
	Mat imgHSV, mask;
	int hmin = 0, smin = 110, vmin = 153;
	int hmax = 19, smax = 240, vmax = 255;

	cvtColor(img, imgHSV, COLOR_BGR2HSV);

	namedWindow("Track Bar", (640, 200));

	createTrackbar("Hue Min", "Track Bar", &hmin, 179);
	createTrackbar("Hue Max", "Track Bar", &hmax, 179);
	createTrackbar("Sat Min", "Track Bar", &smin, 255);
	createTrackbar("Sat Max", "Track Bar", &smax, 255);
	createTrackbar("Val Min", "Track Bar", &vmin, 255);
	createTrackbar("Val Max", "Track Bar", &vmax, 255);
	while (true) {

		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);

		inRange(imgHSV, lower, upper, mask);

		imshow("Image", img);
		imshow("Image HSV", imgHSV);
		imshow("Mask", mask);
		char c = (char)waitKey(5);
		if (c == 27) {
			break;
		}
	}
}

static void getContours(Mat imgCnt, Mat img) {

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	string objectType;
	
	findContours(imgCnt, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	vector<vector<Point>> contoursPoly(contours.size());
	vector<Rect> bbox(contours.size());

	for (int i = 0; i < contours.size(); i++) {
		int area = contourArea(contours[i]);

		if (area > 1000) {
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], contoursPoly[i], 0.02 * peri, true);
			drawContours(img, contoursPoly, i, Scalar(255, 0, 255), 2);
			bbox[i] = boundingRect(contoursPoly[i]);
			rectangle(img, bbox[i].tl(), bbox[i].br(), Scalar(0, 255, 0), 3);


			int numPoints = (int)contoursPoly[i].size();

			if (numPoints == 3) { objectType = "Triangle"; }
			else if (numPoints == 4) {
				float aspectRatio = (float)bbox[i].width / (float)bbox[i].height;
				if (aspectRatio > 0.95 && aspectRatio < 1.05) {
					objectType = "Square";
				}
				else {
					objectType = "Rectangle";
				}
			}
			else if (numPoints > 4) { objectType = "Circle"; }

			putText(img, objectType, { bbox[i].x, bbox[i].y - 5 }, FONT_HERSHEY_DUPLEX, 0.3, Scalar(0, 65, 255), 1);

		}
	}

}

static void detectObjects(string path, bool show) {

	Mat img, imgGray, imgBlur, imgCanny, imgDil, imgErode;
	img = readImage(path);
	tie(img, imgGray, imgBlur, imgCanny, imgDil, imgErode) = basicSegmentationOperations(img, show);

	getContours(imgErode, img);
	imshow("Image with Contours", img);
	waitKey(0);

}

static void colorPicker() {
	VideoCapture cap(0);
	Mat img, imgHSV, mask;

	int hmin = 0, smin = 0, vmin = 0;
	int hmax = 179, smax = 255, vmax = 255;

	
	namedWindow("Track Bar", (640, 200));
	createTrackbar("Hue Min", "Track Bar", &hmin, 179);
	createTrackbar("Hue Max", "Track Bar", &hmax, 179);
	createTrackbar("Sat Min", "Track Bar", &smin, 255);
	createTrackbar("Sat Max", "Track Bar", &smax, 255);
	createTrackbar("Val Min", "Track Bar", &vmin, 255);
	createTrackbar("Val Max", "Track Bar", &vmax, 255);

	while (true) {

		cap.read(img);
		cvtColor(img, imgHSV, COLOR_BGR2HSV);
		Scalar lower(hmin, smin, vmin);
		Scalar upper(hmax, smax, vmax);
		inRange(imgHSV, lower, upper, mask);

		cout << hmin << "," << hmax << "," << smin << "," << smax << "," << vmin << "," << vmax << endl;

		imshow("Image", img);
		imshow("Image HSV", imgHSV);
		imshow("Mask", mask);

		
		cout << hmin << "," << hmax << "," << smin << "," << smax << "," << vmin << "," << vmax << endl;
		char c = (char)waitKey(5);
		if (c == 27) {
			break;
		}
	}
}
