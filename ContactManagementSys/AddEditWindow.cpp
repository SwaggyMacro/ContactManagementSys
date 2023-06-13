#include "AddEditWindow.h"
#include "qpixmap.h"
#include "qpainter.h"
#include "qsize.h"
#include "qrect.h"
#include "qpoint.h"


AddEditWindow::AddEditWindow(QWidget *parent)
	: QMainWindow(parent)
{
	ui.setupUi(this);
	setWindowFlags(Qt::FramelessWindowHint);
	setAttribute(Qt::WA_TranslucentBackground);

	this->setContentsMargins(1, 1, 1, 1);
	ui.statusBar->setVisible(false);
	//QPixmap avatarImg(":/ContactManagementSys/Assets/Images/avatar.jpg");
	////ui.avatarLabel->setAlignment(Qt::AlignCenter);

	// 把图片进行缩放裁剪，然后再放到qss的路径
	QPixmap avatarImg(":/ContactManagementSys/Assets/Images/avatar.jpg");
	QPixmap fitpixmap = avatarImg.scaled(ui.avatarLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
	fitpixmap.save("Assets/Images/Crop/avatar.jpg");
	ui.avatarLabel->setStyleSheet("background-image: url(Assets/Images/Crop/avatar.jpg); background-repeat: no-repeat; background-position: center center; border-radius: 45px;");

	ui.tbName->setClearButtonEnabled(true);
	ui.tbName->setPlaceholderText("姓名");

	ui.tbComName->setClearButtonEnabled(true);
	ui.tbComName->setPlaceholderText("公司");

	ui.tbPhone->setPlaceholderText("联系电话");
	ui.tbEmail->setPlaceholderText("邮箱");
	ui.tbAddress->setPlaceholderText("通讯地址");
	ui.tbComPos->setPlaceholderText("职位");
	ui.tbComPos->setClearButtonEnabled(true);
	ui.tbPhone->setClearButtonEnabled(true);
	ui.tbEmail->setClearButtonEnabled(true);
	ui.tbAddress->setClearButtonEnabled(true);
	
	ui.saveBtn->setIcon(QIcon(":/ContactManagementSys/Assets/Images/icon_confirm_32.png"));
	ui.saveBtn->setIconSize(QSize(22, 22));

	ui.cancelBtn->setIcon(QIcon(":/ContactManagementSys/Assets/Images/icon_cancel.png"));
	ui.cancelBtn->setIconSize(QSize(22, 22));
	
	ui.saveBtn->setCursor(QCursor(Qt::PointingHandCursor));
	ui.cancelBtn->setCursor(QCursor(Qt::PointingHandCursor));

	ui.avatarLabel->setCursor(QCursor(Qt::PointingHandCursor));
}

AddEditWindow::~AddEditWindow()
{
}
