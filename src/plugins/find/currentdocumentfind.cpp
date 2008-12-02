/***************************************************************************
**
** This file is part of Qt Creator
**
** Copyright (c) 2008 Nokia Corporation and/or its subsidiary(-ies).
**
** Contact:  Qt Software Information (qt-info@nokia.com)
**
**
** Non-Open Source Usage
**
** Licensees may use this file in accordance with the Qt Beta Version
** License Agreement, Agreement version 2.2 provided with the Software or,
** alternatively, in accordance with the terms contained in a written
** agreement between you and Nokia.
**
** GNU General Public License Usage
**
** Alternatively, this file may be used under the terms of the GNU General
** Public License versions 2.0 or 3.0 as published by the Free Software
** Foundation and appearing in the file LICENSE.GPL included in the packaging
** of this file.  Please review the following information to ensure GNU
** General Public Licensing requirements will be met:
**
** http://www.fsf.org/licensing/licenses/info/GPLv2.html and
** http://www.gnu.org/copyleft/gpl.html.
**
** In addition, as a special exception, Nokia gives you certain additional
** rights. These rights are described in the Nokia Qt GPL Exception
** version 1.2, included in the file GPL_EXCEPTION.txt in this package.
**
***************************************************************************/

#include "currentdocumentfind.h"

#include <aggregation/aggregate.h>
#include <coreplugin/coreconstants.h>
#include <coreplugin/modemanager.h>

#include <QtGui/QApplication>
#include <QtDebug>

using namespace Core;
using namespace Find;
using namespace Find::Internal;

CurrentDocumentFind::CurrentDocumentFind(ICore *core)
        : m_core(core), m_currentFind(0)
{
    connect(qApp, SIGNAL(focusChanged(QWidget*, QWidget*)),
            this, SLOT(updateCurrentFindFilter(QWidget*,QWidget*)));
}

void CurrentDocumentFind::removeConnections()
{
    disconnect(qApp, 0, this, 0);
    removeFindSupportConnections();
}

void CurrentDocumentFind::resetIncrementalSearch()
{
    Q_ASSERT(m_currentFind);
    if (m_currentFind)
        m_currentFind->resetIncrementalSearch();
}

void CurrentDocumentFind::clearResults()
{
    Q_ASSERT(m_currentFind);
    if (m_currentFind)
        m_currentFind->clearResults();
}

bool CurrentDocumentFind::isEnabled() const
{
    return m_currentFind && (!m_currentWidget || m_currentWidget->isVisible());
}

bool CurrentDocumentFind::supportsReplace() const
{
    Q_ASSERT(m_currentFind);
    return m_currentFind ? m_currentFind->supportsReplace() : false;
}

QString CurrentDocumentFind::currentFindString() const
{
    Q_ASSERT(m_currentFind);
    return m_currentFind ? m_currentFind->currentFindString() : QString();
}

QString CurrentDocumentFind::completedFindString() const
{
    Q_ASSERT(m_currentFind);
    return m_currentFind ? m_currentFind->completedFindString() : QString();
}

void CurrentDocumentFind::highlightAll(const QString &txt, QTextDocument::FindFlags findFlags)
{
    Q_ASSERT(m_currentFind);
    if (m_currentFind)
        m_currentFind->highlightAll(txt, findFlags);
}

bool CurrentDocumentFind::findIncremental(const QString &txt, QTextDocument::FindFlags findFlags)
{
    Q_ASSERT(m_currentFind);
    return (m_currentFind? m_currentFind->findIncremental(txt, findFlags) : false);
}

bool CurrentDocumentFind::findStep(const QString &txt, QTextDocument::FindFlags findFlags)
{
    Q_ASSERT(m_currentFind);
    return (m_currentFind? m_currentFind->findStep(txt, findFlags) : false);
}

bool CurrentDocumentFind::replaceStep(const QString &before, const QString &after,
    QTextDocument::FindFlags findFlags)
{
    Q_ASSERT(m_currentFind);
    return (m_currentFind? m_currentFind->replaceStep(before, after, findFlags) : false);
}

int CurrentDocumentFind::replaceAll(const QString &before, const QString &after,
    QTextDocument::FindFlags findFlags)
{
    Q_ASSERT(m_currentFind);
    return (m_currentFind? m_currentFind->replaceAll(before, after, findFlags) : 0);
}

void CurrentDocumentFind::defineFindScope()
{
    Q_ASSERT(m_currentFind);
    if (m_currentFind)
        m_currentFind->defineFindScope();
}

void CurrentDocumentFind::clearFindScope()
{
    Q_ASSERT(m_currentFind);
    if (m_currentFind)
        m_currentFind->clearFindScope();
}

void CurrentDocumentFind::updateCurrentFindFilter(QWidget *old, QWidget *now)
{
    Q_UNUSED(old);
    QWidget *candidate = now;
    QPointer<IFindSupport> impl = 0;
    while (!impl && candidate) {
        impl = Aggregation::query<IFindSupport>(candidate);
        if (!impl)
            candidate = candidate->parentWidget();
    }
    if (!impl)
        return;
    removeFindSupportConnections();
    m_currentWidget = candidate;
    m_currentFind = impl;
    if (m_currentFind) {
        connect(m_currentFind, SIGNAL(changed()), this, SIGNAL(changed()));
        connect(m_currentFind, SIGNAL(destroyed(QObject*)), SLOT(findSupportDestroyed()));
    }
    if (m_currentWidget)
        m_currentWidget->installEventFilter(this);
    emit changed();
}

void CurrentDocumentFind::removeFindSupportConnections()
{
    if (m_currentFind) {
        disconnect(m_currentFind, SIGNAL(changed()), this, SIGNAL(changed()));
        disconnect(m_currentFind, SIGNAL(destroyed(QObject*)), this, SLOT(findSupportDestroyed()));
    }
    if (m_currentWidget)
        m_currentWidget->removeEventFilter(this);
}

void CurrentDocumentFind::findSupportDestroyed()
{
    removeFindSupportConnections();
    m_currentWidget = 0;
    m_currentFind = 0;
    emit changed();
}

bool CurrentDocumentFind::setFocusToCurrentFindSupport()
{
    if (m_currentFind && m_currentWidget) {
        m_currentWidget->setFocus();
        return true;
    }
    return false;
}

bool CurrentDocumentFind::eventFilter(QObject *obj, QEvent *event)
{
    if (m_currentWidget && obj == m_currentWidget) {
        if (event->type() == QEvent::Hide || event->type() == QEvent::Show) {
            emit changed();
        }
    }
    return QObject::eventFilter(obj, event);
}
