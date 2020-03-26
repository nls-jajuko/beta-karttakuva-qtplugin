#ifndef QPLACEMANAGERENGINEBETAKARTTAKUVA_H
#define QPLACEMANAGERENGINEBETAKARTTAKUVA_H

#include <QtLocation/QPlaceManagerEngine>
#include <QtLocation/QGeoServiceProvider>

QT_BEGIN_NAMESPACE

class QNetworkAccessManager;
class QNetworkReply;
class QPlaceCategoriesReplyBetaKarttakuva;

class QPlaceManagerEngineBetaKarttakuva : public QPlaceManagerEngine
{
    Q_OBJECT

public:
    QPlaceManagerEngineBetaKarttakuva(const QVariantMap &parameters, QGeoServiceProvider::Error *error,
                           QString *errorString);
    ~QPlaceManagerEngineBetaKarttakuva();

    QPlaceSearchReply *search(const QPlaceSearchRequest &request) override;

    QPlaceReply *initializeCategories() override;
    QString parentCategoryId(const QString &categoryId) const override;
    QStringList childCategoryIds(const QString &categoryId) const override;
    QPlaceCategory category(const QString &categoryId) const override;

    QList<QPlaceCategory> childCategories(const QString &parentId) const override;

    QList<QLocale> locales() const override;
    void setLocales(const QList<QLocale> &locales) override;

private slots:
    void categoryReplyFinished();
    void categoryReplyError();
    void replyFinished();
    void replyError(QPlaceReply::Error errorCode, const QString &errorString);

private:
    void fetchNextCategoryLocale();

    QNetworkAccessManager *m_networkManager;
    QByteArray m_userAgent;
    QString m_urlPrefix;
    QList<QLocale> m_locales;
    bool m_debugQuery = false;
    int m_limit = 50; // the default page size of the public nominatim server

    QNetworkReply *m_categoriesReply;
    QList<QPlaceCategoriesReplyBetaKarttakuva *> m_pendingCategoriesReply;
    QHash<QString, QPlaceCategory> m_categories;
    QHash<QString, QStringList> m_subcategories;

    QList<QLocale> m_categoryLocales;
};

QT_END_NAMESPACE

#endif // QPLACEMANAGERENGINEBetaKarttakuva_H
