#include "ContactManagementSys.h"
#include <QtWidgets/QApplication>
#include "Contact.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ContactManagementSys w;
    w.show();
    return a.exec();
}
