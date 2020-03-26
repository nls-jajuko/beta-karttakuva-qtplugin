#ifndef QPLACESEARCHREPLYBETAKARTTAKUVA_H
#define QPLACESEARCHREPLYBETAKARTTAKUVA_H

#include <QtLocation/QPlaceSearchReply>
#include <QNetworkReply>

QT_BEGIN_NAMESPACE

class QNetworkReply;
class QPlaceManagerEngineBetaKarttakuva;
class QPlaceResult;

class QPlaceSearchReplyBetaKarttakuva : public QPlaceSearchReply
{
    Q_OBJECT

public:
    QPlaceSearchReplyBetaKarttakuva(const QPlaceSearchRequest &request, QNetworkReply *reply,
                          QPlaceManagerEngineBetaKarttakuva *parent);
    ~QPlaceSearchReplyBetaKarttakuva();

    QString requestUrl;

private slots:
    void setError(QPlaceReply::Error errorCode, const QString &errorString);
    void replyFinished();
    void networkError(QNetworkReply::NetworkError error);

private:
    QPlaceResult parsePlaceResult(const QJsonObject &item) const;
};

QT_END_NAMESPACE

#endif // QPLACESEARCHREPLYBetaKarttakuva_H
