TEMPLATE = lib
TARGET = qtbetakarttakuva_plugin
QT += location-private positioning-private network concurrent

QT_FOR_CONFIG += location-private
qtConfig(location-labs-plugin): DEFINES += LOCATIONLABS

# Input
SOURCES += \
    qgeoserviceproviderpluginbetakarttakuva.cpp \
    qgeocodingmanagerenginebetakarttakuva.cpp \
    qgeocodereplybetakarttakuva.cpp \
    qplacemanagerenginebetakarttakuva.cpp \
    qplacesearchreplybetakarttakuva.cpp

HEADERS += \
    qgeoserviceproviderpluginbetakarttakuva.h \
    qgeocodingmanagerenginebetakarttakuva.h \
    qgeocodereplybetakarttakuva.h \
    qplacemanagerenginebetakarttakuva.h \
    qplacesearchreplybetakarttakuva.h

OTHER_FILES += \
    betakarttakuva_plugin.json

PLUGIN_TYPE = geoservices
PLUGIN_CLASS_NAME = QGeoServiceProviderFactoryBetaKarttakuva
load(qt_plugin)
