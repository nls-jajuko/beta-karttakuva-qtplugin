#ifndef QGeocodingManagerEngineBetaKarttakuva_H
#define QGeocodingManagerEngineBetaKarttakuva_H

#include <QtLocation/QGeoServiceProvider>
#include <QtLocation/QGeoCodingManagerEngine>
#include <QtLocation/QGeoCodeReply>

QT_BEGIN_NAMESPACE

class QNetworkAccessManager;

class QGeocodingManagerEngineBetaKarttakuva : public QGeoCodingManagerEngine
{
    Q_OBJECT

public:
    QGeocodingManagerEngineBetaKarttakuva(const QVariantMap &parameters, QGeoServiceProvider::Error *error,
                               QString *errorString);
    ~QGeocodingManagerEngineBetaKarttakuva();

    QGeoCodeReply *geocode(const QGeoAddress &address, const QGeoShape &bounds) override;
    QGeoCodeReply *geocode(const QString &address, int limit, int offset,
                           const QGeoShape &bounds) override;
    QGeoCodeReply *reverseGeocode(const QGeoCoordinate &coordinate,
                                  const QGeoShape &bounds) override;

private Q_SLOTS:
    void replyFinished();
    void replyError(QGeoCodeReply::Error errorCode, const QString &errorString);

private:
    QNetworkAccessManager *m_networkManager;
    QByteArray m_userAgent;
    QString m_urlPrefix;
    bool m_debugQuery = false;

};

QT_END_NAMESPACE

#endif // QGeocodingManagerEngineBetaKarttakuva_H
