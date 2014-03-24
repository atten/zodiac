#ifndef PLUGIN_INTERFACE
#define PLUGIN_INTERFACE

#include <QString>
#include <QStringList>
#include <QVariant>
//#include <Astroprocessor/Gui>
#include <QIcon>

class AstroFileHandler;

struct Plugin
{
    enum Layout { Layout_Stack, Layout_DefinedByAlign, Layout_Window };

    QString           path;
    QString           name;
    QString           title;
    QString           description;
    QString           currentLocale;
    AstroFileHandler* widget;
    Layout            layout;
    QVariant          layoutParameter;
    bool              loaded;
    bool              loadable;
};

class PluginInterface {
    public:
        virtual QString           translate ()          = 0;
        virtual QString           getTitle()            = 0;
        virtual QString           getInfo()             = 0;
        virtual QIcon             getIcon()             = 0;
        virtual AstroFileHandler* load ( )              = 0;
        virtual Plugin::Layout    getLayout()           = 0;
        virtual QVariant          getLayoutParameter()  = 0;

        virtual ~PluginInterface() {}
};

Q_DECLARE_INTERFACE(PluginInterface, "com.mysoft.QVisualInterface/1.1")

#endif
