#ifndef QTBETAKARTTAKUVAPLUGIN_PLUGIN_H
#define QTBETAKARTTAKUVAPLUGIN_PLUGIN_H

#include <QQmlExtensionPlugin>

class QtBetaKarttakuvaPluginPlugin : public QQmlExtensionPlugin
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID QQmlExtensionInterface_iid)

public:
    void registerTypes(const char *uri) override;
};

#endif // QTBETAKARTTAKUVAPLUGIN_PLUGIN_H
