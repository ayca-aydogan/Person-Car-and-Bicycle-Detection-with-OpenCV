#include "ObjectDetector.h"


string weight_file = "Resources/yolov3.weights";
string cfg_file = "Resources/yolov3.cfg";

ObjectDetector::ObjectDetector(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    this->setStyleSheet("background-color: #D5D3CF");
	ui.frame->setStyleSheet("background-color: #EAD288");
	ui.browsebtn->setStyleSheet("background-color: #F3EDDE");
	ui.chsobj->setStyleSheet("background-color: #F3EDDE");
	ui.pathtxt->setStyleSheet("background-color: #F3EDDE");
	ui.chsobj->setStyleSheet("background-color: #F3EDDE");
	ui.startbtn->setStyleSheet("background-color: #F3EDDE");
	ui.label->setStyleSheet("background-color: #EAD288; border: 10px solid black");

	ui.titlelabel->setText("Processed Video: ");

	//Add objects to the combobox
	ui.chsobj->addItem("Person");
	ui.chsobj->addItem("Car");
	ui.chsobj->addItem("Bicycle");
	ui.chsobj->setItemData(0, QColor(243, 237, 222), Qt::BackgroundRole );
	ui.chsobj->setItemData(1, QColor(243, 237, 222), Qt::BackgroundRole);
	ui.chsobj->setItemData(2, QColor(243, 237, 222), Qt::BackgroundRole);
}

void ObjectDetector::on_browsebtn_clicked() {
    filename = QFileDialog::getOpenFileName(this, "Open a File", "", "Video File (*.avi *.mpg *.mp4 *.mkv)");
    ui.pathtxt->setText(filename);
}

void ObjectDetector::on_startbtn_clicked() {
	if (ui.pathtxt->toPlainText() == "" || ui.chsobj->currentText() == "Select Object") {
		dia->label->setText("Please Fill All Fields!");
		dia->show();
	}
	else {
		QString qp{ ui.pathtxt->toPlainText() }, qo{ ui.chsobj->currentText() };
		detector(qp, qo);
	}
}

void ObjectDetector::detector(QString path, QString obj) {

	string  object { obj.toStdString() };

	VideoCapture cap;
	bool ok;
	int p = path.toInt(&ok, 10);

	if (ok == true)
		cap.open(p);
	else
		cap.open(path.toStdString());


	ui.titlelabel->setText("Processed Video: Capturing " + obj);
	int idxnumber;
	if (object == "Person")
		idxnumber = 0;
	else if (object == "Bicycle")
		idxnumber = 1;
	else if (object == "Car")
		idxnumber = 2;

	//Create and load neural networks
	Net net = readNetFromDarknet(cfg_file, weight_file);

	if (net.empty())
	{
		dia->label->setText("read caffe model data error");
	}

	//Set calculation background
	net.setPreferableBackend(DNN_BACKEND_OPENCV);
	net.setPreferableTarget(DNN_TARGET_OPENCL);


	//Get all output layers
	vector<string> outNames = net.getUnconnectedOutLayersNames();

	Mat src;

	while (cap.read(src)) {

		if (src.empty())
		{
			dia->label->setText("pic read error"); 
		}

		//Picture format conversion
		Mat blobimage = blobFromImage(src, 0.00392, Size(416, 416), Scalar(), true, false);

		//Network input data
		net.setInput(blobimage);

		//yolo has multiple output layers to obtain identification data
		vector<Mat> outs;
		net.forward(outs, outNames);

		//Each layer has a rectangular confidence label index
		vector<Rect> boxes;
		vector<int> classIds;
		vector<float> confidences;

		//analysis
		for (size_t i = 0; i < outs.size(); i++)
		{
			float* data = (float*)outs[i].data;
			//Parse each row of data for each output layer
			for (int j = 0; j < outs[i].rows; j++, data += outs[i].cols)
			{
				//Remove the box data in each line and get the box location information
				Mat scores = outs[i].row(j).colRange(5, outs[i].cols);
				Point classIdPoint;   //Maximum position
				double confidence;    //Get confidence value
				minMaxLoc(scores, 0, &confidence, 0, &classIdPoint);
				if (confidence > 0.5)
				{
					int centerx = (int)(data[0] * src.cols);
					int centery = (int)(data[1] * src.rows);
					int width = (int)(data[2] * src.cols);
					int height = (int)(data[3] * src.rows);
					int left = centerx - width / 2;
					int top = centery - height / 2;
					classIds.push_back(classIdPoint.x);
					confidences.push_back(confidence);
					boxes.push_back(Rect(left, top, width, height));
				}
			}
		}

		vector<int> indexes;
		// Delete duplicate box
		NMSBoxes(boxes, confidences, 0.5, 0.4, indexes);
		for (auto i:indexes)
		{
			int idx = classIds[i];
			if (idx == idxnumber) {
				Rect box = boxes[i];
				rectangle(src, box, Scalar(0, 0, 255), 1, 8);
			}
		}

		// Convert image from Mat to Pixmap
		QImage imgIn = QImage((uchar*)src.data, src.cols, src.rows, src.step, QImage::Format_BGR888);
		QPixmap pix = QPixmap::fromImage(imgIn);
		ui.label->setPixmap(pix.scaled(650, 500, Qt::KeepAspectRatio));

		waitKey(1);

		//to speed up the video
		for (int i = 0; i < 8; i++)
			cap.read(src);
	}
	destroyAllWindows();
}