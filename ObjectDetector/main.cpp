#include "ObjectDetector.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ObjectDetector w;
    w.show();
    return a.exec();
}
