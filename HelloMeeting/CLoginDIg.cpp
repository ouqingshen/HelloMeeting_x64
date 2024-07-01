#include "CLoginDIg.h"
#include <qdebug.h>
#include <qt_windows.h>
#include <qmessagebox.h>
#include <QGraphicsDropShadowEffect>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <qdatetime.h>
#include <QCryptographicHash>

CLoginDIg::CLoginDIg(QWidget* parent)
	: QDialog(parent)
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL"); // 使用QMYSQL驱动
	db.setHostName("localhost"); // 设置MySQL服务器地址
	db.setDatabaseName("hellomeeting"); // 设置数据库名
	db.setUserName("root"); // 设置用户名
	db.setPassword("root"); // 设置密码

	if (!db.open()) {
		qDebug() << "Error connecting to database:" << db.lastError().text();
	}
	else {
		qDebug() << "Connected to database successfully";
	}

	setWindowIcon(QIcon(":/CLoginDIg/resources/login/logo.png"));
	setProperty("inheritStyle", false);
	setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
	ui.setupUi(this);
	ui.widget_right->setStyleSheet("background-image: url(:/CLoginDIg/resources/login/login_right.jpg)");

	ui.lineEdit_roomId->setText("1001");
	ui.lineEdit_pwd->setFixedHeight(40);
	ui.lineEdit_pwd->setText("1111111");
	ui.lineEdit_roomId->setFixedHeight(40);
	ui.lineEdit_userName->setFixedHeight(40);
	ui.lineEdit_userName->setText("1001");

	ui.btnJoin->setFixedHeight(50);

	ui.label_logo->setText("");
	QPixmap pixmap(":/CLoginDIg/resources/login/logo.png");
	pixmap = pixmap.scaled(40, 40, Qt::KeepAspectRatio, Qt::SmoothTransformation);  // 保持比例缩放
	ui.label_logo->setPixmap(pixmap);

	ui.btnMin->setText("");
	ui.btnClose->setText("");
	ui.btnMin->setFixedSize(32, 32);
	ui.btnClose->setFixedSize(32, 32);

	connect(ui.btnMin, &QPushButton::clicked, this, &CLoginDIg::on_titleBar_clicked);
	connect(ui.btnClose, &QPushButton::clicked, this, &CLoginDIg::on_titleBar_clicked);
	connect(ui.btnJoin, &QPushButton::clicked, this, &CLoginDIg::btnJoin_clicked);
	connect(ui.btn_register, &QPushButton::clicked, this, [&]() {
		m_pRegisterDlg = new CRegisterDlg(this);
		m_pRegisterDlg->show();
		});

	ui.btnSet->setCheckable(true);
	ui.btnSet->setChecked(true);

	//btnJoin_clicked();
}

CLoginDIg::~CLoginDIg()
{}

QString CLoginDIg::getRoomId() const
{
	return ui.lineEdit_roomId->text();
}

uint CLoginDIg::getUserName() const
{
	return ui.lineEdit_userName->text().toUInt();
}

void CLoginDIg::mousePressEvent(QMouseEvent* event)
{
	if (ReleaseCapture())
	{
		QWidget* pWindow = this->window();
		if (pWindow->isTopLevel())
		{
			SendMessage(HWND(pWindow->winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
		}
	}
}

void CLoginDIg::btnJoin_clicked() {
	QString roomId = ui.lineEdit_roomId->text();
	QString userName = ui.lineEdit_userName->text();
	QString password = ui.lineEdit_pwd->text();
	if (roomId.isEmpty() || userName.isEmpty() || password.isEmpty())
	{
		QMessageBox::warning(this, "警告", "房间号用户ID不能为空");
		return;
	}
	accept();
	return;
	//loginUser(userName, password);

	/*
	else if (loginUser(userName, password)) {
		// 登录成功，显示成功消息或执行其他操作
	   // QMessageBox::information(this, "成功", "登录成功");

		accept();
		return;
	}
	else {
		// 登录失败，显示错误消息
		QMessageBox::critical(this, "错误", "!!!!!!!!!!!");
		return;
	}
	*/
}

void CLoginDIg::on_titleBar_clicked() {
	QPushButton* p_button = qobject_cast<QPushButton*>(sender());
	QWidget* t_window = this->window();
	if (p_button == ui.btnMin)
	{
		t_window->showMinimized();
	}
	else if (p_button == ui.btnClose)
	{
		t_window->close();
	}
}

bool CLoginDIg::loginUser(const QString& userName, const QString& password)
{
	// 创建或获取数据库连接
	QSqlDatabase db = QSqlDatabase::database();
	if (!db.isOpen()) {
		// 处理数据库未打开的情况
		qDebug() << "Database is not open.";
		return false;
	}

	// 准备SQL查询语句
	QString sql = "SELECT password_hash, salt FROM users WHERE username = :username";
	QSqlQuery query;
	query.prepare(sql);
	query.bindValue(":username", userName);

	// 执行查询
	if (!query.exec()) {
		// 处理SQL执行失败的情况
		qDebug() << "Error executing query:" << query.lastError().text();
		return false;
	}
	// 检查查询结果
	if (query.next()) {
		const QByteArray storedHash = query.value(0).toByteArray();
		const QString salt111 = query.value(1).toString();
		const QByteArray salt = QByteArray::fromHex(salt111.toUtf8());

		// 使用相同的哈希算法和盐值对用户提供的密码进行哈希处理
		QByteArray passwordHash = QCryptographicHash::hash((password + salt).toUtf8(), QCryptographicHash::Sha256).toHex();
		// 比较存储的哈希值和计算的哈希值是否相同
		if (passwordHash == storedHash) {
			// 哈希值匹配，登录成功
			return true;
		}
		else {
			// 哈希值不匹配，登录失败
			return false;
		}
	}
	else {
		// 没有找到对应的用户名，登录失败
		return false;
	}
}