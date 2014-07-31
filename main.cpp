#include <QtCore>
#include <QtDebug>

#include "qwebdav.cpp"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    QWebDav wd;
    QWebDav::Error error;
    wd.connectToHost("192.168.1.35", 48073, "dav", "dav");
    
    error = wd.lastError();

    if (error != QWebDav::NoError) {
        switch (error) {
            case QWebDav::AuthFailedError:
                qDebug() << "Auth failed";
                break;

            case QWebDav::ConnectionTimeoutError:
                qDebug() << "Connection timeout";
                break;

            case QWebDav::NetworkError:
                qDebug() << "Network error";
                break;

            case QWebDav::NoError:
                break;
        }
        return 1;
    }

    wd.list("/%D0%9E%D0%BB%D1%8C%D0%B3%D0%B0%20%D0%93%D1%80%D0%BE%D0%BC%D1%8B%D0%BA%D0%BE/");
    error = wd.lastError();

    if (error != QWebDav::NoError) {
        switch (error) {
            case QWebDav::AuthFailedError:
                qDebug() << "Auth failed";
                break;

            case QWebDav::ConnectionTimeoutError:
                qDebug() << "Connection timeout";
                break;

            case QWebDav::NetworkError:
                qDebug() << "Network error";
                break;

            case QWebDav::NoError:
                break;
        }
    }
    
}
