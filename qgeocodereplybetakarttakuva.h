#ifndef QGeoCodeReplyBetaKarttakuva_H
#define QGeoCodeReplyBetaKarttakuva_H

#include <QtNetwork/QNetworkReply>
#include <QtLocation/QGeoCodeReply>
#include <QtLocation/private/qgeocodereply_p.h>

QT_BEGIN_NAMESPACE

class QGeoCodeReplyBetaKarttakuva : public QGeoCodeReply
{
    Q_OBJECT

public:
    explicit QGeoCodeReplyBetaKarttakuva(QNetworkReply *reply, QObject *parent = 0);
    ~QGeoCodeReplyBetaKarttakuva();

private Q_SLOTS:
    void networkReplyFinished();
    void networkReplyError(QNetworkReply::NetworkError error);

private:
    bool m_includeExtraData = false;
};

class QGeoCodeReplyBetaKarttakuvaPrivate : public QGeoCodeReplyPrivate
{
public:
    QGeoCodeReplyBetaKarttakuvaPrivate();
    ~QGeoCodeReplyBetaKarttakuvaPrivate();
    QVariantMap extraData() const override;

    QVariantMap m_extraData;
};

QT_END_NAMESPACE

#endif // QGeoCodeReplyBetaKarttakuva_H
