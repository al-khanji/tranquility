/*
Copyright (c) 2009 Louai Al-Khanji

Permission is hereby granted, free of charge, to any person
obtaining a copy of this software and associated documentation
files (the "Software"), to deal in the Software without
restriction, including without limitation the rights to use,
copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the
Software is furnished to do so, subject to the following
conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES
OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT
HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR
OTHER DEALINGS IN THE SOFTWARE.
*/

#ifndef CLIENT_H
#define CLIENT_H

#include <QWidget>
#include <X11/Xlib.h>

class Frame;
class Application;
class QHBoxLayout;
class QSizeGrip;

class Client : public QObject
{
    Q_OBJECT
    public:
        Client(WId win, Application* app);
        ~Client();
        void map();

        void configure(XConfigureRequestEvent* e);

    protected:
        bool eventFilter(QObject* watched, QEvent* event);

    private:
        WId m_window;
        Frame* m_frame;
        Application* m_app;

        bool m_isMoving;
        QPoint m_mouseMovePosition;

        QHBoxLayout* m_sizeGripLayout;
        QSizeGrip* m_sizeGrip;

        void resizeClient();
        void addSizeGrip();
};

#endif // CLIENT_H
