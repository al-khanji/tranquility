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

#include "client.h"
#include "frame.h"

#include <QX11Info>
#include <QMouseEvent>

#include <X11/Xlib.h>

Client::Client(WId win, Application* app)
: m_window(win)
, m_frame(0)
, m_app(app)
{
    m_frame = new Frame(this);
    m_frame->installEventFilter(this);

    XReparentWindow(QX11Info::display(), m_window, m_frame->winId(), 0, 0);
    XSelectInput(QX11Info::display(), m_frame->winId(), KeyPressMask
                                                      | KeyReleaseMask
                                                      | ButtonPressMask
                                                      | ButtonReleaseMask
                                                      | KeymapStateMask
                                                      | ButtonMotionMask
                                                      | PointerMotionMask
                                                      | EnterWindowMask
                                                      | LeaveWindowMask
                                                      | FocusChangeMask
                                                      | VisibilityChangeMask
                                                      | ExposureMask
                                                      | StructureNotifyMask
                                                      | SubstructureRedirectMask
                                                      | SubstructureNotifyMask);

    XWindowAttributes wa;
    if (XGetWindowAttributes(QX11Info::display(), m_window, &wa)) {
        m_frame->setClientSize(QSize(wa.width, wa.height));
        QRect r = m_frame->clientArea();
        XMoveResizeWindow(QX11Info::display(), m_window, r.x(), r.y(),
                          r.width(), r.height());
    }
}

Client::~Client()
{
    delete m_frame;
    m_frame = 0;
}

void Client::map()
{
    QX11Info x;
    XMapWindow(x.display(), m_frame->winId());
    XMapWindow(x.display(), m_window);
}

bool Client::eventFilter(QObject* watched, QEvent* event)
{
    if (watched != m_frame) {
        // not interested
        return QObject::eventFilter(watched, event);
    }

    if (event->type() == QEvent::MouseButtonRelease) {
        QMouseEvent* e = static_cast<QMouseEvent*>(event);
        if (e->button() == Qt::MidButton) {
//             stackLower();
            qDebug("MIDBUTTON PRESSED!");
        }
    }

    return QObject::eventFilter(watched, event);
}

#include "client.moc"
