#ifndef BASEWINDOW_H
#define BASEWINDOW_H

#include <QWidget>
#include <QLineEdit>
#include <QPushButton>
#include <QMouseEvent>
#include <QIcon>
#include <QPalette>
#include <QPixmap>
#include <QShowEvent>
#include <QTimer>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>

class BaseWindow : public QWidget
{
    Q_OBJECT

public:
    explicit BaseWindow(QWidget *parent = nullptr);
    virtual ~BaseWindow();

    void setAccountAndPassword(const QString& account, const QString& password);
    void setStyleSheetIndex(int index);
    void setButtonColors(); // 添加新的方法声明

public slots:
    void set_style();

signals:
    void styleSheetChanged(const QString& stylesheet);
    void backToLogin();

protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;
    void showEvent(QShowEvent *event) override;

private:
    QString currentaccount;
    QString currentpassword;
    int currentStyleSheetIndex;
    QPoint m_dragPosition;
    bool m_dragging;
};

#endif // BASEWINDOW_H
