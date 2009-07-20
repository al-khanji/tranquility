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

#include <QX11Info>
#include <QMouseEvent>

#include "client.h"
#include "frame.h"

Client::Client(WId win, Application* app)
: m_window(win)
, m_frame(0)
, m_app(app)
{
    m_frame = new Frame(this);
    m_frame->installEventFilter(this);

    QX11Info x;
    XWindowAttributes wa;

    XGetWindowAttributes(x.display(), m_window, &wa);
    XSetWindowBorderWidth(x.display(), m_window, 0);
    XSetWindowBorderWidth(x.display(), m_frame->winId(), 0);
    XAddToSaveSet(x.display(), m_window);
    XReparentWindow(x.display(), m_window, m_frame->winId(), 0, 0);
    XSelectInput(x.display(), m_window, FocusChangeMask
                                        | PropertyChangeMask
                                        | StructureNotifyMask);

    m_frame->setClientSize(QSize(wa.width, wa.height));
    resizeClient();
    QPoint target(wa.x, wa.y);
    target -= m_frame->clientArea().topLeft();
    if (target.x() < 0) {
        target.setX(0);
    }
    if (target.y() < 0) {
        target.setY(0);
    }
    XMoveWindow(x.display(), m_frame->winId(), target.x(), target.y());
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

void Client::configure(XConfigureRequestEvent* e)
{
    QRect r = m_frame->clientArea();
    r.translate(m_frame->pos());

    if (e->value_mask & CWX) {
        r.setX(e->x);
    }
    if (e->value_mask & CWY) {
        r.setX(e->y);
    }
    if (e->value_mask & CWWidth) {
        r.setWidth(e->width);
    }
    if (e->value_mask & CWHeight) {
        r.setHeight(e->height);
    }

    if (e->value_mask & (CWWidth | CWHeight)) {
        m_frame->setClientSize(r.size());
        resizeClient();
    }

    if (e->value_mask & (CWX | CWY)) {
        QPoint target = r.topLeft();
        target -= m_frame->clientArea().topLeft();
        m_frame->move(target);
//         XMoveWindow(QX11Info::display(), m_frame->winId(),
//                     target.x(), target.y());
    }

    XConfigureEvent ce;
    ce.type = ConfigureNotify;
    ce.event = m_window;
    ce.window = m_window;
    ce.x = r.x();
    ce.y = r.y();
    ce.width = r.width();
    ce.height = r.height();
    ce.above = None;
    ce.border_width = ce.override_redirect = 0;
    XSendEvent(QX11Info::display(), m_window, False, StructureNotifyMask,
               reinterpret_cast<XEvent*>(&ce));
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

void Client::resizeClient()
{
    QRect r = m_frame->clientArea();
    XMoveResizeWindow(QX11Info::display(), m_window, r.x(), r.y(),
                      r.width(), r.height());
}

#include "client.moc"
