#ifndef QWEBDAV_H
#define QWEBDAV_H

#include <QtCore>
#include <QtNetwork>

/**
 * WebDav connection class.
 *
 * It implements very basic support of WEBDAV protocol.
 */
class QWebDav : QNetworkAccessManager
{
    Q_OBJECT
public:
    QWebDav(QObject* parent = 0);
    ~QWebDav();

    void connectToHost(const QString & hostName, quint16 port, const QString & username, const QString & password);

protected:
    QNetworkReply * createRequest(const QString& method, QNetworkRequest& req, const QByteArray& outgoingData);
    QNetworkReply * createRequest(const QString& method, QNetworkRequest& req, QIODevice* outgoingData);


private:
    struct Private;
    Private * p;
    // hide this method
    void connectToHost(const QString & hostName, quint16 port = 80);
};
#endif
