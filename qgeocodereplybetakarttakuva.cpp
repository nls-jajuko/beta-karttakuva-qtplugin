#include "qgeocodereplybetakarttakuva.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonObject>
#include <QtCore/QJsonArray>
#include <QtPositioning/QGeoCoordinate>
#include <QtPositioning/QGeoAddress>
#include <QtPositioning/QGeoLocation>
#include <QtPositioning/QGeoRectangle>
#include <QtLocation/private/qgeojson_p.h>

QT_BEGIN_NAMESPACE

QGeoCodeReplyBetaKarttakuva::QGeoCodeReplyBetaKarttakuva(QNetworkReply *reply, QObject *parent)
    :   QGeoCodeReply(*new QGeoCodeReplyBetaKarttakuvaPrivate, parent)
{
    if (!reply) {
        setError(UnknownError, QStringLiteral("Null reply"));
        return;
    }
    connect(reply, SIGNAL(finished()), this, SLOT(networkReplyFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)),
            this, SLOT(networkReplyError(QNetworkReply::NetworkError)));
    connect(this, &QGeoCodeReply::aborted, reply, &QNetworkReply::abort);
    connect(this, &QObject::destroyed, reply, &QObject::deleteLater);
    setLimit(1);
    setOffset(0);
}

QGeoCodeReplyBetaKarttakuva::~QGeoCodeReplyBetaKarttakuva()
{
}

static QGeoAddress parseObject(const QJsonObject &item)
{
    QString label;
    QString municipality;
    QString placeType;
    QString resultTitle ;

    if(item.contains(QStringLiteral("properties"))) {
        QJsonObject props = item.value(QStringLiteral("properties")).toObject();

        if( props.contains(QStringLiteral("label"))) {
           label = props.value(QStringLiteral("label")).toString();
        }

        if( props.contains(QStringLiteral("label:municipality"))) {
           municipality = props.value(QStringLiteral("label:municipality")).toString();
        }

        if( props.contains(QStringLiteral("label:placeType"))) {
           placeType = props.value(QStringLiteral("label:placeType")).toString();
        }

    }

    QString labelText =
            QString("%1 %2 (%3)").arg( label, municipality, placeType );



    QGeoAddress address;
    address.setText(labelText);
    return address;
}



void QGeoCodeReplyBetaKarttakuva::networkReplyFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
        return;

    QList<QGeoLocation> locations;
    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    if (document.isObject()) {
        QJsonObject object = document.object();

        if(object.contains(QStringLiteral("features"))) {


            QJsonArray results = object.value(QStringLiteral("features")).toArray();

            for (int i = 0; i < results.count(); ++i) {
                if (!results.at(i).isObject())
                    continue;

                QJsonObject object = results.at(i).toObject();

                QGeoCoordinate coordinate;

                if (object.contains(QStringLiteral("geometry"))) {
                    QJsonObject geom = object.value(QStringLiteral("geometry")).toObject();
                    QJsonArray a = geom.value(QStringLiteral("coordinates")).toArray();

                    double lon = a.at(0).toDouble(),
                           lat = a.at(1).toDouble();
                    coordinate.setLatitude(lat);
                    coordinate.setLongitude(lon);

                }

                QGeoLocation location;
                location.setCoordinate(coordinate);

                location.setAddress(parseObject(object));
                locations.append(location);
            }
        }

    }

    setLocations(locations);
    setFinished(true);
}

void QGeoCodeReplyBetaKarttakuva::networkReplyError(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error);
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    setError(QGeoCodeReply::CommunicationError, reply->errorString());
}

QGeoCodeReplyBetaKarttakuvaPrivate::QGeoCodeReplyBetaKarttakuvaPrivate()
{

}

QGeoCodeReplyBetaKarttakuvaPrivate::~QGeoCodeReplyBetaKarttakuvaPrivate()
{

}

QVariantMap QGeoCodeReplyBetaKarttakuvaPrivate::extraData() const
{
    return m_extraData;
}

QT_END_NAMESPACE
