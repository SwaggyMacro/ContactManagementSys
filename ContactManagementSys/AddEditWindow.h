#pragma once

#include <QMainWindow>
#include "ui_AddEditWindow.h"
#include <QApplication>
#include <QWidget>
#include <QPainter>
#include <QPainterPath>
#include <QGraphicsDropShadowEffect>
#include <QMouseEvent>

class AddEditWindow : public QMainWindow
{
	Q_OBJECT

public:
	AddEditWindow(QWidget *parent = nullptr);
	~AddEditWindow();

private:
	Ui::AddEditWindowClass ui;
    bool m_dragging = false;
    QPoint m_dragPos;
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

