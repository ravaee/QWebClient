#ifndef WEBCLIENT_H
#define WEBCLIENT_H

#include <QJsonObject>
#include <QJsonDocument>
#include <QMap>
#include <QVariant>
#include <QtCore/QObject>
#include <QtCore/qglobal.h>
#include <QtQml/QJSValue>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QUrlQuery>
#include <settingcontroller.h>
class RavasaWebClient : public QObject,protected SettingController
{
    Q_OBJECT

    Q_PROPERTY(QString url READ url WRITE setUrl NOTIFY urlChanged)
    Q_PROPERTY(QString minUrl READ minUrl WRITE setMinUrl NOTIFY minUrlChanged)

    QString m_url;
    QString m_minUrl;

public:
    explicit RavasaWebClient(QObject *parent = nullptr);

    Q_INVOKABLE void get(QString controller, QString action, QJSValue val1);
    Q_INVOKABLE void getWithToken(QString controller, QString action, QJSValue val1, QString token);
    Q_INVOKABLE void getWithTokenUnpatterned(QString controller, QString action, QJSValue val1, QString token);
    Q_INVOKABLE void post(QString controller, QString action, QJSValue val1);
    Q_INVOKABLE void getToken(QString username, QString password);

    Q_INVOKABLE void sendJson(QVariant object);

    Q_INVOKABLE void postJson(QString controller, QString action,QVariant data);

    QString url() const
    {
        return m_url;
    }

    void setUrl(QString url)
    {
        if (m_url == url)
            return;
        m_url = url;
        emit urlChanged(m_url);
    }

    QString minUrl() const
    {
        return m_minUrl;
    }

signals:

    void dataRecived(QString jsData);
    void errorRecived(QString jsError);
    void connectionError(QString ErrorNistVaErrorNakhahimFahmid);
    void urlChanged(QString url);


    void minUrlChanged(QString minUrl);

public slots:
    void onDataRecived(QNetworkReply *rep);
    void onDataRecivedUnpatterned(QNetworkReply *rep);

    void setMinUrl(QString minUrl)
    {
        if (m_minUrl == minUrl)
            return;

        m_minUrl = minUrl;
        emit minUrlChanged(m_minUrl);
    }
};



#endif // WEBCLIENT_H
