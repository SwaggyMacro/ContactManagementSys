// -*- coding: utf-8 -*-
#include <Windows.h>
#include "ContactManagementSys.h"
#include "AddEditWindow.h"

using namespace std;
using namespace WzhePinYin;

class CustomItemDelegate : public QStyledItemDelegate {
public:
    using QStyledItemDelegate::QStyledItemDelegate;

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override {
        if (option.state & QStyle::State_MouseOver) {
            int row = index.row();

            QTableView* tableView = qobject_cast<QTableView*>(parent());
            if (tableView) {
                if (tableView->isColumnHidden(2)) {
                    tableView->setColumnHidden(2, false);
                    tableView->setColumnHidden(3, false);
                }
            }
        }
        else {
            QTableView* tableView = qobject_cast<QTableView*>(parent());
            if (tableView) {
                if (!tableView->isColumnHidden(2)) {
                    tableView->setColumnHidden(2, true);
                    tableView->setColumnHidden(2, true);
                }
            }
        }
        QStyledItemDelegate::paint(painter, option, index);
    }
};

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

    ui.searchBox->setClearButtonEnabled(true);
    ui.contactTable->verticalHeader()->setDefaultSectionSize(50);

    ui.contactTable->horizontalHeader()->setVisible(false);
    ui.contactTable->verticalHeader()->setVisible(false);
    ui.contactTable->setFocusPolicy(Qt::NoFocus);
    ui.contactTable->setColumnWidth(0, ui.contactTable->width() - 130);
    ui.contactTable->setColumnWidth(1, 100);
    ui.contactTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.contactTable->setItemDelegate(new CustomItemDelegate(ui.contactTable));

    ui.addButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(ui.addButton, &QPushButton::clicked, this, &ContactManagementSys::onAddBtnClicked);
    connect(ui.contactTable->verticalScrollBar(), &QScrollBar::valueChanged, this, &ContactManagementSys::onScrollBarValueChanged);

    insertShuffleData();
    loadData();

    hideColumn(ui.contactTable, 1);
    hideColumn(ui.contactTable, 2);
    hideColumn(ui.contactTable, 3);

}

void ContactManagementSys::onAddBtnClicked() {
    AddEditWindow* addEditWindow = new AddEditWindow();
	addEditWindow->show();
}

/// <summary>
/// load data from local to the contact table
/// </summary>
void ContactManagementSys::loadData() {
    ContactList contactList;
    contactList.loadFromFile("data.json");
    Contact* contact = contactList.getHead();
    while (contact != nullptr && contact->next != nullptr) {
        insertRow(contact);
        contact = contact->next;
	}
}


void ContactManagementSys::hideColumn(QTableView* table, int column) {
	table->setColumnHidden(column, true);
}

/// <summary>
/// insert row to the contact table
/// </summary>
/// <param name="contact">the contact</param>
void ContactManagementSys::insertRow(Contact* contact) {
    if (!contact) {
        return;
    }

    if (model == nullptr) {
        model = new QStandardItemModel();
        ui.contactTable->setModel(model);
        ui.contactTable->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui.contactTable->setShowGrid(false);
        ui.contactTable->setSelectionMode(QAbstractItemView::SingleSelection);
    }

    QStandardItem* itemName = new QStandardItem();
    QStandardItem* itemPhone = new QStandardItem();

    QString name = QString::fromStdString(contact->name);
    QString phone = QString::fromStdString(contact->phone);

    itemName->setText(name);
    itemPhone->setText(phone);

    QStandardItem* itemEdit = new QStandardItem();
    QStandardItem* itemDel = new QStandardItem();
    QPixmap pEdit(":/ContactManagementSys/Assets/Images/edit.png");
    QPixmap pDel(":/ContactManagementSys/Assets/Images/delete.png");

    QIcon iconEdit(pEdit);
    itemEdit->setIcon(iconEdit);
    QIcon iconDel(pDel);
    itemDel->setIcon(iconDel);

    QList<QStandardItem*> row;
    row << itemName << itemPhone << itemEdit << itemDel;
    model->appendRow(row);
    
}

/// <summary>
/// Vertical scroll bar value changed event
/// </summary>
/// <param name="value"></param>
void ContactManagementSys::onScrollBarValueChanged(int value) {
    QModelIndex index = ui.contactTable->indexAt(QPoint(0, value));

    // return if the index is invalid
    if (!index.isValid()) {
        return;
    }
    // get the data of the index
    QVariant data = ui.contactTable->model()->data(index);
    // get the first letter of the data
    QString firstLetter = data.toString().left(1);

    wchar_t* wstr = new wchar_t[firstLetter.length() + 1];
    firstLetter.toWCharArray(wstr);

    // if the first letter is Chinese, get the pinyin of the first letter
    if (WzhePinYin::Pinyin::IsChinese(*wstr)) {
        wstr[firstLetter.length()] = L'\0';
        std::vector<std::string> pinyins = WzhePinYin::Pinyin::GetPinyins(*wstr);
        firstLetter = QString::fromStdString(pinyins[0]).left(1);
    }

    QAbstractItemModel* model = ui.sortLetterList->model();
    for (int i = 0; i < model->rowCount(); i++) {
        QModelIndex index = model->index(i, 0);
        QVariant data = model->data(index);
        QString letter = data.toString().left(1);
        if (letter == firstLetter) {
            model->setData(index, QColor("#1d81c4"), Qt::ForegroundRole);
        }
        else {
            model->setData(index, QColor("#848484"), Qt::ForegroundRole);
        }
    }
}


void ContactManagementSys::insertShuffleData() {
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
