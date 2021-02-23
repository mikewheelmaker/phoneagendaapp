#ifndef NETWORKREQUEST_H
#define NETWORKREQUEST_H

#include <QObject>
#include <QNetworkRequest>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QJsonObject>
#include <QList>
#include <QLoggingCategory>

Q_DECLARE_LOGGING_CATEGORY(appNetworking)

struct JsonFields {
    QString name;
    QString phoneNumber;
    QJsonObject toJson() const
    {
        return {{"name", name}, {"phoneNumber", phoneNumber}};
    }
};

class NetworkRequest : public QObject
{
    Q_OBJECT
public:
    NetworkRequest();

    void get();
    void post();

    QList<JsonFields> getData();
    void setData(const QList<JsonFields>& postData);

    QJsonArray toJson(const QList<JsonFields>& list);

signals:
    void isGetDone(bool newIsGetDone);
    void isPostDone(bool newIsPostDone);

private:
    QNetworkAccessManager* m_networkAccessManager;
    QList<JsonFields> m_data;
};

#endif // NETWORKREQUEST_H
