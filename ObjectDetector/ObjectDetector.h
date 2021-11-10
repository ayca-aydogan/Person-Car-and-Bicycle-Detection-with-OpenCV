#pragma once

#include <QtWidgets/QMainWindow>
#include <QtWidgets>
#include <qpixmap.h>
#include <qfiledialog.h>
#include <qurl.h>
#include <qboxlayout.h>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/objdetect.hpp>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <opencv2/dnn/dnn.hpp>
#include <opencv2/dnn.hpp>
#include <opencv2/opencv.hpp>
#include <vector>

using namespace cv;
using namespace std;
using namespace cv::dnn;

#include "ui_ObjectDetector.h"
#include "dialog.h"



class ObjectDetector : public QMainWindow
{
    Q_OBJECT

public:
    ObjectDetector(QWidget *parent = Q_NULLPTR);

private:
    Ui::ObjectDetectorClass ui;
    dialog* dia = new dialog();
    QString filename;

    void detector(QString path, QString obj);

private slots:
    void on_startbtn_clicked();
    void on_browsebtn_clicked();
};
