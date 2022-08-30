

static void numPlateDetection() {

	Mat img;
	VideoCapture cap(0);
	CascadeClassifier plateCascade;
	vector<Rect> plates;
	plateCascade.load("Resources/haarcascade_russian_plate_number.xml");

	if (plateCascade.empty()) {
		cout << "XML file is not loaded" << endl;
	}

	while (true) {

		cap.read(img);
		plateCascade.detectMultiScale(img, plates, 1.1, 10);
		for (int i = 0; i < plates.size(); i++) {

			Mat imgCrop = img(plates[i]);
			//imshow(to_string(i), imgCrop);

			imwrite("Resources/Plates/" + to_string(i) + ".png", imgCrop);

			rectangle(img, plates[i].tl(), plates[i].br(), Scalar(255, 0, 255), 2);
		}

		imshow("Video", img);

		char c = (char)waitKey(25);
		if (c == 27) {
			cap.release();
			break;
		}

	}
}

