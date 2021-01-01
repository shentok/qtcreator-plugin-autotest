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

#include "cargotestconfiguration.h"
#include "cargotestconstants.h"
#include "cargotestoutputreader.h"
#include "cargotestsettings.h"
#include "cargotest_utils.h"
#include "../autotestplugin.h"
#include "../itestframework.h"
#include "../testsettings.h"

#include <utils/stringutils.h>

namespace Autotest {
namespace Internal {

TestOutputReader *CargoTestConfiguration::outputReader(const QFutureInterface<TestResultPtr> &fi,
                                                    QProcess *app) const
{
    auto qtSettings = dynamic_cast<CargoTestSettings *>(framework()->frameworkSettings());
    const CargoTestOutputReader::OutputMode mode = qtSettings && qtSettings->useXMLOutput
            ? CargoTestOutputReader::XML
            : CargoTestOutputReader::PlainText;
    return new CargoTestOutputReader(fi, app, buildDirectory(), projectFile(), mode, TestType::QtTest);
}

QStringList CargoTestConfiguration::argumentsForTestRunner(QStringList *omitted) const
{
    QStringList arguments;
    if (AutotestPlugin::settings()->processArgs) {
        arguments.append(CargoTestUtils::filterInterfering(
                             runnable().commandLineArguments.split(' ', Qt::SkipEmptyParts),
                             omitted, false));
    }
    auto qtSettings = dynamic_cast<CargoTestSettings *>(framework()->frameworkSettings());
    if (!qtSettings)
        return arguments;
    if (qtSettings->useXMLOutput)
        arguments << "-xml";
    if (!testCases().isEmpty())
        arguments << testCases();

    const QString &metricsOption = CargoTestSettings::metricsTypeToOption(qtSettings->metrics);
    if (!metricsOption.isEmpty())
        arguments << metricsOption;

    if (qtSettings->verboseBench)
        arguments << "-vb";

    if (qtSettings->logSignalsSlots)
        arguments << "-vs";

    if (isDebugRunMode() && qtSettings->noCrashHandler)
        arguments << "-nocrashhandler";

    return arguments;
}

Utils::Environment CargoTestConfiguration::filteredEnvironment(const Utils::Environment &original) const
{
    return CargoTestUtils::prepareBasicEnvironment(original);
}

} // namespace Internal
} // namespace Autotest
