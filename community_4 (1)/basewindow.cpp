#include "BaseWindow.h"
// 添加 QLabel 头文件
#include <QLabel>
#include <QTableView>
#include <QHeaderView>

BaseWindow::BaseWindow(QWidget *parent) : QWidget(parent), currentStyleSheetIndex(1)
{
    // 设置窗口为无边框
    this->setWindowFlags(Qt::FramelessWindowHint);

    // 主题切换按钮连接，这里假设派生类会处理按钮点击信号
    // 连接 pushButtonMenu 点击事件，这里假设派生类会处理按钮点击信号


    // 在构造函数末尾添加
    QTimer::singleShot(0, this, [this]() {
        // 这里假设派生类会处理按钮点击，因为基类不知道按钮对象名
    });

    m_dragging = false;

    // 默认隐藏 btn_1, btn_2, btn_3, btn_4 和 pushButtonBack，这里假设派生类会处理按钮显示隐藏
}

BaseWindow::~BaseWindow()
{
    // 基类析构函数，不需要额外操作
}

void BaseWindow::setAccountAndPassword(const QString& account, const QString& password)
{
    currentaccount = account;
    currentpassword = password;
}

void BaseWindow::setStyleSheetIndex(int index)
{
    currentStyleSheetIndex = index;
}

void BaseWindow::set_style()
{
    QPushButton* btn = qobject_cast<QPushButton*>(sender());
    QString imagePath;
    if("btn_1" == btn->objectName())        // 对应 background-1.png
    {
        imagePath = "D:/qt/community_4/res/pic/background-1.png";
        currentStyleSheetIndex = 1;
    }
    else if("btn_2" == btn->objectName())  // 对应 background-2.png
    {
        imagePath = "D:/qt/community_4/res/pic/background-2.png";
        currentStyleSheetIndex = 2;
    }
    else if("btn_3" == btn->objectName())  // 对应 background-3.png
    {
        imagePath = "D:/qt/community_4/res/pic/background-3.png";
        currentStyleSheetIndex = 3;
    }
    else if("btn_4" == btn->objectName())  // 对应 background-4.png
    {
        imagePath = "D:/qt/community_4/res/pic/background-4.png";
        currentStyleSheetIndex = 4;
    }

    QPixmap pixmap(imagePath);
    if (pixmap.isNull()) {
        qDebug() << "无法加载背景图片: " << imagePath;
        QMessageBox::warning(this, "错误", "无法加载背景图片，请检查文件路径！");
        return;
    }

    QPalette palette = this->palette();
    palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
    this->setPalette(palette);

    // 调用设置按钮颜色的方法
    setButtonColors();

    emit styleSheetChanged("");
}

void BaseWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = true;
        m_dragPosition = event->globalPos() - this->frameGeometry().topLeft();
        event->accept();
    }
}

void BaseWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_dragging && (event->buttons() & Qt::LeftButton)) {
        this->move(event->globalPos() - m_dragPosition);
        event->accept();
    }
}

void BaseWindow::mouseReleaseEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
        m_dragging = false;
        event->accept();
    }
}

