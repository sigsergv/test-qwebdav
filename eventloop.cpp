#include <QtDebug>

#include "eventloop.h"

EventLoop::EventLoop(QObject * parent)
    :QEventLoop(parent)
{

}

void EventLoop::quit(QNetworkReply::NetworkError code)
{
    Q_UNUSED(code);
    
    qDebug() << "QUIT";
    QEventLoop::quit();
}