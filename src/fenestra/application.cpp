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

#include "application.h"
#include "client.h"

#include <QX11Info>

#include <X11/Xproto.h>
#include <X11/extensions/shape.h>
#include <X11/extensions/Xrender.h>
#include <X11/extensions/Xfixes.h>
#include <X11/extensions/Xdamage.h>
#include <X11/extensions/Xrandr.h>
#include <X11/extensions/Xcomposite.h>
#include <X11/extensions/sync.h>
#include <GL/glx.h>


Application::Application(int& argc, char** argv)
: QApplication(argc, argv)
{
    internStdAtoms();
    QMetaObject::invokeMethod(this, "startWindowManagement",
                              Qt::QueuedConnection);
}

Atom Application::atom(AtomId id) const
{
    return m_atoms[id];
}

QPair<int, int> Application::extension(Extension ext) const
{
    return m_extensions[ext];
}

bool Application::x11EventFilter(XEvent* e)
{
    QX11Info x;
    XWindowAttributes wa;
    WId id;

    qDebug("%d", e->type);

    switch(e->type) {
        case MapRequest:
            id = e->xmaprequest.window;
            if (!XGetWindowAttributes(x.display(), id, &wa)) {
                return true;
            }

            if (wa.override_redirect) {
                XMapRaised(x.display(), id);
                return true;
            }

            if (!m_clients.contains(id)) {
                m_clients[id] = new Client(id, this);
            }

            m_clients[id]->map();
            return true;
            break;

        case MapNotify:
            id = e->xmap.window;
            if (!m_clients.contains(id)) {
                m_unmanaged.insert(id);
            }
            return true;
            break;

        case ConfigureRequest:
            id = e->xconfigurerequest.window;
            if (!m_clients.contains(id)) {
                XWindowChanges wc;
                wc.x = e->xconfigurerequest.x;
                wc.y = e->xconfigurerequest.y;
                wc.width = e->xconfigurerequest.width;
                wc.height = e->xconfigurerequest.height;
                e->xconfigurerequest.value_mask &= (CWX|CWY|CWWidth|CWHeight);
                XConfigureWindow(x.display(), id,
                                 e->xconfigurerequest.value_mask, &wc);
            }
            return true;
            break;

        case DestroyNotify:
            id = e->xdestroywindow.window;
            if (m_clients.contains(id)) {
                Client* c = m_clients.take(id);
                delete c;
                return true;
            } else if (m_unmanaged.contains(id)) {
                m_unmanaged.remove(id);
                return true;
            }
            break;

        case FocusIn:
        case FocusOut:
            return true;
            break;
    }

    return false;
}

static bool initialising = false;
static int xErrorHandler(Display* d, XErrorEvent* e)
{
    if (initialising
        && e->error_code == BadAccess
        && (e->request_code == X_ChangeWindowAttributes
            || e->request_code == X_GrabKey)) {
        QString err = QApplication::translate("X Error",
                "Another window manager seems to be running already. Aborting.");
        qCritical(qPrintable(err));
        exit(0);
    }

    if (initialising) {
        QString err = QApplication::translate("X Error",
                                     "Failure during initialisation. Aboring.");
        qCritical(qPrintable(err));
        exit(0);
    }

    return 0;
}

void Application::startWindowManagement()
{
    QX11Info x;
    XSetErrorHandler(xErrorHandler);
    initialising = true;
    XSelectInput(x.display(), x.appRootWindow(), KeyPressMask
                                                 | PropertyChangeMask
                                                 | ColormapChangeMask
                                                 | SubstructureRedirectMask
                                                 | SubstructureNotifyMask
                                                 | FocusChangeMask
                                                 | ExposureMask);
    syncX(); // trap any errors now
    initialising = false;

    queryExtensions();
}

void Application::internStdAtoms()
{
    if (m_atoms.count() > 0) {
        return;
    }

    int atomIdEnum = metaObject()->indexOfEnumerator("AtomId");
    QMetaEnum atomEnum = metaObject()->enumerator(atomIdEnum);
    int count = atomEnum.keyCount();

    QVector<char *> names;
    names.resize(count);

    for (int i = 0; i < count; i++) {
        names[i] = (char *) atomEnum.key(i);
    }

    m_atoms.resize(count);
    m_atoms.squeeze();

    XInternAtoms(QX11Info::display(), names.data(), count, false,
                 m_atoms.data());
}

void Application::queryExtensions()
{
    if (m_extensions.count() > 0) {
        return;
    }

    int enumId = metaObject()->indexOfEnumerator("Extension");
    QMetaEnum extEnum = metaObject()->enumerator(enumId);
    int count = extEnum.keyCount();

    m_extensions.resize(count);
    m_extensions.squeeze();

    for (int i = 0; i < count; i++) {
        const char* name = extEnum.key(i);
        int opcode, event, error;
        if (XQueryExtension(QX11Info::display(), name, &opcode, &event, &error))
        {
            m_extensions[i] = QPair<int, int>(event, error);
        } else {
            qDebug("%s extension unavailable.", name);
            m_extensions[i] = QPair<int, int>(-1, -1);
        }
    }
}

#include "application.moc"
