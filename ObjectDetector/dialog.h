#pragma once

#include <QWidget>
#include "ui_dialog.h"

class dialog : public QWidget, public Ui::dialog
{
	Q_OBJECT

public:
	dialog(QWidget *parent = Q_NULLPTR);
	~dialog();


private slots:
	void on_pushButton_clicked();
};
