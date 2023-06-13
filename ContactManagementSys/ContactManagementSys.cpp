// -*- coding: utf-8 -*-
#include <Windows.h>
#include "ContactManagementSys.h"
#include "AddEditWindow.h"

using namespace std;
using namespace WzhePinYin;

class CustomDelegate : public QStyledItemDelegate
{
public:
    ContactManagementSys* contactManagementSys;
    explicit CustomDelegate(ContactManagementSys* cms, QObject* parent = nullptr) : QStyledItemDelegate(parent), contactManagementSys(cms) {}

    void paint(QPainter* painter, const QStyleOptionViewItem& option, const QModelIndex& index) const override
    {
        QStyledItemDelegate::paint(painter, option, index);

        if (index.column() == 0) {
            QIcon icon = qvariant_cast<QIcon>(index.data(Qt::DecorationRole));
            if (!icon.isNull()) {
                QRect iconRect = option.rect.adjusted(4, 4, -4, -4);
                icon.paint(painter, iconRect, Qt::AlignCenter);
            }
        }
    }

    bool editorEvent(QEvent* event, QAbstractItemModel* model, const QStyleOptionViewItem& option, const QModelIndex& index) override
    {
        if (event->type() == QEvent::MouseButtonRelease) {
            QMouseEvent* mouseEvent = static_cast<QMouseEvent*>(event);

            // edit icon click event
            if (index.column() == 2 && option.rect.contains(mouseEvent->pos())) {
                qDebug() << "edit Icon clicked at row" << index.row();

                QString name = model->data(model->index(index.row(), 0)).toString();
                qDebug() << "name:" << name;
                QString phone = model->data(model->index(index.row(), 1)).toString();
                qDebug() << "phone:" << phone;

                AddEditWindow* addEditWindow = new AddEditWindow(nullptr, 1, phone);
                addEditWindow->show();
                connect(addEditWindow, &AddEditWindow::windowClosed, [&]() {
                    qDebug() << "Window closed, reload data.";
                    contactManagementSys->loadData();
                });
                return true;
            }

            // delete icon click event
            if (index.column() == 3 && option.rect.contains(mouseEvent->pos())) {
                qDebug() << "delete Icon clicked at row" << index.row();
                QString name = model->data(model->index(index.row(), 0)).toString();
                qDebug() << "name:" << name;
                QString phone = model->data(model->index(index.row(), 1)).toString();
                qDebug() << "phone:" << phone;

                QMessageBox msgBox;
                msgBox.setText("是否确定删除联系人？");
                msgBox.setIcon(QMessageBox::Question);
                msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
                msgBox.setDefaultButton(QMessageBox::No);
                int ret = msgBox.exec();
                switch (ret) {
                    case QMessageBox::Yes:
					    qDebug() << "Yes clicked";
                        ContactList contactList;
                        contactList.loadFromFile("data.json");
                        contactList.deleteContact(phone.toStdString());
                        contactList.saveToFile("data.json");
					    contactManagementSys->loadData();
					break;
                }
                return true;
            }
        }

        return QStyledItemDelegate::editorEvent(event, model, option, index);
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
    ui.contactTable->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    ui.contactTable->setItemDelegate(new CustomDelegate(this, ui.contactTable));

    ui.addButton->setCursor(QCursor(Qt::PointingHandCursor));

    connect(ui.addButton, &QPushButton::clicked, this, &ContactManagementSys::onAddBtnClicked);
    connect(ui.contactTable->verticalScrollBar(), &QScrollBar::valueChanged, this, &ContactManagementSys::onScrollBarValueChanged);

    onScrollBarValueChanged(0);

    ui.contactTable->setColumnHidden(1, true);
    // searchBox textChanged and search in tableview
    connect(ui.searchBox, &QLineEdit::textChanged, this, &ContactManagementSys::onSearchBoxTextChanged);

    // sortLetterList(ListView) item click event
    connect(ui.sortLetterList, &QListView::clicked, this, &ContactManagementSys::onSortLetterListItemClicked);

    // tableview item count changed event
    connect(model, &QStandardItemModel::rowsInserted, this, &ContactManagementSys::onTableViewItemCountChanged);
    connect(model, &QStandardItemModel::rowsRemoved, this, &ContactManagementSys::onTableViewItemCountChanged);

    connect(ui.closeBtn, &QPushButton::clicked, this, &ContactManagementSys::onCloseBtnClicked);

    initLetterList();
    loadData();

   }

void ContactManagementSys::onCloseBtnClicked() {
    exit(0);
}

void ContactManagementSys::onTableViewItemCountChanged() {
	ui.labelCount->setText(QString::number(model->rowCount()) + " 已找到");
}

void ContactManagementSys::onSortLetterListItemClicked(const QModelIndex& index) {
    qDebug() << "sortLetterList item clicked:" << index.data().toString();
	QString letter = index.data().toString();
    for (int i = 0; i < model->rowCount(); i++) {
		QString name = model->data(model->index(i, 0)).toString().toUpper();
        wchar_t* wstrName = new wchar_t[name.length() + 1];
        name.toWCharArray(wstrName);
        if (WzhePinYin::Pinyin::IsChinese(*wstrName)) {
			wstrName[name.length()] = L'\0';
			std::vector<std::string> pinyins = WzhePinYin::Pinyin::GetPinyins(*wstrName);
			name = QString::fromStdString(pinyins[0]);
        }
        if (name.startsWith(letter)) {
            ui.contactTable->scrollTo(model->index(i, 0));
        }
        else {
            // 取消选中状态
            ui.sortLetterList->selectionModel()->clearSelection();
        }
	}
}

void  ContactManagementSys::onSearchBoxTextChanged(const QString& text) {
    int count = 0;
	qDebug() << "search text changed:" << text;
    // search from the contact table
    for (int i = 0; i < model->rowCount(); i++) {
        bool isChinese = false;

        QString name = model->data(model->index(i, 0)).toString();
		QString phone = model->data(model->index(i, 1)).toString();
        
        wchar_t* wstrName = new wchar_t[name.length() + 1];
        name.toWCharArray(wstrName);
        
        if (Pinyin::IsChinese(*wstrName)) {
            QRegularExpression re("^[\u4e00-\u9fa5]+$");
            QRegularExpressionMatch match = re.match(text);

            // if the text is chinese, then search the chinese name, or convert to pinyin letters.
            if (!match.hasMatch()) {
                isChinese = true;
                std::vector<std::string> pinyins = WzhePinYin::Pinyin::GetPinyins(*wstrName);
                name = QString::fromStdString(pinyins[0]).toLower();
            }
        }

        if (name.contains(isChinese ? text.toLower() : text) || phone.contains(isChinese ? text.toLower() : text)) {
			ui.contactTable->showRow(i);
            count++;
		}
        else {
			ui.contactTable->hideRow(i);
		}
	}
    ui.labelCount->setText(QString::number(model->rowCount()) + " 已找到");
}

void ContactManagementSys::onAddBtnClicked() {
    AddEditWindow* addEditWindow = new AddEditWindow();
	addEditWindow->show();
    connect(addEditWindow, &AddEditWindow::windowClosed, [&]() {
        qDebug() << "Window closed, reload data.";
        loadData();
    });
}

/// <summary>
/// load data from local to the contact table
/// </summary>
void ContactManagementSys::loadData() {
    // clear contactTable
    if (model != nullptr) {
		model->clear();
	}
    ContactList contactList;
    contactList.loadFromFile("data.json");
    Contact* contact = contactList.getHead();
    contactList.sortByName();
    while (contact != nullptr) {
        insertRow(contact);
        if (contact->next == nullptr) break;
        contact = contact->next;
	}
    onTableViewItemCountChanged();
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
    QPixmap pEdit(":/ContactManagementSys/Assets/Images/edit_green.png");
    QPixmap pDel(":/ContactManagementSys/Assets/Images/delete.png");

    QIcon iconEdit(pEdit);
    itemEdit->setIcon(iconEdit);
    QIcon iconDel(pDel);
    itemDel->setIcon(iconDel);

    QList<QStandardItem*> row;
    row << itemName << itemPhone << itemEdit << itemDel;
    model->appendRow(row);

    // set the ror span 4 col
    ui.contactTable->setSpan(model->rowCount() - 1, 0, 1, 4);

    ui.contactTable->setColumnHidden(1, true);
    
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
    QString firstLetter = data.toString().left(1).toUpper();

    wchar_t* wstr = new wchar_t[firstLetter.length() + 1];
    firstLetter.toWCharArray(wstr);

    // if the first letter is Chinese, get the pinyin of the first letter
    if (WzhePinYin::Pinyin::IsChinese(*wstr)) {
        wstr[firstLetter.length()] = L'\0';
        std::vector<std::string> pinyins = WzhePinYin::Pinyin::GetPinyins(*wstr);
        firstLetter = QString::fromStdString(pinyins[0]).left(1);
    }

    QAbstractItemModel* model = ui.sortLetterList->model();
    // get the last index of the sortLetterList
    QModelIndex otherIndex = model->index(model->rowCount() - 1, 0);

    for (int i = 0; i < model->rowCount(); i++) {
        QModelIndex index = model->index(i, 0);
        QVariant data = model->data(index);
        QString letter = data.toString().left(1);
        

        if (letter == firstLetter) {
            model->setData(index, QColor("#1d81c4"), Qt::ForegroundRole);
        }
        else {
            model->setData(index, QColor("#848484"), Qt::ForegroundRole);
            if (!QString("ABCDEFGHIJKLMNOPQRSTUVWXYZ").contains(firstLetter.toUpper())) {
				model->setData(otherIndex, QColor("#1d81c4"), Qt::ForegroundRole);
            }
            else {
                model->setData(otherIndex, QColor("#848484"), Qt::ForegroundRole);
            }
        }
    }
}


void ContactManagementSys::initLetterList() {
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
