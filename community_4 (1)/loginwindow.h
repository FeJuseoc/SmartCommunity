#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include "database.h"
#include "manager.h"
#include "worker.h"
#include "owner.h"

namespace Ui {
class loginwindow;
}

class loginwindow : public QDialog
{
    Q_OBJECT

public:
    explicit loginwindow(QWidget *parent = nullptr);
    ~loginwindow();
    int currentSheet();
    void getAccountAndPassword();
    int getCurrentStyleSheetIndex() const;
    void setStyleSheetIndex(int index);

public slots:
    void set_style();
    void managerdryon(Manager& manager);
    void ownerdryon(Owner& owner);
    void workerdryon(Worker& worker);
    void tryon(Owner& owner);
    void show_repair(Worker& worker);
    void showWorker(Worker& worker);

signals:
    void showAdminMain();
    void showStaffMain();
    void showOwnerMain();
    void styleSheetChanged(const QString& stylesheet);

private slots:
    void on_loginBtn_clicked();
    void on_exitBtn_clicked();
    void on_pushButtonClose_clicked();
    void on_pushButtonMinimize_clicked();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    Ui::loginwindow *ui;
    database db;
    QLineEdit* usernameEdit;
    QLineEdit* passwordEdit;
    QPoint m_dragPosition;
    bool m_dragging;
    QString account;
    QString password;
    int currentStyleSheetIndex = 1;
};

#endif // LOGINWINDOW_H
