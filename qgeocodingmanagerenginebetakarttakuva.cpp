#include "qgeocodingmanagerenginebetakarttakuva.h"

#include <QtCore/QVariantMap>
#include <QtCore/QUrl>
#include <QtCore/QUrlQuery>
#include <QtCore/QLocale>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtPositioning/QGeoCoordinate>
#include <QtPositioning/QGeoAddress>
#include <QtPositioning/QGeoShape>
#include <QtPositioning/QGeoRectangle>
#include "qgeocodereplybetakarttakuva.h"


QT_BEGIN_NAMESPACE

static QString addressToQuery(const QGeoAddress &address)
{
    return address.street() + QStringLiteral(" ") +
           address.district() + QStringLiteral(" ") +
           address.city() + QStringLiteral(" ") +
           address.state() + QStringLiteral(" ") +
           address.country();
}


QGeocodingManagerEngineBetaKarttakuva::QGeocodingManagerEngineBetaKarttakuva(const QVariantMap &parameters,
                                                       QGeoServiceProvider::Error *error,
                                                       QString *errorString)
:   QGeoCodingManagerEngine(parameters), m_networkManager(new QNetworkAccessManager(this))
{
    if (parameters.contains(QStringLiteral("betakarttakuva.useragent")))
        m_userAgent = parameters.value(QStringLiteral("betakarttakuva.useragent")).toString().toLatin1();
    else
        m_userAgent = "Qt Location based application";

    if (parameters.contains(QStringLiteral("betakarttakuva.geocoding.host")))
        m_urlPrefix = parameters.value(QStringLiteral("betakarttakuva.geocoding.host")).toString().toLatin1();
    else
        m_urlPrefix = QStringLiteral("https://beta-karttakuva.maanmittauslaitos.fi");

    if (parameters.contains(QStringLiteral("betakarttakuva.geocoding.debug_query")))
        m_debugQuery = parameters.value(QStringLiteral("betakarttakuva.geocoding.debug_query")).toBool();

    *error = QGeoServiceProvider::NoError;
    errorString->clear();
}

QGeocodingManagerEngineBetaKarttakuva::~QGeocodingManagerEngineBetaKarttakuva()
{
}

QGeoCodeReply *QGeocodingManagerEngineBetaKarttakuva::geocode(const QGeoAddress &address, const QGeoShape &bounds)
{
    return geocode(addressToQuery(address), -1, -1, bounds);
}

QGeoCodeReply *QGeocodingManagerEngineBetaKarttakuva::geocode(const QString &address, int limit, int offset, const QGeoShape &bounds)
{
    Q_UNUSED(offset);
    Q_UNUSED(bounds);

    // https://beta-paikkatieto.maanmittauslaitos.fi/geocoding/v1/pelias/search?
    //    lang=fin&crs=http://www.opengis.net/def/crs/EPSG/0/4326&size=50&text=pasila

    QNetworkRequest request;
    request.setRawHeader("User-Agent", m_userAgent);

    QUrl url(QString("%1/geocoding/v1/pelias/search").arg(m_urlPrefix));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("text"), address);
    query.addQueryItem(QStringLiteral("accept-language"), locale().name().left(2));
    query.addQueryItem(QStringLiteral("crs"), QStringLiteral("http://www.opengis.net/def/crs/EPSG/0/4326"));

    /* // todo this is reverse with text in beta-karttakuva
    if (bounds.type() != QGeoShape::UnknownType) {
        query.addQueryItem(QStringLiteral("viewbox"), boundingBoxToLtrb(bounds.boundingGeoRectangle()));
        query.addQueryItem(QStringLiteral("bounded"), QStringLiteral("1"));
    }
    */
    if (limit != -1)
        query.addQueryItem(QStringLiteral("size"), QString::number(limit));

    url.setQuery(query);
    request.setUrl(url);

    QNetworkReply *reply = m_networkManager->get(request);

    QGeoCodeReplyBetaKarttakuva *geocodeReply = new  QGeoCodeReplyBetaKarttakuva(reply, this);
    if (m_debugQuery) {
        QGeoCodeReplyBetaKarttakuvaPrivate *replyPrivate
                = static_cast<QGeoCodeReplyBetaKarttakuvaPrivate *>(QGeoCodeReplyPrivate::get(*geocodeReply));
        replyPrivate->m_extraData["request_url"] = url;
    }

    connect(geocodeReply, SIGNAL(finished()), this, SLOT(replyFinished()));
    connect(geocodeReply, SIGNAL(error(QGeoCodeReply::Error,QString)),
            this, SLOT(replyError(QGeoCodeReply::Error,QString)));

    return geocodeReply;
}

QGeoCodeReply *QGeocodingManagerEngineBetaKarttakuva::reverseGeocode(const QGeoCoordinate &coordinate,
                                                          const QGeoShape &bounds)
{
    Q_UNUSED(bounds);

    QNetworkRequest request;
    request.setRawHeader("User-Agent", m_userAgent);

    // https://beta-paikkatieto.maanmittauslaitos.fi/geocoding/v1/pelias/reverse?
    //  lang=fin&crs=http://www.opengis.net/def/crs/EPSG/0/4326&size=50&
    //  boundary.circle.radius=999&point.lon=24.93435767824873&point.lat=60.199406543931275&

    QUrl url(QString("%1/geocoding/v1/pelias/reverse").arg(m_urlPrefix));
    QUrlQuery query;
    query.addQueryItem(QStringLiteral("crs"), QStringLiteral("http://www.opengis.net/def/crs/EPSG/0/4326"));
    query.addQueryItem(QStringLiteral("boundary.circle.radius"), QString::number(999));
    query.addQueryItem(QStringLiteral("point.lat"), QString::number(coordinate.latitude()));
    query.addQueryItem(QStringLiteral("point.lon"), QString::number(coordinate.longitude()));

    url.setQuery(query);
    request.setUrl(url);

    QNetworkReply *reply = m_networkManager->get(request);

    QGeoCodeReplyBetaKarttakuva *geocodeReply = new QGeoCodeReplyBetaKarttakuva(reply, this);
    if (m_debugQuery) {
        QGeoCodeReplyBetaKarttakuvaPrivate *replyPrivate
                = static_cast<QGeoCodeReplyBetaKarttakuvaPrivate *>(QGeoCodeReplyPrivate::get(*geocodeReply));
        replyPrivate->m_extraData["request_url"] = url;
    }

    connect(geocodeReply, SIGNAL(finished()), this, SLOT(replyFinished()));
    connect(geocodeReply, SIGNAL(error(QGeoCodeReply::Error,QString)),
            this, SLOT(replyError(QGeoCodeReply::Error,QString)));

    return geocodeReply;
}

void QGeocodingManagerEngineBetaKarttakuva::replyFinished()
{
    QGeoCodeReply *reply = qobject_cast<QGeoCodeReply *>(sender());
    if (reply)
        emit finished(reply);
}

void QGeocodingManagerEngineBetaKarttakuva::replyError(QGeoCodeReply::Error errorCode, const QString &errorString)
{
    QGeoCodeReply *reply = qobject_cast<QGeoCodeReply *>(sender());
    if (reply)
        emit error(reply, errorCode, errorString);
}

QT_END_NAMESPACE
