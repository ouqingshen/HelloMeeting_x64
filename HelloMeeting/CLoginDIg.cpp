#include "CLoginDIg.h"
#include <qdebug.h>
#include <qt_windows.h>
#include <qmessagebox.h>
#include <QGraphicsDropShadowEffect>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <qdatetime.h>
CLoginDIg::CLoginDIg(QWidget *parent)
    : QDialog(parent)
{
    initMySQL();

    setWindowIcon(QIcon(":/CLoginDIg/resources/login/logo.png"));
    setProperty("inheritStyle", false);
    setWindowFlags(Qt::FramelessWindowHint | Qt::WindowMinMaxButtonsHint);
    ui.setupUi(this);
    ui.widget_right->setStyleSheet("background-image: url(:/CLoginDIg/resources/login/login_right.jpg)");
 
    ui.lineEdit_roomId->setText("1001");
    ui.lineEdit_pwd->setFixedHeight(40);
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
    ui.btnMin->setFixedSize(32,32);
    ui.btnClose->setFixedSize(32,32);


    connect(ui.btnMin,&QPushButton::clicked,this,&CLoginDIg::on_titleBar_clicked);
    connect(ui.btnClose,&QPushButton::clicked,this,&CLoginDIg::on_titleBar_clicked);
    connect(ui.btnJoin,&QPushButton::clicked,this,&CLoginDIg::btnJoin_clicked);

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

void CLoginDIg::initMySQL()
{
   static QSqlDatabase db = QSqlDatabase::addDatabase("QMYSQL"); // 使用QMYSQL驱动  
    db.setHostName("localhost"); // 设置MySQL服务器地址  
    db.setDatabaseName("blog"); // 设置数据库名  
    db.setUserName("root"); // 设置用户名  
    db.setPassword("root"); // 设置密码  

    if (!db.open()) {
        qDebug() << "Error connecting to database:" << db.lastError().text();
    }
    else {
        qDebug() << "Connected to database successfully";
        QSqlQuery query;
        query.exec("SELECT * FROM b_article");
        while (query.next()) {
            qDebug() << "ID:" << query.value(0).toLongLong();
            qDebug() << "User ID:" << query.value(1).toString();
            qDebug() << "Category ID:" << query.value(2).toLongLong();
            qDebug() << "Title:" << query.value(3).toString();
            qDebug() << "Avatar:" << query.value(4).toString();
            qDebug() << "Summary:" << query.value(5).toString();
            qDebug() << "Content:" << query.value(6).toString();
            qDebug() << "Content (MD):" << query.value(7).toString();
            qDebug() << "Read Type:" << query.value(8).toInt();
            qDebug() << "Is Stick:" << query.value(9).toInt();
            qDebug() << "Is Publish:" << query.value(10).toInt();
            qDebug() << "Is Original:" << query.value(11).toInt();
            qDebug() << "Original URL:" << query.value(12).toString();
            qDebug() << "Quantity:" << query.value(13).toLongLong();
            qDebug() << "Create Time:" << query.value(14).toDateTime();
            qDebug() << "Update Time:" << query.value(15).toDateTime();
            qDebug() << "Is Carousel:" << query.value(16).toInt();
            qDebug() << "Is Recommend:" << query.value(17).toInt();
            qDebug() << "Keywords:" << query.value(18).toString();
        }
        db.close();
    }

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
    uint userName = ui.lineEdit_userName->text().toUInt();
    if (roomId.isEmpty()||userName==NULL)
    {
       // QMessageBox::warning(this, u8"警告", u8"房间号用户ID不能为空！！！！！");
       // return;
    }
    
    accept();
}

void CLoginDIg::on_titleBar_clicked() {
    QPushButton* p_button = qobject_cast<QPushButton*>(sender());
    QWidget* t_window = this->window();
    if (p_button==ui.btnMin)
    {
        t_window->showMinimized();
    }
    else if (p_button==ui.btnClose)
    {
        t_window->close();
    }
  
}