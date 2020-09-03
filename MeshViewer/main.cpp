//===============================================================
// Author       : Wei Li (lw15@mail.ustc.edu.cn)
// Created Date : 2017/Jan/10 13:32:51
// Last Change  : 2017/Jan/10 13:32:54
// Summary      :
//===============================================================

#include <QtGlobal>
#include <QtWidgets/QApplication>
#include <QtWidgets/QtWidgets>

#include "MainWindow.h"

int main(int argc, char* argv[])
{
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);
    QApplication a(argc, argv);

    QT_REQUIRE_VERSION(argc, argv, "5.6.0");
    QApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);  //解决Qt在Retina屏幕上图片模糊问题
    int ratio = QApplication::desktop()->screen()->devicePixelRatio();
    // std::cout << ratio << std::endl;

    MainWindow w;
    w.show();
    return a.exec();
}
