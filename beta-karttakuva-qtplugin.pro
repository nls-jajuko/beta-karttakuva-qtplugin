TEMPLATE = lib
TARGET = qtbetakarttuva_plugin
QT += location-private positioning-private network concurrent

QT_FOR_CONFIG += location-private
qtConfig(location-labs-plugin): DEFINES += LOCATIONLABS

# Input
SOURCES += \
    qgeoserviceproviderpluginbetakarttakuva.cpp \
    qgeocodingmanagerenginebetakarttakuva.cpp \
    qgeocodereplybetakarttakuva.cpp

HEADERS += \
    qgeoserviceproviderpluginbetakarttakuva.h \
    qgeocodingmanagerenginebetakarttakuva.h \
    qgeocodereplybetakarttakuva.h

OTHER_FILES += \
    betakarttakuva_plugin.json

PLUGIN_TYPE = geoservices
PLUGIN_CLASS_NAME = QGeoServiceProviderFactoryBetaKarttakuva
load(qt_plugin)
