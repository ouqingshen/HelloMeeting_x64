#pragma once
#include <qdialog.h>
#include <ui_CRegisterDlg.h>
class CRegisterDlg :
    public QDialog
{
    Q_OBJECT



public:
    CRegisterDlg(QDialog* parent=nullptr);
    ~CRegisterDlg();
private:
    void btn_register_ok();
    bool registerUser(const QString& userName, const QString& password);

public slots:

private:
    Ui::Dialog* ui;

};

