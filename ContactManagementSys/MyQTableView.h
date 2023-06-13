#pragma once

#include <QTableView>
#include "ui_MyQTableView.h"
#include <QMouseEvent>

class MyQTableView : public QTableView
{
	Q_OBJECT

public:
	MyQTableView(QWidget *parent = nullptr);
	~MyQTableView();

private:
	Ui::MyQTableViewClass ui;

protected:
	void mouseMoveEvent(QMouseEvent* event) override;
};
