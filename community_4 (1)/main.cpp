// Community/main.cpp
#include "mainwindow.h"
#include <QApplication>
#include "loginwindow.h"
#include "worker.h" // 假设存在这些窗口类头文件
#include "manager.h"
#include "owner.h"
#include <QFile>
#include <QTextStream>
#include <QIcon>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    loginwindow l;

    QFile file(":/res/qss/style-1.qss"); // QSS文件路径
    file.open(QFile::ReadOnly);
    QTextStream filetext(&file);
    QString stylesheet = filetext.readAll();
    a.setStyleSheet(stylesheet); // 修复：改为 a.setStyleSheet
    file.close();

    // 设置程序图标
    a.setWindowIcon(QIcon(":/res/pic/screen.png"));

    Manager adminWindow;
    Worker staffWindow;
    Owner ownerWindow;

    // 连接登录窗口信号到各主窗口显示
    QObject::connect(&l, &loginwindow::showAdminMain, &adminWindow, &Manager::show);
    QObject::connect(&l, &loginwindow::showStaffMain, &staffWindow, &Worker::show);
    QObject::connect(&l, &loginwindow::showOwnerMain, [ &l,&ownerWindow](){l.ownerdryon(ownerWindow);ownerWindow.show();});
    QObject::connect(&l, &loginwindow::showAdminMain, [ &l,&adminWindow](){l.managerdryon(adminWindow);adminWindow.show();});
    QObject::connect(&l, &loginwindow::showStaffMain, [ &l,&staffWindow](){l.workerdryon(staffWindow);staffWindow.show();l.show_repair(staffWindow);l.showWorker(staffWindow);});

    // 连接样式表改变信号
    QObject::connect(&l, &loginwindow::styleSheetChanged, &adminWindow, [&adminWindow](const QString& stylesheet) {
        adminWindow.setStyleSheet(stylesheet);
    });
    QObject::connect(&l, &loginwindow::styleSheetChanged, &staffWindow, [&staffWindow](const QString& stylesheet) {
        staffWindow.setStyleSheet(stylesheet);
    });
    QObject::connect(&l, &loginwindow::styleSheetChanged, &ownerWindow, [&ownerWindow](const QString& stylesheet) {
        ownerWindow.setStyleSheet(stylesheet);
    });

    // 连接返回登录界面的信号
    QObject::connect(&ownerWindow, &Owner::backToLogin, &l, &loginwindow::show);
    QObject::connect(&adminWindow, &Manager::backToLogin, &l, &loginwindow::show);
    QObject::connect(&staffWindow, &Worker::backToLogin, &l, &loginwindow::show);

    l.show();

    return a.exec();
}
