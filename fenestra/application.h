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

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QVector>
#include <QPair>
#include <QMetaEnum>
#include <QHash>
#include <QSet>

#include <X11/Xlib.h>

class Client;

class Application : public QApplication
{
    Q_OBJECT
    Q_ENUMS(AtomId)
    Q_ENUMS(Extension)
    public:
        Application(int& argc, char** argv);
        enum AtomId {
            WM_PROTOCOLS,
            WM_DELETE_WINDOW,
            WM_CHANGE_STATE,
            WM_STATE,
            WM_TAKE_FOCUS,
            RESOURCE_MANAGER,
            _NET_WM_NAME,
            _NET_WM_ICON,
            _NET_WM_USER_TIME,
            _NET_WM_STATE,
            _NET_SUPPORTED,
            _NET_WM_WINDOW_TYPE,
            _NET_WM_WINDOW_TYPE_NORMAL,
            _NET_WM_WINDOW_TYPE_DESKTOP,
            _NET_WM_WINDOW_TYPE_DIALOG,
            _NET_WM_WINDOW_TYPE_SPLASH,
            _NET_WM_WINDOW_TYPE_DND
        };
        enum Extension {
            SHAPE,
            RANDR,
            DAMAGE,
            Composite,
            XFIXES,
            RENDER,
            GLX,
            SYNC
        };

        Atom atom(AtomId id) const;
        QPair<int, int> extension(Extension) const;

    protected:
        virtual bool x11EventFilter(XEvent* event);

    private slots:
        void startWindowManagement();

    private:
        void internStdAtoms();
        void queryExtensions();
        void manageExistingWindows();

        QVector<Atom> m_atoms;
        QVector<QPair<int, int> > m_extensions;
        QHash<WId, Client*> m_clients;
        QSet<WId> m_unmanaged;
};

#endif // APPLICATION_H
