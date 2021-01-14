#include <QCoreApplication>
#include <QDebug>

#include "messageserver.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MessageServer server;

    return a.exec();
}
