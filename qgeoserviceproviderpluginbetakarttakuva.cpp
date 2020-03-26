#include "qgeoserviceproviderpluginbetakarttakuva.h"
#include "qgeocodingmanagerenginebetakarttakuva.h"
#include "qplacemanagerenginebetakarttakuva.h"

QT_BEGIN_NAMESPACE

QGeoCodingManagerEngine *QGeoServiceProviderFactoryBetaKarttakuva::createGeocodingManagerEngine(
    const QVariantMap &parameters, QGeoServiceProvider::Error *error, QString *errorString) const
{
    return new QGeocodingManagerEngineBetaKarttakuva(parameters, error, errorString);
}

QGeoMappingManagerEngine *QGeoServiceProviderFactoryBetaKarttakuva::createMappingManagerEngine(
    const QVariantMap &parameters, QGeoServiceProvider::Error *error, QString *errorString) const
{

    Q_UNUSED(parameters);
    Q_UNUSED(error);
    Q_UNUSED(errorString);

    return 0;}

QGeoRoutingManagerEngine *QGeoServiceProviderFactoryBetaKarttakuva::createRoutingManagerEngine(
    const QVariantMap &parameters, QGeoServiceProvider::Error *error, QString *errorString) const
{

    Q_UNUSED(parameters);
    Q_UNUSED(error);
    Q_UNUSED(errorString);

    return 0;}

QPlaceManagerEngine *QGeoServiceProviderFactoryBetaKarttakuva::createPlaceManagerEngine(
    const QVariantMap &parameters, QGeoServiceProvider::Error *error, QString *errorString) const
{
return new QPlaceManagerEngineBetaKarttakuva(parameters, error, errorString);

}

QT_END_NAMESPACE
