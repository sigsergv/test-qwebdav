#include <QtCore>
#include <QtDebug>

#include "qwebdav.h"

struct QWebDav::Private
{
    QUrl baseUrl;
    QString username;
    QString password;
};

QWebDav::QWebDav(QObject* parent)
    : QNetworkAccessManager(parent)
{
    p = new Private();
}

QWebDav::~QWebDav()
{
    delete p;
}

void QWebDav::connectToHost(const QString & hostName, quint16 port)
{
    // Do nothing
}

void QWebDav::connectToHost(const QString & hostName, quint16 port, const QString & username, const QString & password) 
{
    p->baseUrl.setScheme("http");
    p->baseUrl.setHost(hostName);
    p->baseUrl.setPort(port);
    p->username = username;
    p->password = password;

    QUrl reqUrl(p->baseUrl);
    QNetworkRequest req;

    req.setUrl(reqUrl);

    QNetworkReply * reply = createRequest("GET", req, QByteArray());
    QEventLoop loop;
    connect(reply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    QString response = QString::fromUtf8(reply->readAll());
    std::cout << "finished:" << std::endl;
    std::cout << response.toStdString() << std::endl;
    std::cout << "Headers:" << std::endl;

    int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    foreach (const QNetworkReply::RawHeaderPair & p, reply->rawHeaderPairs()) {
        std::cout << p.first.constData() << ": " << p.second.constData() << std::endl;
    }

}


QNetworkReply * QWebDav::createRequest(const QString& method, QNetworkRequest& req, QIODevice* outgoingData)
{
    if(outgoingData != 0 && outgoingData->size() !=0) {
        req.setHeader(QNetworkRequest::ContentLengthHeader, outgoingData->size());
        req.setHeader(QNetworkRequest::ContentTypeHeader, "text/xml; charset=utf-8");
    }

    return sendCustomRequest(req, method.toLatin1(), outgoingData);
}


QNetworkReply * QWebDav::createRequest(const QString & method, QNetworkRequest & req, const QByteArray& outgoingData)
{ 
    QBuffer* dataIO = new QBuffer;
    dataIO->setData(outgoingData);
    dataIO->open(QIODevice::ReadOnly);

    QNetworkReply* reply = createRequest(method, req, dataIO);
    // m_outDataDevices.insert(reply, dataIO);
    return reply;
}