#include "AddEditWindow.h"
#include "qpixmap.h"
#include "qpainter.h"
#include "qsize.h"
#include "qrect.h"
#include "qpoint.h"
#include <qfiledialog.h>
#include <qsettings.h>
#include <qmessagebox.h>
#include <qregularexpression.h>


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

	ui.chooseAvatarBtn->setCursor(QCursor(Qt::PointingHandCursor));

	connect(ui.chooseAvatarBtn, SIGNAL(clicked()), this, SLOT(onChooseAvatarBtnClicked()));
	connect(ui.cancelBtn, SIGNAL(clicked()), this, SLOT(onCancelBtnClicked()));
	connect(ui.saveBtn, SIGNAL(clicked()), this, SLOT(onConfirmBtnClicked()));
}

void AddEditWindow::onCancelBtnClicked()
{
	this->close();
}

void AddEditWindow::onChooseAvatarBtnClicked() {
	QString lastPath = QSettings().value("lastPath").toString();
	QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), lastPath, tr("Image Files (*.png *.jpg *.bmp)"));
	if (fileName != "")
	{
		QPixmap avatarImg(fileName);
		QPixmap fitpixmap = avatarImg.scaled(ui.avatarLabel->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
		std::string timestamp = std::to_string(std::time(nullptr));
		std::string path = "Assets/Images/Crop/avatar_" + timestamp + ".jpg";
		fitpixmap.save(path.c_str());
		ui.avatarLabel->setStyleSheet(("background-image: url(" + QString(path.c_str()) + "); background-repeat: no-repeat; background-position: center center; border-radius: 45px;").toStdString().c_str());
	}
}

void AddEditWindow::onConfirmBtnClicked() {
	if (ui.tbName->text().isEmpty()) {
		QMessageBox::warning(this, "Warning", "姓名不能为空！");
		return;
	}
	if (ui.tbPhone->text().isEmpty()) {
		QMessageBox::warning(this, "Warning", "联系电话不能为空！");
		return;
	}
	else {
		QRegularExpression regex(R"((\d+))");
		QRegularExpressionMatch match = regex.match(ui.tbPhone->text());
		if (!match.hasMatch()) {
			QMessageBox::warning(this, "Warning", "联系电话格式不正确！");
			return;
		}
	}
	if (!ui.tbEmail->text().isEmpty()) {
		if (!isValidEmail(ui.tbEmail->text())) {
			QMessageBox::warning(this, "Warning", "邮箱格式不正确！");
			return;
		}
	}
}

bool AddEditWindow::isValidEmail(const QString& email) {
	QRegularExpression regex(R"((\w+)(\.|_)?(\w*)@(\w+)(\.(\w+))+)");
	QRegularExpressionMatch match = regex.match(email);
	return match.hasMatch();
}

AddEditWindow::~AddEditWindow()
{
}
