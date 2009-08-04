#ifndef SOL_CLIENT_APPLICATION_H
#define SOL_CLIENT_APPLICATION_H

#include <QApplication>
#include <QX11EmbedWidget>

class ClientWindow;

class Application : public QApplication
{
    Q_OBJECT
    public:
        Application(int& argc, char** argv);
        bool parseArgs(WId& id);
        void usage();
        bool createClientWindow();

    public slots:
        void embedError(QX11EmbedWidget::Error err);

    private:
        ClientWindow* m_clientWindow;
};

#endif // SOL_CLIENT_APPLICATION_H

