#include "MyQTableView.h"

MyQTableView::MyQTableView(QWidget *parent)
	: QTableView(parent)
{
	ui.setupUi(this);
	// set editable to false
	setEditTriggers(QAbstractItemView::NoEditTriggers);
	// set selection mode to single selection
	setSelectionBehavior(QAbstractItemView::SelectRows);
}

void MyQTableView::mouseMoveEvent(QMouseEvent* event)
{
	if(model() == nullptr || model()->rowCount() == 0) return;
	QTableView::mouseMoveEvent(event);
	QModelIndex index = indexAt(event->pos());
	int row = index.row();
	int rows = model()->rowCount();
	setColumnHidden(2, false);
	setColumnHidden(3, false);
	setColumnWidth(0, 250);
	setColumnWidth(2, 40);
	setColumnWidth(3, 40);
	for (int i = 0; i < rows; i++) {
		if (i != row) {
			setSpan(i, 0, 1, 4);
		}
	}
	if (index.isValid()) {
		for (int i = 1; i <= 4; i++) {
			setSpan(row, 0, 1, 1); // 设置单元格跨度为1x0，显示特定行的列
		}
	}
}
MyQTableView::~MyQTableView()
{}
