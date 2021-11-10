#include "dialog.h"

dialog::dialog(QWidget *parent)
	: QWidget(parent)
{
	setupUi(this);

	pushButton->setStyleSheet("background-color: #F3EDDE");
}

dialog::~dialog()
{
}


void dialog::on_pushButton_clicked() {
	this->close();
}
