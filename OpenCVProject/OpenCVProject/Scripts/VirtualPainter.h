vector<vector<int>> drawPoints;

static Point getContoursOfDetectedColors(Mat imgCnt, Mat img) {

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
    Point myPoint(0, 0);

	findContours(imgCnt, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);
    vector<vector<Point>> contoursPoly(contours.size());
    vector<Rect> bbox(contours.size());
	for (int i = 0; i < contours.size(); i++) {
		int area = contourArea(contours[i]);


		if (area > 10) {
			float peri = arcLength(contours[i], true);
			approxPolyDP(contours[i], contoursPoly[i], 0.02 * peri, true);
			drawContours(img, contoursPoly, i, Scalar(255, 0, 255), 2);
			bbox[i] = boundingRect(contoursPoly[i]);

            myPoint.x = bbox[i].x + (bbox[i].width / 2);
            myPoint.y = bbox[i].y;
			rectangle(img, bbox[i].tl(), bbox[i].br(), Scalar(0, 255, 0), 3);
		}
	}
    return myPoint;
}


void drawOnCanvas(Mat img, vector<Scalar> displayColors) {
    cout << drawPoints.size() << endl;
    for (int i = 0; i < drawPoints.size(); i++) {
        circle(img, Point(drawPoints[i][0], drawPoints[i][1]), 10, displayColors[drawPoints[i][2]], FILLED);
    }
}

static void findColor(Mat img, vector<vector<int>> myColors) {
    Mat imgHSV;
    cvtColor(img, imgHSV, COLOR_BGR2HSV);
    for (int i = 0; i < myColors.size(); i++) {
        Mat mask;
        Scalar lower(myColors[i][0], myColors[i][1], myColors[i][2]);
        Scalar upper(myColors[i][3], myColors[i][4], myColors[i][5]);
        inRange(imgHSV, lower, upper, mask);

        Point myPoint = getContoursOfDetectedColors(mask, img);

        if (myPoint.x != 0 && myPoint.y != 0) {
            drawPoints.push_back({ myPoint.x, myPoint.y, i });
         }
         
        //imshow(to_string(i), mask);
    }
}


static void virtualPainter() {
    Mat img;
    vector<vector<int>> myColors{ {32,31,205,127,213,255}, // purple
                                  {68,72,156, 102, 126, 255} }; // green

    vector<Scalar> displayColors{ {0,0,255}, {0,255,0} };
    VideoCapture cap(0);
  

    while (true) {

        cap.read(img);
        findColor(img, myColors);

        drawOnCanvas(img, displayColors);

        imshow("Webcam", img);
        char c = (char)waitKey(30);
        if (c == 27) {
            cap.release();
            break;
        }
    }
}
