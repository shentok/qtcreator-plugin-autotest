/****************************************************************************
**
** Copyright (C) 2019 The Qt Company Ltd.
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

#include "../itestparser.h"
#include "boosttestframework.h"
#include "boosttesttreeitem.h"

namespace Autotest {
namespace Internal {

class BoostTestParseResult : public TestParseResult
{
public:
    explicit BoostTestParseResult(BoostTestFramework *framework) : TestParseResult(), m_framework(framework) {}
    TestTreeItem *createTestTreeItem() const override;
    ITestFramework *framework() const override { return m_framework; }
    // TODO special attributes/states (labeled, timeout,...?)
    BoostTestTreeItem::TestStates state = BoostTestTreeItem::Enabled;
private:
    BoostTestFramework *m_framework;
};

class BoostTestParser : public CppParser
{
public:
    explicit BoostTestParser(BoostTestFramework *framework) : CppParser(), m_framework(framework) {}
    bool processDocument(QFutureInterface<TestParseResultPtr> futureInterface,
                         const QString &fileName) override;
    ITestFramework *framework() const override { return m_framework; }
private:
    BoostTestFramework *m_framework;
};

} // namespace Internal
} // namespace Autotest
