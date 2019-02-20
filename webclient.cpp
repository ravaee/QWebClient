#include "webclient.h"

RavasaWebClient::RavasaWebClient(QObject *parent) : QObject(parent)
{
    setUrl(hostName+"/api/");
    setMinUrl(hostName);
}

void RavasaWebClient::get(QString controller, QString action, QJSValue val1)
{
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onDataRecived(QNetworkReply *)));
    QNetworkRequest  req;
    req.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
    QMap<QString, QVariant> props=val1.toVariant().toMap();
    int counter=0;
    QString getUrl;
    if(props.keys().count()>0){
        getUrl = url() + controller + "/" + action+"?";
        foreach (auto key, props.keys()) {
            if(counter==0){
                getUrl+=key+"="+props.value(key).toString();
                counter++;
            }else{
                getUrl+="&"+key+"="+props.value(key).toString();
            }
        }
    }else{
        getUrl = url() + controller + "/" + action;
    }

    qDebug()<<"#CPP SAY: "<<getUrl;
    req.setUrl(getUrl);
    mgr->get(req);
}

void RavasaWebClient::getWithToken(QString controller, QString action, QJSValue val1,QString token)
{
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onDataRecived(QNetworkReply *)));
    QNetworkRequest  req;
    req.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
    req.setRawHeader("Authorization", QString("Bearer "+token).toLocal8Bit());
    QMap<QString, QVariant> props=val1.toVariant().toMap();
    int counter=0;
    QString getUrl;
    if(props.keys().count()>0){
        getUrl = url() + controller + "/" + action+"?";
        foreach (auto key, props.keys()) {
            if(counter==0){
                getUrl+=key+"="+props.value(key).toString();
            }else{
                getUrl+="&"+key+"="+props.value(key).toString();
            }
        }
    }else{
        getUrl = url() + controller + "/" + action;
    }
    qDebug()<<"#CPP SAY: "<<getUrl;
    req.setUrl(getUrl);
    mgr->get(req);
}

void RavasaWebClient::getWithTokenUnpatterned(QString controller, QString action, QJSValue val1, QString token)
{
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onDataRecivedUnpatterned(QNetworkReply *)));
    QNetworkRequest  req;
    req.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
    req.setRawHeader("Authorization", QString("Bearer "+token).toLocal8Bit());
    QMap<QString, QVariant> props=val1.toVariant().toMap();
    int counter=0;
    QString getUrl;
    if(props.keys().count()>0){
        getUrl = url() + controller + "/" + action+"?";
        foreach (auto key, props.keys()) {
            if(counter==0){
                getUrl+=key+"="+props.value(key).toString();
            }else{
                getUrl+="&"+key+"="+props.value(key).toString();
            }
        }
    }else{
        getUrl = url() + controller + "/" + action;
    }
    qDebug()<<"#CPP SAY: "<<getUrl;
    req.setUrl(getUrl);
    mgr->get(req);
}

void RavasaWebClient::post(QString controller, QString action, QJSValue val1)
{
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onDataRecived(QNetworkReply *)));
    QNetworkRequest req;
    req.setUrl(QUrl(url() + controller + "/" + action));
    req.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
    QUrlQuery params;
    QMap<QString, QVariant> props=val1.toVariant().toMap();
    foreach (auto key, props.keys()) {
        params.addQueryItem(key,props.value(key).toString());
    }
    mgr->post(req,params.query(QUrl::FullyEncoded).toUtf8());
}

void RavasaWebClient::getToken(QString username,QString password)
{
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onDataRecived(QNetworkReply *)));

    QUrlQuery params;
    params.addQueryItem("username",username);
    params.addQueryItem("password",password);
    params.addQueryItem("grant_type","password");

    QNetworkRequest  req =  QNetworkRequest(QUrl(minUrl()+"/Token"));
    req.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/x-www-form-urlencoded"));
    mgr->post(req,params.query(QUrl::FullyEncoded).toUtf8());
}

void RavasaWebClient::sendJson(QVariant data)
{
    QJsonDocument jsonDoc = QJsonDocument::fromVariant(data.value<QJSValue>().toVariant());
    qDebug() << jsonDoc.toJson();
}

void RavasaWebClient::postJson(QString controller, QString action,QVariant data)
{
    QNetworkAccessManager * mgr = new QNetworkAccessManager(this);
    connect(mgr,SIGNAL(finished(QNetworkReply*)),this,SLOT(onDataRecived(QNetworkReply *)));
    QNetworkRequest req;
    req.setUrl(QUrl(url() + controller + "/" + action));
    req.setHeader(QNetworkRequest::ContentTypeHeader,QVariant("application/json"));
    mgr->post(req,QJsonDocument::fromVariant(data.value<QJSValue>().toVariant()).toJson());
    qDebug() << QJsonDocument::fromVariant(data.value<QJSValue>().toVariant()).toJson();
}

//SLOTS

void RavasaWebClient::onDataRecived(QNetworkReply *rep)
{
    QByteArray response = rep->readAll();
    if(QString::fromLocal8Bit(response).contains( "done") || QString::fromLocal8Bit(response).contains("Done")){
        emit dataRecived(response);
        return;
    }
    QJsonDocument doc(QJsonDocument::fromJson(response));
    QJsonObject json = doc.object();
    qDebug()<<"#CPP SAY : "<<QString::fromLocal8Bit(response);
    if(json["Result"]=="Done" || json["Result"]=="done"){
        emit dataRecived(response);
    }else if(json["Result"]=="Error"){
        emit errorRecived(response);
    }else if(json["token_type"]=="bearer"){
        emit dataRecived(response);
    }else{
        emit connectionError(response);
    }
}

void RavasaWebClient::onDataRecivedUnpatterned(QNetworkReply *rep)
{
    QByteArray response = rep->readAll();
    QString res = QString::fromLocal8Bit(response);

    qDebug()<<"#CPP SAY : "<<QString::fromLocal8Bit(response);
    if(res == ""){
        emit connectionError("");
    }else{
        emit dataRecived(response);
    }
}
