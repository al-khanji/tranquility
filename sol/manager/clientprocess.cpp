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

#include "clientprocess.h"

#include <QProcess>
#include <QDebug>

static const char* magic = "SOL CLIENT WINDOW ID:";

ClientProcess::ClientProcess(QObject* parent)
: QObject(parent)
{
    m_clientProcess = new QProcess(this);
    connect(m_clientProcess, SIGNAL(readyReadStandardOutput()), SLOT(readStandardOutput()));
    m_clientProcess->start("sol_client");
}

void ClientProcess::readStandardOutput()
{
    m_clientProcess->setReadChannel(QProcess::StandardOutput);
    QByteArray data = m_clientProcess->readLine();
    if (!data.isEmpty()) {
        qDebug() << data;
        QString line = QString::fromLocal8Bit(data.data());
        if (line.startsWith(magic)) {
            bool ok = false;
            int wid = 0;
            line.remove(magic);
            line = line.trimmed();
            wid = line.toInt(&ok);
            if (ok) {
                emit clientReady(wid);
            }
        }
    }
}

