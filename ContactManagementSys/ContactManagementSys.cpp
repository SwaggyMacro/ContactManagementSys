// -*- coding: utf-8 -*-
#include <Windows.h>
#include "ContactManagementSys.h"
#include <QGraphicsDropShadowEffect>
#include <QVBoxLayout>
#include <QtWidgets>
#include <qlistview.h>
#include <qlabel.h>
#include <qtextdocumentfragment.h>
#include <qimage.h>
#include <qicon.h>
#include <qpixmap.h>
#include <QModelIndex>
#include <QScrollBar>
#include <QAbstractItemModel>
#include <QVariant>
#include <QPainter>
#include <QPainterPath>
#include "Pinyin.h"
#include "AddEditWindow.h"

using namespace std;
using namespace WzhePinYin;


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

    ui.addButton->setCursor(QCursor(Qt::PointingHandCursor));

    // addButton 点击事件 打开AddEditWindow 不以子窗口的形式打开
    connect(ui.addButton, &QPushButton::clicked, this, [=]() {
		AddEditWindow* addEditWindow = new AddEditWindow();
		addEditWindow->show();
	});




    // contactTable 监听滚动事件，每当滚动时获取可视范围内的第一个项目的内容的第一个字母，然后跟sortLetterList中的内容比对，如果匹配则改变sortLetterList中该项目的字体颜色
// 获取垂直滚动条
    QScrollBar* scrollBar = ui.contactTable->verticalScrollBar();

    // 监听滚动事件
    connect(scrollBar, &QScrollBar::valueChanged, this, [=](int value) {
        // 获取可视范围内的第一个项目的索引
        QModelIndex index = ui.contactTable->indexAt(QPoint(0, value));

        // 如果索引无效则返回
        if (!index.isValid()) {
			return;
		}

        // 获取该项目的内容
        QVariant data = ui.contactTable->model()->data(index);
        // 提取该内容的第一个字母
        QString firstLetter = data.toString().left(1);
        //qDebug() << firstLetter;
        wchar_t* wstr = new wchar_t[firstLetter.length() + 1];
        firstLetter.toWCharArray(wstr);
        if (WzhePinYin::Pinyin::IsChinese(*wstr)) {
            wstr[firstLetter.length()] = L'\0';
            std::vector<std::string> pinyins = WzhePinYin::Pinyin::GetPinyins(*wstr);
            firstLetter = QString::fromStdString(pinyins[0]).left(1);
           // qDebug() << firstLetter;
		}

        // 比对sortLetterList中的内容
        QAbstractItemModel* model = ui.sortLetterList->model();
        for (int i = 0; i < model->rowCount(); i++) {
            QModelIndex index = model->index(i, 0);
            QVariant data = model->data(index);
            QString letter = data.toString().left(1);
            if (letter == firstLetter) {
                // 改变该行的字体颜色
                model->setData(index, QColor("#1d81c4"), Qt::ForegroundRole);
            }
            else {
                // 恢复该项目的字体颜色
                model->setData(index, QColor("#848484"), Qt::ForegroundRole);
            }
        }
        });




    insertShuffleData();
}

void ContactManagementSys::insertShuffleData() {
    QStandardItemModel* model = new QStandardItemModel();
    // 循环插入50个项目，内容是 A-Z 开始的随机字符
    // QString 数组， 存放随机汉字
    QString strChinese[] = { "王","李","张","刘","陈","杨","黄","赵","周","吴","徐","孙","胡","朱","高","林","何","郭","马","罗","梁","宋","郑","谢","韩","唐","冯","于","董","萧","程","曹","袁","邓","许","傅","沈","曾","彭","吕","苏","卢","蒋","蔡","贾","丁","魏","薛","叶","阎","余","潘","杜","戴","夏","钟","汪","田","任","姜","范","方","石","姚","谭","廖","邹","熊","金","陆","郝","孔","白","崔","康","毛","邱","秦","江","史","顾","侯","邵","孟","龙","万","段","漕","钱","汤","尹","黎","易","常","武","乔","贺","赖","龚","文" };
    srand((unsigned)time(NULL));
    for (int i = 0; i < 150; i++) {
		QStandardItem* item = new QStandardItem();
        QString name = strChinese[rand() % 100] + strChinese[rand() % 100];
		item->setText(name);
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
    ui.contactTable->setColumnWidth(0, ui.contactTable->width() - 130);
    

    
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
