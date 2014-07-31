#include <QtCore>
#include <QtDebug>
#include <QtNetwork>

#include "qwebdav.h"
// #include "eventloop.h"

struct QWebDav::Private
{
    QUrl baseUrl;
    QString username;
    QString password;
    QWebDav::Error lastError;

    QNetworkReply * lastAuthReply;
};

QWebDav::QWebDav(QObject* parent)
    : QNetworkAccessManager(parent)
{
    p = new Private();
    p->lastError = NoError;

    connect(this, SIGNAL(authenticationRequired(QNetworkReply*,QAuthenticator*)), this, SLOT(provideAuthentication(QNetworkReply*,QAuthenticator*)));
}

QWebDav::~QWebDav()
{
    delete p;
}

QWebDav::Error QWebDav::lastError()
{
    return p->lastError;
}


void QWebDav::connectToHost(const QString & hostName, quint16 port)
{
    Q_UNUSED(hostName);
    Q_UNUSED(port);
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
    QNetworkRequest request;

    request.setUrl(reqUrl);

    p->lastError = NoError;
    QNetworkReply * reply = createRequest("HEAD", request, QByteArray());

    QEventLoop * loop;
    loop = new QEventLoop();
    connect(reply, SIGNAL(finished()), loop, SLOT(quit()));
    QTimer::singleShot(2000, loop, SLOT(quit()));  // 2 seconds timeout
    loop->exec();
    if (reply->isRunning()) {
        // connection is not established, so terminated it and emit corresponding signal
        reply->abort();
        return;
    }
    delete loop;

    // connection successful, so destroy reply and continue
    reply->abort();
    reply->deleteLater();

    /*
    QString response = QString::fromUtf8(reply->readAll());
    qDebug() << "finished: " << response;

    qDebug() << "Headers:";

    // int statusCode = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();

    foreach (const QNetworkReply::RawHeaderPair & p, reply->rawHeaderPairs()) {
        qDebug() << p.first.constData() << ": " << p.second.constData();
    }
    */
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
    return reply;
}


void QWebDav::provideAuthentication(QNetworkReply * reply, QAuthenticator * authenticator)
{
    Q_UNUSED(reply);

    if (reply == p->lastAuthReply) {
        reply->abort();
        reply->deleteLater();
        p->lastAuthReply = 0;
        p->lastError = AuthFailedError;
        return;
    }

    authenticator->setUser(p->username);
    authenticator->setPassword(p->password);

    p->lastAuthReply = reply;
}