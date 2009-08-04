#ifndef SOL_CLIENT_WIDGET_H
#define SOL_CLIENT_WIDGET_H

#include <QX11EmbedWidget>
#include "ui_clientwidget.h"

class ClientWindow : public QX11EmbedWidget, private Ui_ClientWidget
{
    Q_OBJECT
    public:
        ClientWindow();
};

#endif // SOL_CLIENT_WIDGET_H

