#include "ContactManagementSys.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ContactManagementSys w;
    w.show();
    return a.exec();
}
