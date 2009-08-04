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

#include "mainwindow.h"
#include "clientprocess.h"

#include <QTabWidget>
#include <QX11EmbedContainer>

MainWindow::MainWindow()
: QMainWindow()
, m_tabWidget(0)
{
    m_tabWidget = new QTabWidget(this);
    setCentralWidget(m_tabWidget);
    newTab();
    qDebug("Called newTab");
}

void MainWindow::newTab()
{
    ClientProcess* c = new ClientProcess(this);
    m_clientProcesses << c;
    connect(c, SIGNAL(clientReady(WId)), SLOT(embedWindow(WId)));
    qDebug("Created new ClientProcess");
}

void MainWindow::embedWindow(WId id)
{
    qDebug("Embedding window %d", id);
    QX11EmbedContainer* c = new QX11EmbedContainer(this);
    c->show();
    connect(c, SIGNAL(clientIsEmbedded()), SLOT(clientEmbedded()));
    connect(c, SIGNAL(error(QX11EmbedContainer::Error)), SLOT(error(QX11EmbedContainer::Error)));
    c->embedClient(id);
    m_tabWidget->addTab(c, "Foo");
}

void MainWindow::clientEmbedded()
{
    qDebug("Client embedded!");
}

void MainWindow::error(QX11EmbedContainer::Error error)
{
    qDebug("ERROR!!!! %d", error);
}

