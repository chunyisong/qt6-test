#include "mywindow.h"
#include <QApplication>

/// 初始化静态库中的资源,qrc文件名称要符合c++变量命名规范,该函数不能在命名空间中,必须在全局中定义该函数执行Q_INIT_RESOURCE
inline void initResources()
{
#ifndef APP_LIB_EXPORT // 静态库时未定义宏导出,则注册资源文件
    Q_INIT_RESOURCE(mywindow);
    Q_INIT_RESOURCE(mywindow2);
#endif
}

int main(int argc, char* argv[])
{
    QApplication a(argc, argv);

    initResources();

    MyWindow m;
    m.show();
    return a.exec();
}