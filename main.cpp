#include <QtCore>
#include <QtDebug>

#include "qwebdav.cpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QWebDav wd;
    wd.connectToHost("192.168.40.118", 8888, "user", "###davdrive");
    
    switch (wd.lastError()) {
        case QWebDav::AuthFailedError:
            qDebug() << "Auth failed";
            break;

        case QWebDav::NoError:
            qDebug() << "No errors, connection OK";
            break;
    }

    
    
}