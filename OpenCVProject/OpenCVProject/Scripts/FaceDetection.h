#include "ColorAndObjectDetection.h"
#include <opencv2/objdetect.hpp>


static void faceDetection(string path) {

	Mat img = readImage(path);

	CascadeClassifier faceCascade;

	faceCascade.load("Resources/haarcascade_frontalface_default.xml");

	if (faceCascade.empty()) {
		cout << "XML file is not loaded" << endl;
	}

	vector<Rect> faces;
	faceCascade.detectMultiScale(img, faces, 1.1, 10);

	for (int i = 0; i < faces.size(); i++) {
		rectangle(img, faces[i].tl(), faces[i].br(), Scalar(255, 0, 255), 2);
	}

	imshow("Image with detections", img);
	waitKey(0);
}

