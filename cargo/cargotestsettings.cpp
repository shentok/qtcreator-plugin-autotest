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

#include "cargotestsettings.h"

namespace Autotest {
namespace Internal {

static const char metricsKey[]           = "Metrics";
static const char noCrashhandlerKey[]    = "NoCrashhandlerOnDebug";
static const char useXMLOutputKey[]      = "UseXMLOutput";
static const char verboseBenchKey[]      = "VerboseBench";
static const char logSignalsSlotsKey[]   = "LogSignalsSlots";

static CargoTestSettings::MetricsType intToMetrics(int value)
{
    switch (value) {
    case CargoTestSettings::Walltime:
        return CargoTestSettings::Walltime;
    case CargoTestSettings::TickCounter:
        return CargoTestSettings::TickCounter;
    case CargoTestSettings::EventCounter:
        return CargoTestSettings::EventCounter;
    case CargoTestSettings::CallGrind:
        return CargoTestSettings::CallGrind;
    case CargoTestSettings::Perf:
        return CargoTestSettings::Perf;
    default:
        return CargoTestSettings::Walltime;
    }
}

QString CargoTestSettings::name() const
{
    return QString("cargotest");
}

void CargoTestSettings::fromFrameworkSettings(const QSettings *s)
{
    metrics = intToMetrics(s->value(metricsKey, Walltime).toInt());
    noCrashHandler = s->value(noCrashhandlerKey, true).toBool();
    useXMLOutput = s->value(useXMLOutputKey, true).toBool();
    verboseBench = s->value(verboseBenchKey, false).toBool();
    logSignalsSlots = s->value(logSignalsSlotsKey, false).toBool();
}

void CargoTestSettings::toFrameworkSettings(QSettings *s) const
{
    s->setValue(metricsKey, metrics);
    s->setValue(noCrashhandlerKey, noCrashHandler);
    s->setValue(useXMLOutputKey, useXMLOutput);
    s->setValue(verboseBenchKey, verboseBench);
    s->setValue(logSignalsSlotsKey, logSignalsSlots);
}

QString CargoTestSettings::metricsTypeToOption(const MetricsType type)
{
    switch (type) {
    case MetricsType::Walltime:
        return QString();
    case MetricsType::TickCounter:
        return QString("-tickcounter");
    case MetricsType::EventCounter:
        return QString("-eventcounter");
    case MetricsType::CallGrind:
        return QString("-callgrind");
    case MetricsType::Perf:
        return QString("-perf");
    }
    return QString();
}

} // namespace Internal
} // namespace Autotest
