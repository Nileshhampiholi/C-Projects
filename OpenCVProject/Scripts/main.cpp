#include "FaceDetection.h"
#include "VirtualPainter.h"
#include "DocumentScanner.h"
#include "NumPlateDetection.h"

int main() {

	string image_path = "Resources/test.png";
	string video_path = "Resources/test_video.mp4";
	string lambo_path = "Resources/lambp.jpg";
	string shapes_path = "Resources/shapes.png";
	string doc_image = "Resources/paper.jpg";
	bool show = true;


	// read and show image 
	Mat img = readImage(image_path);
	imshow("Image", img);
	waitKey(0);

	// Play video
	playVideo(video_path);

	// Stream webcam, press esc to exit
	streamWebcam();

	// Basic segmentation 
	basicSegmentationOperations(img, show);

	// sizing operations 
	basicImageOperations(image_path);

	// draw shapes 
	drawShapes();

	// wrap images
	wrapImages();

	// Color detector and mask generation press esc to exit
	colorDetection(lambo_path);
	
	// Detects objects using shapes
	detectObjects(shapes_path, show);

	// Color Picker with webcam
	colorPicker();

	// Face detection 
	faceDetection(image_path);


	// Virtual painter
	virtualPainter();


	// Document Scanner 
	documentScanner(doc_image);


	// Live document scanner from webcam
	liveDocumentScanner();


	// live number plate detection
	numPlateDetection();

}

