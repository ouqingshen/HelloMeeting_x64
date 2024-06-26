#pragma once
#include "CRegisterDlg.h"
#include <QtWidgets/QDialog>
#include "ui_CLoginDIg.h"
#include "CRegisterDlg.h"
class CLoginDIg : public QDialog
{
	Q_OBJECT

public:
	CLoginDIg(QWidget* parent = nullptr);
	~CLoginDIg();

	QString getRoomId() const;
	uint getUserName() const;

	void mousePressEvent(QMouseEvent* event) override;
	bool loginUser(const QString& userName, const QString& password);

public slots:
	void on_titleBar_clicked();
	void btnJoin_clicked();
	//void btnregister_clicked();

private:
	Ui::CLoginDIgClass ui;
	CRegisterDlg* m_pRegisterDlg;
};
