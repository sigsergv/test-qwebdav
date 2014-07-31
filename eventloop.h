#include <QEventLoop>
#include <QNetworkReply>

class EventLoop : public QEventLoop
{
    Q_OBJECT
public:
    EventLoop(QObject * parent = 0);

public slots:
    void quit(QNetworkReply::NetworkError code);
};
