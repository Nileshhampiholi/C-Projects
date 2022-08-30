static vector<Point> getDocContours(Mat imgCnt, Mat img) {

	vector<vector<Point>> contours;
	vector<Vec4i> hierarchy;
	vector<Point> biggest;
	int maxArea = 0;

	findContours(imgCnt, contours, hierarchy, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

	if (!contours.empty()) {
		vector<vector<Point>> contoursPoly(contours.size());
		vector<Rect> bbox(contours.size());
		for (int i = 0; i < contours.size(); i++) {
			int area = contourArea(contours[i]);
			if (area > 10) {
				float peri = arcLength(contours[i], true);
				approxPolyDP(contours[i], contoursPoly[i], 0.02 * peri, true);

				if (area > maxArea && contoursPoly[i].size() == 4) {
					//drawContours(img, contoursPoly, i, Scalar(255, 0, 255), 5);
					maxArea = area;
					biggest = { contoursPoly[i][0], contoursPoly[i][1],contoursPoly[i][2], contoursPoly[i][3] };
				}

				bbox[i] = boundingRect(contoursPoly[i]);
				//rectangle(img, bbox[i].tl(), bbox[i].br(), Scalar(0, 255, 0), 3);
			}
		}

	}
	return biggest;

}

static void drawPointsOnImage(Mat img, vector<Point> points, Scalar color) {

	for (int i = 0; i < points.size(); i++) {
		circle(img, points[i], 10, color, FILLED);
		putText(img, to_string(i), points[i], FONT_HERSHEY_PLAIN, 10, color, 5);
	}
}


static vector<Point> reorderPoints(vector<Point> points) {
	vector<Point> newPoints;
	vector<int> sumPoints, subPoints;

	for (int i = 0; i < 4; i++) {
		sumPoints.push_back(points[i].x + points[i].y);
		subPoints.push_back(points[i].x - points[i].y);
	}
#
	newPoints.push_back(points[min_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);  // 0
	newPoints.push_back(points[max_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);  // 1
	newPoints.push_back(points[min_element(subPoints.begin(), subPoints.end()) - subPoints.begin()]);  // 2
	newPoints.push_back(points[max_element(sumPoints.begin(), sumPoints.end()) - sumPoints.begin()]);  // 3

	return newPoints;
}


static Mat getWrap(Mat img, vector<Point> points, float w, float h) {

	Mat imgWrap;
	Point2f src[4] = {points[0], points[1], points[2], points[3]};
	Point2f dst[4] = {{0.0f,0.0f}, {w,0.0f}, {0.0f,h}, {w,h}};

	Mat matrix = getPerspectiveTransform(src, dst);
	warpPerspective(img, imgWrap, matrix, Point(w, h));

	return imgWrap;

}


void documentScanner(string path) {

	bool show = false;
	Mat img, imgGray, imgBlur, imgCanny, imgDil, imgErode, imgWarp, imgCrop;
	float w = 420, h = 596;

	// read image 
	img = readImage(path);

	// image pre processing
	tie(img, imgGray, imgBlur, imgCanny, imgDil, imgErode) = basicSegmentationOperations(img, show);

	// get contour corner points 
	vector<Point> initialPoints = getDocContours(imgErode, img);

	// organise points 
	vector<Point> orderedPoints = reorderPoints(initialPoints);
	
	// wrap
	imgWarp = getWrap(img, orderedPoints, w, h);
	
	// draw points 
	drawPointsOnImage(img, orderedPoints, Scalar(0, 0, 255));

	// Crop image 
	int cropVal = 10;
	Rect roi(cropVal, cropVal, w - (2 * cropVal), h - (2 * cropVal));
	imgCrop = imgWarp(roi);

	resize(img, img, Size(), 0.5, 0.5);
	imshow("Image", img);
	imshow("Image Wrapped", imgCrop);

	waitKey(0);

}


void liveDocumentScanner() {

	bool show = false;
	Mat img, imgGray, imgBlur, imgCanny, imgDil, imgErode, imgWarp, imgCrop;
	float w = 420, h = 596;
	vector<Point> initialPoints{}, orderedPoints{};
	VideoCapture cap(0);


	while (true) {

		cap.read(img);

		// image pre processing
		tie(img, imgGray, imgBlur, imgCanny, imgDil, imgErode) = basicSegmentationOperations(img, show);

		// get contour corner points

		initialPoints = getDocContours(imgErode, img);


		if (!initialPoints.empty() && initialPoints.size()==4) {
			// organise points 
			orderedPoints = reorderPoints(initialPoints);

			// wrap
			imgWarp = getWrap(img, orderedPoints, w, h);

			// draw points 
			drawPointsOnImage(img, orderedPoints, Scalar(0, 0, 255));

			// Crop image 
			int cropVal = 10;
			Rect roi(cropVal, cropVal, w - (2 * cropVal), h - (2 * cropVal));
			imgCrop = imgWarp(roi);

			// show document
			imshow("Scanned Document", imgCrop);
		}


		// show video
		resize(img, img, Size(), 0.5, 0.5);
		imshow("Video", img);

		char c = (char)waitKey(25);
		if (c == 27) {
			cap.release();
			break;
		}
	}
}

