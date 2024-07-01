#include "CRegisterDlg.h"
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QCryptographicHash>
#include <QByteArray>
#include <QRandomGenerator>
#include <qdebug.h>
#include <QMessageBox>

CRegisterDlg::CRegisterDlg(QDialog* parent)
	:QDialog(parent),
	ui(new Ui::Dialog)

{
	ui->setupUi(this);
	connect(ui->okButton, &QPushButton::clicked, this, [&]() {
		btn_register_ok();
		});
}

CRegisterDlg::~CRegisterDlg()
{
}

void CRegisterDlg::btn_register_ok() {
	QString userName = ui->lineEdit_userName->text();
	QString password = ui->lineEdit_passWord->text();
	if (userName.isEmpty() || password.isEmpty())
	{
		QMessageBox::warning(this, u8"警告", u8"房间号用户ID不能为空");
		return;
	}
	if (!registerUser(userName, password)) {
		QMessageBox::critical(this, "Error", u8"注册失败");
		return;
	}
	else {
		QMessageBox::information(this, "Success", u8"注册成功");
		close();
	}
}

bool CRegisterDlg::registerUser(const QString& userName, const QString& password)
{
	QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL");
	db.setHostName("localhost");
	db.setDatabaseName("hellomeeting");
	db.setUserName("root");
	db.setPassword("root");

	if (!db.open()) {
		qDebug() << "Error connecting to database:" << db.lastError().text();
		return false;
	}

	QByteArray salt = QCryptographicHash::hash(QByteArray::number(QRandomGenerator::global()->bounded(1000000000)), QCryptographicHash::Sha256);
	qDebug() << salt << endl;
	QByteArray passwordHash = QCryptographicHash::hash((password + salt).toUtf8(), QCryptographicHash::Sha256)/*.toHex()*/;
	qDebug() << passwordHash;

	QString sql = "INSERT INTO users (username, salt ,password_hash) VALUES (:username,:salt, :password_hash )";
	QSqlQuery query;

	query.prepare(sql);
	query.bindValue(":username", userName);
	query.bindValue(":salt", salt.toHex());
	query.bindValue(":password_hash", passwordHash.toHex());

	if (!query.exec()) {
		qDebug() << "Error inserting user:" << query.lastError().text();
		db.close();
		return false;
	}

	db.close();

	return true;
}