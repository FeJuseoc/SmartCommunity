#include "loginwindow.h"
#include "ui_loginwindow.h"
#include <QMessageBox>
#include <QSqlQuery>
#include <QFile>
#include <QTextStream>
#include <QTimer>
#include <QMouseEvent>
#include <QIcon>

loginwindow::loginwindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::loginwindow)
{
    ui->setupUi(this);

    // 设置窗口为无边框
    this->setWindowFlags(Qt::FramelessWindowHint);
    this->setWindowTitle("智慧小区系统 - 登录");

    // 初始化UI元素
    usernameEdit = ui->lineEditUsername;
    passwordEdit = ui->lineEditpassword;
    ui->lineEditpassword->setEchoMode(QLineEdit::Password);

    // 主题切换按钮连接
    connect(ui->btn_1, SIGNAL(clicked(bool)), this, SLOT(set_style()));
    connect(ui->btn_2, SIGNAL(clicked(bool)), this, SLOT(set_style()));
    connect(ui->btn_3, SIGNAL(clicked(bool)), this, SLOT(set_style()));
    connect(ui->btn_4, SIGNAL(clicked(bool)), this, SLOT(set_style()));

    // 连接菜单按钮点击事件
    connect(ui->pushButtonMenu, &QPushButton::clicked, this, [this]() {
        bool isVisible = ui->btn_1->isVisible();
        ui->btn_1->setVisible(!isVisible);
        ui->btn_2->setVisible(!isVisible);
        ui->btn_3->setVisible(!isVisible);
        ui->btn_4->setVisible(!isVisible);
    });

    // 初始化数据库
    if (!db.initDatabase()) {
        QMessageBox::critical(this, "数据库错误", "无法连接到数据库，请检查文件是否存在！");
        this->close();
    }

    // 连接按钮信号
    connect(ui->exitBtn, &QPushButton::clicked, this, &loginwindow::on_exitBtn_clicked);
    connect(ui->loginBtn, &QPushButton::clicked, this, &loginwindow::on_loginBtn_clicked);

    // 初始化窗口拖动相关
    m_dragging = false;

    // 默认隐藏主题按钮
    ui->btn_1->setVisible(false);
    ui->btn_2->setVisible(false);
    ui->btn_3->setVisible(false);
    ui->btn_4->setVisible(false);

    // 设置图标
    ui->pushButtonLogo->setIcon(QIcon(":/image/screen.png"));
    ui->pushButtonLogo->setIconSize(QSize(32, 32));
    ui->pushButtonMenu->setIcon(QIcon(":/image/menu-hover.png"));
    ui->pushButtonMenu->setIconSize(QSize(32, 32));
    ui->pushButtonMinimize->setIcon(QIcon(":/image/minimize-hover.png"));
    ui->pushButtonMinimize->setIconSize(QSize(32, 32));
    ui->pushButtonClose->setIcon(QIcon(":/image/close-hover.png"));
    ui->pushButtonClose->setIconSize(QSize(32, 32));

    // 默认应用第一个主题
    QTimer::singleShot(0, this, [this]() {
        ui->btn_1->click();
    });
}

void loginwindow::set_style()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    QString filePath;

    if("btn_1" == btn->objectName()) {
        filePath = ":/res/qss/style-1.qss";
        currentStyleSheetIndex = 1;
    } else if("btn_2" == btn->objectName()) {
        filePath = ":/res/qss/style-2.qss";
        currentStyleSheetIndex = 2;
    } else if("btn_3" == btn->objectName()) {
        filePath = ":/res/qss/style-3.qss";
        currentStyleSheetIndex = 3;
    } else if("btn_4" == btn->objectName()) {
        filePath = ":/res/qss/style-4.qss";
        currentStyleSheetIndex = 4;
    }

    QFile file(filePath);
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    this->setStyleSheet(stylesheet);
    file.close();

    ui->loginBtn->setStyleSheet("background-color: #007BFF; color: white; border: none; padding: 10px 20px;");
    ui->exitBtn->setStyleSheet("background-color: #DC3545; color: white; border: none; padding: 10px 20px;");

    emit styleSheetChanged(stylesheet);
}

loginwindow::~loginwindow()
{
    delete ui;
}

void loginwindow::on_loginBtn_clicked()
{
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();

    if (username.isEmpty() || password.isEmpty()) {
        QMessageBox::warning(this, "输入错误", "用户名或密码不能为空！");
        return;
    }

    database::LoginResult result = db.validateLogin(username, password);

    if (result.success) {
        if (result.role == "物业管理人员") {
            emit showAdminMain();
        } else if (result.role == "物业工作人员") {
            emit showStaffMain();
        } else if (result.role == "业主") {
            emit showOwnerMain();
        } else {
            QMessageBox::warning(this, "角色错误", "未知用户角色，请联系管理员！");
            return;
        }

        QSqlQuery query;
        query.prepare("UPDATE users SET background=? WHERE account=? AND password=?");
        query.addBindValue(currentSheet());
        query.addBindValue(username);
        query.addBindValue(password);
        if(query.exec()) {
            this->hide();
        }
    } else {
        QMessageBox::warning(this, "登录失败", result.errorMessage);
    }
}

void loginwindow::on_exitBtn_clicked()
{
    this->close();
}

void loginwindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPos() - this->frameGeometry().topLeft();
        event->accept();
    }
}

void loginwindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        this->move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void loginwindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        event->accept();
    }
}

void loginwindow::on_pushButtonClose_clicked()
{
    close();
}

void loginwindow::on_pushButtonMinimize_clicked()
{
    setWindowState(Qt::WindowMinimized);
}

int loginwindow::currentSheet(){
   return currentStyleSheetIndex;
}

int loginwindow::getCurrentStyleSheetIndex() const {
    return currentStyleSheetIndex;
}

void loginwindow::getAccountAndPassword()
{
    account = usernameEdit->text().trimmed();
    password = passwordEdit->text();
}

void loginwindow::managerdryon(Manager& manager)
{
    getAccountAndPassword();
    manager.setAccountAndPassword(account, password);
    manager.setStyleSheetIndex(getCurrentStyleSheetIndex());
}

void loginwindow::workerdryon(Worker& worker)
{
    getAccountAndPassword();
    worker.setAccountAndPassword(account, password);
    worker.setStyleSheetIndex(getCurrentStyleSheetIndex());
}

void loginwindow::ownerdryon(Owner& owner)
{
    getAccountAndPassword();
    owner.setAccountAndPassword(account, password);
    owner.setStyleSheetIndex(getCurrentStyleSheetIndex());
}

void loginwindow::tryon(Owner& owner)
{
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE account=? AND password=?");
    query.addBindValue(username);
    query.addBindValue(password);

    if(query.exec() && query.next()) {
        owner.tryin(query.value("name").toString(), query.value("phone").toString(), query.value("address").toString());
        getAccountAndPassword();
        owner.setAccountAndPassword(account, password);
    }
}

void loginwindow::show_repair(Worker& worker)
{
    worker.show_repair_table();
}

void loginwindow::showWorker(Worker& worker)
{
    QString username = usernameEdit->text().trimmed();
    QString password = passwordEdit->text();
    QSqlQuery query;
    query.prepare("SELECT * FROM users WHERE account=? AND password=?");
    query.addBindValue(username);
    query.addBindValue(password);

    if(query.exec() && query.next()) {
        worker.showWorkerMassage(query.value("name").toString(), query.value("phone").toString(), query.value("address").toString());
        getAccountAndPassword();
        worker.setAccountAndPassword(account, password);
    }
}
