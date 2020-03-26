#include "qplacesearchreplybetakarttakuva.h"
#include "qplacemanagerenginebetakarttakuva.h"

#include <QtCore/QJsonDocument>
#include <QtCore/QJsonArray>
#include <QtCore/QJsonObject>
#include <QtNetwork/QNetworkReply>
#include <QtPositioning/QGeoCircle>
#include <QtPositioning/QGeoRectangle>
#include <QtLocation/QPlaceResult>
#include <QtLocation/QPlaceSearchRequest>
#include <QtLocation/private/qplacesearchrequest_p.h>

QT_BEGIN_NAMESPACE

QPlaceSearchReplyBetaKarttakuva::QPlaceSearchReplyBetaKarttakuva(const QPlaceSearchRequest &request,
                                             QNetworkReply *reply, QPlaceManagerEngineBetaKarttakuva *parent)
:   QPlaceSearchReply(parent)
{
    Q_ASSERT(parent);
    if (!reply) {
        setError(UnknownError, QStringLiteral("Null reply"));
        return;
    }
    setRequest(request);

    connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
    connect(reply, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(networkError(QNetworkReply::NetworkError)));
    connect(this, &QPlaceReply::aborted, reply, &QNetworkReply::abort);
    connect(this, &QObject::destroyed, reply, &QObject::deleteLater);
}

QPlaceSearchReplyBetaKarttakuva::~QPlaceSearchReplyBetaKarttakuva()
{
}

void QPlaceSearchReplyBetaKarttakuva::setError(QPlaceReply::Error errorCode, const QString &errorString)
{
    QPlaceReply::setError(errorCode, errorString);
    emit error(errorCode, errorString);
    setFinished(true);
    emit finished();
}



void QPlaceSearchReplyBetaKarttakuva::replyFinished()
{
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    reply->deleteLater();

    if (reply->error() != QNetworkReply::NoError)
        return;

    QJsonDocument document = QJsonDocument::fromJson(reply->readAll());

    QJsonObject object = document.object();

    if( !object.contains("features") ) {
        return;
    }

    QJsonArray features = object.value(QStringLiteral("features")).toArray();

    QStringList placeIds;

    QList<QPlaceSearchResult> results;
    for (int i = 0; i < features.count(); ++i) {
        QJsonObject item = features.at(i).toObject();
        QPlaceResult pr = parsePlaceResult(item);
        placeIds.append(pr.place().placeId());
        results.append(pr);
    }

    QVariantMap searchContext = request().searchContext().toMap();

    setResults(results);

    setFinished(true);
    emit finished();
}

void QPlaceSearchReplyBetaKarttakuva::networkError(QNetworkReply::NetworkError error)
{
    Q_UNUSED(error);
    QNetworkReply *reply = static_cast<QNetworkReply *>(sender());
    reply->deleteLater();
    setError(QPlaceReply::CommunicationError, reply->errorString());
}

QPlaceResult QPlaceSearchReplyBetaKarttakuva::parsePlaceResult(const QJsonObject &item) const
{
    QPlace place;
    QGeoCoordinate coordinate ;

    if (item.contains(QStringLiteral("geometry"))) {
        QJsonObject geom = item.value(QStringLiteral("geometry")).toObject();
        QJsonArray a = geom.value(QStringLiteral("coordinates")).toArray();
        if (a.count() == 2) {
            coordinate.setLatitude(a.at(1).toString().toDouble());
            coordinate.setLongitude(a.at(0).toString().toDouble());

        }
    }

    QString title = item.value("label").toString();

    place.setName(title);


    QGeoLocation location;
    location.setCoordinate(coordinate);

    place.setLocation(location);

    QPlaceResult result;
    result.setPlace(place);
    result.setTitle(title);

    return result;
}

QT_END_NAMESPACE
