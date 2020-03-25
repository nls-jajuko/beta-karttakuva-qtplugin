#include "qtbetakarttakuvaplugin_plugin.h"

#include "qbetakarttakuva.h"

#include <qqml.h>

void QtBetaKarttakuvaPluginPlugin::registerTypes(const char *uri)
{
    // @uri fi.nls.betakarttakuva
    qmlRegisterType<QBetaKarttakuva>(uri, 1, 0, "QBetaKarttakuva");
}

