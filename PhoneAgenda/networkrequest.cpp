#include "networkrequest.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

Q_LOGGING_CATEGORY(appNetworking, "networking.log")

NetworkRequest::NetworkRequest()
{

}

void NetworkRequest::get()
{
    emit isGetDone(true);
    QUrl url{"http://jsonplaceholder.typicode.com/users"};
    QNetworkRequest networkRequest;

    networkRequest.setUrl(url);
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    m_networkAccessManager = new QNetworkAccessManager(this);

    connect(m_networkAccessManager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply)
    {
        QJsonArray result = QJsonDocument::fromJson(reply->readAll()).array();
        if (reply->error() == QNetworkReply::NoError)
        {
            m_data.clear();
            for(auto && item : result)
            {
                QJsonObject itemObject = item.toObject();
                m_data.append({itemObject.value("name").toString(), itemObject.value("phone").toString()});
            }
            qDebug() << result;
            emit isGetDone(false);
        }
        else
        {
            qDebug() << "error reply get" << reply->error();
        }
        //m_networkAccessManager->deleteLater();
    });

    connect(m_networkAccessManager, &QNetworkAccessManager::finished, m_networkAccessManager, &QObject::deleteLater);

    m_networkAccessManager->get(networkRequest);
    qDebug() << "trying to get data from" << url;
}

void NetworkRequest::post()
{
    emit isPostDone(true);
    QUrl url{"http://httpbin.org/post"};
    QNetworkRequest networkRequest;

    m_networkAccessManager = new QNetworkAccessManager(this);

    networkRequest.setUrl(url);
    networkRequest.setHeader(QNetworkRequest::ContentTypeHeader, "application/json");

    connect(m_networkAccessManager, &QNetworkAccessManager::finished, this, [=](QNetworkReply *reply)
    {
        if (reply->error() == QNetworkReply::NoError)
        {
            qDebug() << QJsonDocument::fromJson(reply->readAll());
            emit isPostDone(false);
        }
        else
        {
            qDebug() << "error reply post" << reply->error();
        }
    });

    connect(m_networkAccessManager, &QNetworkAccessManager::finished, m_networkAccessManager, &QObject::deleteLater);

    m_networkAccessManager->post(networkRequest, QJsonDocument(toJson(m_data)).toJson(QJsonDocument::Compact));
    qDebug() << "trying to post data to" << url;
}

QList<JsonFields> NetworkRequest::getData()
{
    return m_data;
}

void NetworkRequest::setData(const QList<JsonFields> &postData)
{
    m_data.clear();
    for(int i = 0; i < postData.size(); ++i)
    {
        m_data.append({postData[i].name, postData[i].phoneNumber});
    }
}

QJsonArray NetworkRequest::toJson(const QList<JsonFields> &list)
{
    QJsonArray array;
    for (auto & user : list)
        array.append(user.toJson());
    return array;
}
