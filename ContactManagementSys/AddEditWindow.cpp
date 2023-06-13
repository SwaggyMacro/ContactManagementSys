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
#include "Contact.h"


AddEditWindow::AddEditWindow(QWidget *parent, int type, QString phone)
	: QMainWindow(parent)
{
	this->type = type;
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

	ui.saveBtn->setFocusPolicy(Qt::NoFocus);
	ui.cancelBtn->setFocusPolicy(Qt::NoFocus);

	ui.avatarLabel->setCursor(QCursor(Qt::PointingHandCursor));

	ui.chooseAvatarBtn->setCursor(QCursor(Qt::PointingHandCursor));

	connect(ui.chooseAvatarBtn, SIGNAL(clicked()), this, SLOT(onChooseAvatarBtnClicked()));
	connect(ui.cancelBtn, SIGNAL(clicked()), this, SLOT(onCancelBtnClicked()));
	connect(ui.saveBtn, SIGNAL(clicked()), this, SLOT(onConfirmBtnClicked()));

	qDebug() << "type: " << type;
	qDebug() << "phone: " << phone;

	this->setWindowTitle("添加联系人");

	if (type == 1) {
		// edit
		ContactList contactList;
		contactList.loadFromFile("data.json");
		Contact* contact = contactList.getContactByPhone(phone.toStdString());
		if (contact != nullptr) {
			if (!contact->avatar.empty())
				ui.avatarLabel->setStyleSheet(("background-image: url(" + QString(contact->avatar.c_str()) + "); background-repeat: no-repeat; background-position: center center; border-radius: 45px;").toStdString().c_str());
			if (!contact->phone.empty())
				ui.tbPhone->setText(contact->phone.c_str());
			if (!contact->email.empty())
				ui.tbEmail->setText(contact->email.c_str());
			if (!contact->address.empty())
				ui.tbAddress->setText(contact->address.c_str());
			if (!contact->company.empty())
				ui.tbComName->setText(contact->company.c_str());
			if (!contact->position.empty())
				ui.tbComPos->setText(contact->position.c_str());
			if (!contact->name.empty()) {
				ui.tbName->setText(contact->name.c_str());
				this->setWindowTitle("编辑 " + QString::fromStdString(contact->name));
			}
		}
	}
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
	Contact contact;
	// QString to std::string
	contact.name = ui.tbName->text().toStdString();
	contact.phone = ui.tbPhone->text().toStdString();
	contact.company = ui.tbComName->text().isEmpty() ? "" : ui.tbComName->text().toStdString();
	contact.email = ui.tbEmail->text().isEmpty() ? "" : ui.tbEmail->text().toStdString();
	contact.address = ui.tbAddress->text().isEmpty() ? "" : ui.tbAddress->text().toStdString();
	contact.position = ui.tbComPos->text().isEmpty() ? "" : ui.tbComPos->text().toStdString();
	contact.avatar = ui.avatarLabel->styleSheet().split("url(")[1].split(")")[0].toStdString();
	contact.next = nullptr;
	ContactList contactList;
	contactList.loadFromFile("data.json");
	
	if (type == 0){
		if (contactList.isContactExists(contact.phone)) {
			QMessageBox::warning(this, "Warning", "联系人(电话)已存在！");
			return;
		}
		contactList.addContact(&contact);
	}
	else {
		contactList.editContact(&contact);
	}

	if (contactList.saveToFile("data.json")) {
		QMessageBox::information(this, "Success", "保存成功！");
		this->close();
	}
	else {
		QMessageBox::warning(this, "Warning", "保存失败！");
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
