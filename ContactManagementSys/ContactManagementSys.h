#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_MainWindow.h"
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>
#include <QScrollBar>
#include <QAbstractItemModel>
#include <QVariant>
#include "Contact.h"
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
#include <QPainterPath>
#include "Pinyin.h"
#include <qicon.h>


class ContactManagementSys : public QMainWindow
{
    Q_OBJECT

public:
    ContactManagementSys(QWidget *parent = nullptr);
    void initLetterList();
    void loadData();
    void insertRow(Contact *contact);
    ~ContactManagementSys();

private:
    Ui::ContactManagementSysClass ui;
    bool m_dragging = false;
    QPoint m_dragPos;
    QStandardItemModel* model = nullptr;

private slots:
    void onScrollBarValueChanged(int value);
    void onAddBtnClicked();
    void onSearchBoxTextChanged(const QString& text);
    void onSortLetterListItemClicked(const QModelIndex& index);
    void onTableViewItemCountChanged();
    void onCloseBtnClicked();

protected:

    void mousePressEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            m_dragging = true;
            m_dragPos = event->globalPos() - frameGeometry().topLeft();
        }
    }

    void mouseMoveEvent(QMouseEvent* event) override
    {
        if (m_dragging)
        {
            move(event->globalPos() - m_dragPos);
            event->accept();
        }
    }

    void mouseReleaseEvent(QMouseEvent* event) override
    {
        if (event->button() == Qt::LeftButton)
        {
            m_dragging = false;
        }
    }
};
