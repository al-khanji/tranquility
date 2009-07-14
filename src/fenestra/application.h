#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class Application : public QApplication
{
    Q_OBJECT
    public:
        Application(int& argc, char** argv);

    protected:
        virtual bool x11EventFilter(XEvent* event);
};

#endif // APPLICATION_H
