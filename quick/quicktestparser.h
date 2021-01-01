/****************************************************************************
**
** Copyright (C) 2016 The Qt Company Ltd.
** Contact: https://www.qt.io/licensing/
**
** This file is part of Qt Creator.
**
** Commercial License Usage
** Licensees holding valid commercial Qt licenses may use this file in
** accordance with the commercial license agreement provided with the
** Software or, alternatively, in accordance with the terms contained in
** a written agreement between you and The Qt Company. For licensing terms
** and conditions see https://www.qt.io/terms-conditions. For further
** information use the contact form at https://www.qt.io/contact-us.
**
** GNU General Public License Usage
** Alternatively, this file may be used under the terms of the GNU
** General Public License version 3 as published by the Free Software
** Foundation with exceptions as appearing in the file LICENSE.GPL3-EXCEPT
** included in the packaging of this file. Please review the following
** information to ensure the GNU General Public License requirements will
** be met: https://www.gnu.org/licenses/gpl-3.0.html.
**
****************************************************************************/

#pragma once

#include "quicktestframework.h"
#include "../itestparser.h"

#include <qmljs/qmljsdocument.h>

#include <QFileSystemWatcher>

namespace Autotest {
namespace Internal {

class QuickTestParseResult : public TestParseResult
{
public:
    explicit QuickTestParseResult(QuickTestFramework *framework) : TestParseResult(), m_framework(framework) {}
    TestTreeItem *createTestTreeItem() const override;
    ITestFramework *framework() const override { return m_framework; }

private:
    QuickTestFramework *m_framework;
};

class QuickTestParser : public QObject, public CppParser
{
    Q_OBJECT
public:
    explicit QuickTestParser(QuickTestFramework *framework);
    void init(const QStringList &filesToParse, bool fullParse) override;
    void release() override;
    bool processDocument(QFutureInterface<TestParseResultPtr> futureInterface,
                         const QString &fileName) override;
    ITestFramework *framework() const override { return m_framework; }
    QString projectFileForMainCppFile(const QString &fileName) const;
private:
    bool handleQtQuickTest(QFutureInterface<TestParseResultPtr> futureInterface,
                           CPlusPlus::Document::Ptr document);
    void handleDirectoryChanged(const QString &directory);
    void doUpdateWatchPaths(const QStringList &directories);
    QString quickTestName(const CPlusPlus::Document::Ptr &doc) const;
    QList<QmlJS::Document::Ptr> scanDirectoryForQuickTestQmlFiles(const QString &srcDir);
    QuickTestFramework *m_framework;
    QmlJS::Snapshot m_qmlSnapshot;
    QHash<QString, QString> m_proFilesForQmlFiles;
    QFileSystemWatcher m_directoryWatcher;
    QMap<QString, QMap<QString, QDateTime> > m_watchedFiles;
    QMap<QString, QString> m_mainCppFiles;
};

} // namespace Internal
} // namespace Autotest
