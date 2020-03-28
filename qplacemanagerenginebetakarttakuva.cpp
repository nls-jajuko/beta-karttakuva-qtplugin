#include "qplacemanagerenginebetakarttakuva.h"
#include "qplacesearchreplybetakarttakuva.h"

#include <QtCore/QUrlQuery>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QRegularExpression>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtPositioning/QGeoCircle>
#include <QtLocation/private/unsupportedreplies_p.h>

#include <QtCore/QElapsedTimer>



QPlaceManagerEngineBetaKarttakuva::QPlaceManagerEngineBetaKarttakuva(const QVariantMap &parameters,
                                               QGeoServiceProvider::Error *error,
                                               QString *errorString)
:   QPlaceManagerEngine(parameters), m_networkManager(new QNetworkAccessManager(this)),
    m_categoriesReply(0)
{
    if (parameters.contains(QStringLiteral("betakarttakuva.useragent")))
        m_userAgent = parameters.value(QStringLiteral("BetaKarttakuva.useragent")).toString().toLatin1();
    else
        m_userAgent = "Qt Location based application";

    if (parameters.contains(QStringLiteral("betakarttakuva.places.host")))
        m_urlPrefix = parameters.value(QStringLiteral("betakarttakuva.places.host")).toString();
    else
        m_urlPrefix = QStringLiteral("https://beta-karttakuva.maanmittauslaitos.fi");


    if (parameters.contains(QStringLiteral("betakarttakuva.places.debug_query")))
        m_debugQuery = parameters.value(QStringLiteral("BetaKarttakuva.places.debug_query")).toBool();

    if (parameters.contains(QStringLiteral("betakarttakuva.places.limit"))
            && parameters.value(QStringLiteral("BetaKarttakuva.places.limit")).canConvert<int>())
        m_limit = parameters.value(QStringLiteral("BetaKarttakuva.places.limit")).toInt();

    *error = QGeoServiceProvider::NoError;
    errorString->clear();
}

QPlaceManagerEngineBetaKarttakuva::~QPlaceManagerEngineBetaKarttakuva()
{
}

QPlaceSearchReply *QPlaceManagerEngineBetaKarttakuva::search(const QPlaceSearchRequest &request)
{
    qInfo() << "About to geocode with betakarttakuva";
    bool unsupported = false;

    // Only public visibility supported
    unsupported |= request.visibilityScope() != QLocation::UnspecifiedVisibility &&
                   request.visibilityScope() != QLocation::PublicVisibility;
    unsupported |= request.searchTerm().isEmpty() ;

    if (unsupported) {
        qInfo() << "Unsupported at betakarttakuva";

        return QPlaceManagerEngine::search(request);
    }

    QUrlQuery queryItems;

    queryItems.addQueryItem(QStringLiteral("text"), request.searchTerm());

    QVariantMap parameters = request.searchContext().toMap();

    queryItems.addQueryItem(QStringLiteral("crs"), QStringLiteral("http://www.opengis.net/def/crs/EPSG/0/4326"));

    queryItems.addQueryItem(QStringLiteral("limit"), (request.limit() > 0) ? QString::number(request.limit())
                                                                           : QString::number(m_limit));

    QUrl requestUrl(QString("%1/geocoding/v1/pelias/search").arg(m_urlPrefix));



    requestUrl.setQuery(queryItems);

    qInfo() << "URL" << requestUrl.toString();


    QNetworkRequest rq(requestUrl);
    rq.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);
    QNetworkReply *networkReply = m_networkManager->get(rq);

    QPlaceSearchReplyBetaKarttakuva *reply = new QPlaceSearchReplyBetaKarttakuva(request, networkReply, this);
    connect(reply, SIGNAL(finished()), this, SLOT(replyFinished()));
    connect(reply, SIGNAL(error(QPlaceReply::Error,QString)),
            this, SLOT(replyError(QPlaceReply::Error,QString)));

    if (m_debugQuery)
        reply->requestUrl = requestUrl.url(QUrl::None);

    return reply;
}

QPlaceReply *QPlaceManagerEngineBetaKarttakuva::initializeCategories()
{
   return 0;
}

QString QPlaceManagerEngineBetaKarttakuva::parentCategoryId(const QString &categoryId) const
{
    Q_UNUSED(categoryId);

    // Only a two category levels
    return QString();
}

QStringList QPlaceManagerEngineBetaKarttakuva::childCategoryIds(const QString &categoryId) const
{
    return m_subcategories.value(categoryId);
}

QPlaceCategory QPlaceManagerEngineBetaKarttakuva::category(const QString &categoryId) const
{
    return m_categories.value(categoryId);
}

QList<QPlaceCategory> QPlaceManagerEngineBetaKarttakuva::childCategories(const QString &parentId) const
{
    QList<QPlaceCategory> categories;
    foreach (const QString &id, m_subcategories.value(parentId))
        categories.append(m_categories.value(id));
    return categories;
}

QList<QLocale> QPlaceManagerEngineBetaKarttakuva::locales() const
{
    return m_locales;
}

void QPlaceManagerEngineBetaKarttakuva::setLocales(const QList<QLocale> &locales)
{
    m_locales = locales;
}

void QPlaceManagerEngineBetaKarttakuva::categoryReplyFinished()
{
}

void QPlaceManagerEngineBetaKarttakuva::categoryReplyError()
{
}

void QPlaceManagerEngineBetaKarttakuva::replyFinished()
{
    QPlaceReply *reply = qobject_cast<QPlaceReply *>(sender());
    if (reply)
        emit finished(reply);
}

void QPlaceManagerEngineBetaKarttakuva::replyError(QPlaceReply::Error errorCode, const QString &errorString)
{
    QPlaceReply *reply = qobject_cast<QPlaceReply *>(sender());
    if (reply)
        emit error(reply, errorCode, errorString);
}

void QPlaceManagerEngineBetaKarttakuva::fetchNextCategoryLocale()
{
}