void BaseWindow::showEvent(QShowEvent *event)
{
    // 根据 currentStyleSheetIndex 设置背景图片
    QString imagePath;
    switch (currentStyleSheetIndex) {
    case 1:
        imagePath = "D:/qt/community_4/res/pic/background-1.png";
        break;
    case 2:
        imagePath = "D:/qt/community_4/res/pic/background-2.png";
        break;
    case 3:
        imagePath = "D:/qt/community_4/res/pic/background-3.png";
        break;
    case 4:
        imagePath = "D:/qt/community_4/res/pic/background-4.png";
        break;
    default:
        imagePath = "D:/qt/community_4/res/pic/background-1.png";
        break;
    }

    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        QPalette palette = this->palette();
        palette.setBrush(QPalette::Window, QBrush(pixmap.scaled(this->size(), Qt::IgnoreAspectRatio, Qt::SmoothTransformation)));
        this->setPalette(palette);
    }

    // 设置按钮文字样式
    QList<QPushButton*> buttons = this->findChildren<QPushButton*>();
    foreach (QPushButton* button, buttons) {
        QString objectName = button->objectName();
        if (objectName != "pushButtonLogo" && objectName != "btn_1" && objectName != "btn_2" &&
            objectName != "btn_3" && objectName != "btn_4" && objectName != "pushButtonMenu" &&
            objectName != "pushButtonBack" && objectName != "pushButtonMinimize" && objectName != "pushButtonClose") {
            // 根据按钮高度调整字体大小
            double fontSize = button->height() / 1.2; // 可根据实际情况调整比例
            QString fontStyle = QString("font-family: 'Microsoft YaHei'; font-weight: bold; font-size: %1px;").arg(fontSize);
            button->setStyleSheet(fontStyle);
        }
    }

    // 设置标签文字字体为微软雅黑
    QList<QLabel*> labels = this->findChildren<QLabel*>();
    foreach (QLabel* label, labels) {
        QFont font("Microsoft YaHei", label->font().pointSize());
        label->setFont(font);
    }

    // 调用设置按钮颜色的方法
    setButtonColors();
    // 设置表格视图的全局样式
    QList<QTableView*> tableViews = this->findChildren<QTableView*>();
        foreach (QTableView* tableView, tableViews) {
            // 设置表头样式
            tableView->horizontalHeader()->setStyleSheet(
                "QHeaderView::section {"
                "    background-color: #4A708B;"  // 深蓝色背景
                "    color: white;"              // 白色文字
                "    padding: 4px;"              // 内边距
                "    border: 5px solid #3a5f7e;" // 边框
                "    font-weight: bold;"         // 粗体
                "    font-size: 14px;"           // 字体大小
                "}"
            );

            // 设置表格主体样式
            tableView->setStyleSheet(
                "QTableView {"
                "    background-color: white;"   // 白色背景
                "    alternate-background-color: #F0F7FF;" // 交替行颜色
                "    gridline-color: #D0D0D0;"   // 网格线颜色
                "    font-size: 14px;"           // 字体大小
                "}"
                "QTableView::item {"
                "    padding: 3px;"              // 单元格内边距
                "    border: none;"              // 无边框
                "}"
                "QTableView::item:selected {"
                "    background-color: #B0C4DE;" // 选中项背景色
                "    color: black;"              // 选中项文字颜色
                "}"
            );

            // 设置交替行颜色
            tableView->setAlternatingRowColors(true);

            // 设置选择行为（整行选择）
            tableView->setSelectionBehavior(QAbstractItemView::SelectRows);

            // 设置网格线显示
            tableView->setShowGrid(true);

            // 设置列宽自适应内容
            tableView->resizeColumnsToContents();

            // 设置行高
            tableView->verticalHeader()->setDefaultSectionSize(25);

            // 设置水平表头的拉伸模式，让表格根据 QTableView 大小自动调整
            tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);

            // 自动调整行高以适应内容
            tableView->resizeRowsToContents();
        }

        QWidget::showEvent(event);
    }

void BaseWindow::setButtonColors()
{
    QList<QPushButton*> buttons = this->findChildren<QPushButton*>();
    QString color;

    // 根据当前背景图片索引设置按钮颜色（使用更浅的色调）
    switch (currentStyleSheetIndex) {
    case 1:
        color = "#FF69B4"; // 浅红色（比#FFCCCC更浅）
        break;
    case 2:
        color = "#FFD700"; // 浅蓝色（比#CCE5FF更浅）
        break;
    case 3:
        color = "#9370DB"; // 浅绿色（比#CCFFCC更浅）
        break;
    case 4:
        color = "#2E8B57"; // 浅米色（替代浅黄色，更柔和）
        break;
    default:
        color = "#FF69B4"; // 默认浅红色
        break;
    }

    foreach (QPushButton* button, buttons) {
        QString objectName = button->objectName();
        if (objectName != "pushButtonLogo" && objectName != "btn_1" && objectName != "btn_2" &&
            objectName != "btn_3" && objectName != "btn_4" && objectName != "pushButtonMenu" &&
            objectName != "pushButtonBack" && objectName != "pushButtonMinimize" && objectName != "pushButtonClose") {
            QString style = QString("background-color: %1;").arg(color);
            button->setStyleSheet(button->styleSheet() + style);
        }
    }
}





