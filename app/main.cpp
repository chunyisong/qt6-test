#include "mywindow.h"

#include <QApplication>
#include <QDebug>
#include <QFontDatabase>
#include <QDialog>
#include <QMessageBox>
#include <QPushButton>
#include <QVBoxLayout>

// 设置全局字体 黑体，用于解决Qt for WebAssembly中文显示异常
void initGlobalFont(QApplication& mainApp)
{
    QString fontName = ":/app/res/simhei.ttf"; // rcc 文件路径
#ifdef Q_OS_WASM
    fontName = "/res/simhei.ttf";  // 内存文件系统路径
    int fontId = QFontDatabase::addApplicationFont(fontName);
    QStringList fontFamilies = QFontDatabase::applicationFontFamilies(fontId);
    qDebug()<< __FUNCTION__ << __LINE__  << " 设置全局字体(" << fontName << "):" << fontFamilies;
    if (fontFamilies.size() > 0)
    {
        QFont font;
        font.setFamily(fontFamilies[0]); //设置全局字体
        mainApp.setFont(font);
    }
#else
    qDebug()<< __FUNCTION__ << __LINE__  << u8" 未设置宏:Q_OS_WASM,无须设置全局字体(" << fontName << ")";
#endif //Q_OS_WASM
}

/// 初始化静态库中的资源,qrc文件名称要符合c++变量命名规范,该函数不能在命名空间中,必须在全局中定义该函数执行Q_INIT_RESOURCE
inline void initResources()
{
#ifndef APP_LIB_IMPORTS // 静态库时未定义宏导出,则注册资源文件
    Q_INIT_RESOURCE(mywindow);
    Q_INIT_RESOURCE(mywindow2);
    qDebug()<< __FUNCTION__ << __LINE__ << "APP_LIB_EXPORT not defined:STATIC";
#else
    qDebug()<< __FUNCTION__ << __LINE__ << "APP_LIB_EXPORT defined:Shared";
#endif
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    initResources();
    initGlobalFont(a);

    auto win = new QWidget;
    auto showDlg = [win](QPushButton* parent, bool isModal = false) {
        auto gw = MyWindow::createGraphicsWindow(1E8);
        auto vLayout = new QVBoxLayout;
        vLayout->addWidget(gw);
        auto dp = (QWidget*)parent;
        // dp = parent->topLevelWidget();
        // dp = win;
        // dp = nullptr;
        auto dlg = new QDialog(dp);
        dlg->setAttribute(Qt::WA_DeleteOnClose, true);
        dlg->setWindowFlags(dlg->windowFlags() | Qt::WindowStaysOnTopHint);
        dlg->setModal(isModal);
        dlg->setLayout(vLayout);
        dlg->show();
        qDebug() << __FUNCTION__ << "#" << __LINE__ << " showDlg>createGraphicsWindow,isModal:" << isModal << ",dlg.parent:" << dlg->parent();
    };
    auto btnDlg = new QPushButton("弹出对话框非模态");
    btnDlg->setToolTip(QString("tooltip:%1").arg(btnDlg->text()));
    auto btnModal = new QPushButton("弹出对话框模态");
    btnModal->setToolTip(QString("tooltip:%1").arg(btnModal->text()));
    auto btnMsg = new QPushButton("弹出消息框");
    btnMsg->setToolTip(QString("tooltip:%1").arg(btnMsg->text()));
    QObject::connect(btnDlg, &QPushButton::clicked, btnDlg, [btnDlg, showDlg](bool checked) {
        showDlg(btnDlg, false);
    });
    QObject::connect(btnModal, &QPushButton::clicked, btnModal, [btnModal, showDlg](bool checked) {
        showDlg(btnModal, true);
    });
    QObject::connect(btnMsg, &QPushButton::clicked, btnMsg, [btnMsg](bool checked) {
        QString str;
        for (int j = 0, n = 10; j < n; ++j) {
            str.append(u8"这是一个警告消息djfidjgf \n");
        }
        auto msgbox = new QMessageBox(QMessageBox::Warning, u8"警告消息", str, QMessageBox::Yes | QMessageBox::No,btnMsg);
        msgbox->setAttribute(Qt::WA_DeleteOnClose, true);
        msgbox->show();
        qDebug() << __FUNCTION__ << "#" << __LINE__ << u8" showMsgbox btn.clicked:" << btnMsg->text();
    });
    auto vLayout = new QVBoxLayout;
    vLayout->addWidget(btnDlg);
    vLayout->addWidget(btnModal);
    vLayout->addWidget(btnMsg);
    vLayout->addWidget(new MyWindow);
    win->setLayout(vLayout);
    win->show();
    return a.exec();
}
