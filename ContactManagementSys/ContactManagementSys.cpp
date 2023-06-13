// -*- coding: utf-8 -*-
#include <Windows.h>
#include "ContactManagementSys.h"
#include "AddEditWindow.h"

using namespace std;
using namespace WzhePinYin;

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

    ui.addButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(ui.addButton, &QPushButton::clicked, this, &ContactManagementSys::onAddBtnClicked);
    connect(ui.contactTable->verticalScrollBar(), &QScrollBar::valueChanged, this, &ContactManagementSys::onScrollBarValueChanged);

    insertShuffleData();
    loadData();
}

void ContactManagementSys::onAddBtnClicked() {
    AddEditWindow* addEditWindow = new AddEditWindow();
	addEditWindow->show();
}
void ContactManagementSys::loadData() {
    ContactList contactList;
    contactList.loadFromFile("data.json");
    Contact* contact = contactList.getHead();
    while (contact->next != nullptr) {
        insertRow(contact);
        contact = contact->next;
	}
}

void ContactManagementSys::insertRow(Contact* contact) {
    QStandardItemModel* model = new QStandardItemModel();
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
    ui.contactTable->setModel(model);
    ui.contactTable->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui.contactTable->setShowGrid(false);
    ui.contactTable->setSelectionMode(QAbstractItemView::SingleSelection);
}

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
 //   QStandardItemModel* model = new QStandardItemModel();
 //   // 循环插入50个项目，内容是 A-Z 开始的随机字符
 //   // QString 数组， 存放随机汉字
 //   QString strChinese[] = { "王","李","张","刘","陈","杨","黄","赵","周","吴","徐","孙","胡","朱","高","林","何","郭","马","罗","梁","宋","郑","谢","韩","唐","冯","于","董","萧","程","曹","袁","邓","许","傅","沈","曾","彭","吕","苏","卢","蒋","蔡","贾","丁","魏","薛","叶","阎","余","潘","杜","戴","夏","钟","汪","田","任","姜","范","方","石","姚","谭","廖","邹","熊","金","陆","郝","孔","白","崔","康","毛","邱","秦","江","史","顾","侯","邵","孟","龙","万","段","漕","钱","汤","尹","黎","易","常","武","乔","贺","赖","龚","文" };
 //   srand((unsigned)time(NULL));
 //   for (int i = 0; i < 150; i++) {
	//	QStandardItem* item = new QStandardItem();
 //       QString name = strChinese[rand() % 100] + strChinese[rand() % 100] + strChinese[rand() % 100] + strChinese[rand() % 100] + strChinese[rand() % 100];
	//	item->setText(name);
 //       QStandardItem* item2 = new QStandardItem();
 //       QPixmap pixmap(":/ContactManagementSys/Assets/Images/phone.png");
 //       QIcon icon(pixmap);
 //       item2->setIcon(icon);
 //       QList<QStandardItem*> row;
 //       row << item << item2;
	//	model->appendRow(row);
	//}
 //   //ui.contactTable->setModel(model);
 //   //ui.contactTable->setSelectionBehavior(QAbstractItemView::SelectRows);
 //   //ui.contactTable->setShowGrid(false);
 //   //ui.contactTable->setSelectionMode(QAbstractItemView::SingleSelection);
 //   //// set header invisible
 //   //ui.contactTable->horizontalHeader()->setVisible(false);
 //   //ui.contactTable->verticalHeader()->setVisible(false);
 //   //ui.contactTable->setFocusPolicy(Qt::NoFocus);
 //   //ui.contactTable->setColumnWidth(0, ui.contactTable->width() - 130);
 //   // set header invisible
    
    
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
