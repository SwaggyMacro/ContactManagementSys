#include <Windows.h>
#include "ContactManagementSys.h"
#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>
#include <QtWidgets>
#include <qlistview.h>


ContactManagementSys::ContactManagementSys(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    setWindowFlags(Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);

    this->setContentsMargins(1, 1, 1, 1);


    // set windows cannot resize
    setFixedSize(this->width(), this->height());
   
    ui.statusBar->setVisible(false);

    QGraphicsDropShadowEffect* dse = new QGraphicsDropShadowEffect();
    dse->setBlurRadius(10);
    dse->setColor(Qt::green);
    dse->setOffset(1, 1);
    ui.labelTitle->setGraphicsEffect(dse);
    insertShuffleData();
}
void ContactManagementSys::insertShuffleData() {
    QStandardItemModel* model = new QStandardItemModel();
    // loop insert 50 items
    for (int i = 0; i < 50; i++) {
		QStandardItem* item = new QStandardItem();
        // insert letter start from a to z
		item->setText(QString("item %1").arg(i));
		model->appendRow(item);
	}
    ui.contactTable->setModel(model);
    ui.contactTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.contactTable->setShowGrid(false);
    ui.contactTable->setSelectionMode(QAbstractItemView::SingleSelection);
    // set header invisible
    ui.contactTable->horizontalHeader()->setVisible(false);
    ui.contactTable->verticalHeader()->setVisible(false);
    ui.contactTable->setFocusPolicy(Qt::NoFocus);
    
    // insert a-z and # to sortLetterList
    QStringList sortLetterList;
    sortLetterList << "A" << "B" << "C" << "D" << "E" << "F" << "G" << "H"
		<< "I" << "J" << "K" << "L" << "M" << "N" << "O" << "P"
		<< "Q" << "R" << "S" << "T" << "U" << "V" << "W" << "X"
		<< "Y" << "Z" << "#";
    // set sortLetterList to sortLetterModel
	QStandardItemModel* sortLetterModel = new QStandardItemModel();
	for (int i = 0; i < sortLetterList.size(); i++) {
        
        QStandardItem* item = new QStandardItem();
        item->setText(sortLetterList.at(i));
        sortLetterModel->appendRow(item);
    }
    ui.sortLetterList->setModel(sortLetterModel);
    ui.sortLetterList->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.sortLetterList->setSelectionMode(QAbstractItemView::SingleSelection);
    ui.sortLetterList->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.sortLetterList->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.sortLetterList->setFocusPolicy(Qt::NoFocus);
}

ContactManagementSys::~ContactManagementSys()
{
}
