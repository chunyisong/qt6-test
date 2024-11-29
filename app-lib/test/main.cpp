#include "mywindow.h"

#include <QApplication>
#include <QDebug>
#include <QFontDatabase>

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

/// 初始化静态库中的资源,<qrc文件名>称要符合c++变量命名规范,该函数不能在命名空间中,必须在全局中定义该函数执行Q_INIT_RESOURCE
inline void initResources()
{
#ifndef APP_LIB_IMPORTS // 静态库时未定义宏导出,则以<qrc文件名>注册资源文件
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

    MyWindow win;
    win.show();

    return a.exec();
}
