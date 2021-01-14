#include <QCoreApplication>

#include "messageclient.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    MessageClient client;
//    client.ping();

    return a.exec();
}
