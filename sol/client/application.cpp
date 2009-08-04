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

#include <iostream>
#include <application.h>
#include "clientwindow.h"

Application::Application(int& argc, char** argv)
: QApplication(argc, argv)
, m_clientWindow(0)
{
    setAttribute(Qt::AA_ImmediateWidgetCreation);
    setAttribute(Qt::AA_NativeWindows);
}

bool Application::parseArgs(WId& id)
{
    QStringList args = arguments();
    if (args.count() == 3) {
        if (args[1] == "-into") {
            bool ok = false;
            id = args[2].toInt(&ok);
            return ok;
        }
    }

    usage();
    return false;
}

void Application::usage()
{
    std::cout << "Usage: " << qPrintable(arguments().at(0)) << " -into <XID>" << std::endl;
}

bool Application::createClientWindow()
{
    m_clientWindow = new ClientWindow;
    connect(m_clientWindow, SIGNAL(containerClosed()), SLOT(quit()));
    connect(m_clientWindow, SIGNAL(error(QX11EmbedWidget::Error)),
            SLOT(embedError(QX11EmbedWidget::Error)));
    WId id;
    bool ok;
    if (parseArgs(id)) {
        m_clientWindow->embedInto(id);
        m_clientWindow->setVisible(true);
        return true;
    } else {
        return false;
    }
}

void Application::embedError(QX11EmbedWidget::Error err)
{
    switch (err) {
        case QX11EmbedWidget::InvalidWindowID:
            std::cout << "Invalid window id passed" << std::endl;
            break;
        case QX11EmbedWidget::Unknown:
            std::cout << "Fatal error inside Qt" << std::endl;
            break;
    }
    ::exit(1);
}

