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

#include "../autotestconstants.h"
#include "cargotestconstants.h"
#include "cargotestsettingspage.h"
#include "cargotestsettings.h"
#include "ui_qttestsettingspage.h"

#include <coreplugin/icore.h>

#include <utils/hostosinfo.h>

namespace Autotest {
namespace Internal {

class QtTestSettingsWidget final : public Core::IOptionsPageWidget
{
    Q_DECLARE_TR_FUNCTIONS(Autotest::Internal::QtTestSettingsWidget)

public:
    explicit QtTestSettingsWidget(CargoTestSettings *settings);

    void apply() final;

private:
    Ui::QtTestSettingsPage m_ui;
    CargoTestSettings *m_settings;
};

QtTestSettingsWidget::QtTestSettingsWidget(CargoTestSettings *settings)
    : m_settings(settings)
{
    m_ui.setupUi(this);
    m_ui.callgrindRB->setEnabled(Utils::HostOsInfo::isAnyUnixHost()); // valgrind available on UNIX
    m_ui.perfRB->setEnabled(Utils::HostOsInfo::isLinuxHost()); // according to docs perf Linux only

    m_ui.disableCrashhandlerCB->setChecked(m_settings->noCrashHandler);
    m_ui.useXMLOutputCB->setChecked(m_settings->useXMLOutput);
    m_ui.verboseBenchmarksCB->setChecked(m_settings->verboseBench);
    m_ui.logSignalsAndSlotsCB->setChecked(m_settings->logSignalsSlots);
    switch (m_settings->metrics) {
    case CargoTestSettings::MetricsType::Walltime:
        m_ui.walltimeRB->setChecked(true);
        break;
    case CargoTestSettings::MetricsType::TickCounter:
        m_ui.tickcounterRB->setChecked(true);
        break;
    case CargoTestSettings::MetricsType::EventCounter:
        m_ui.eventCounterRB->setChecked(true);
        break;
    case CargoTestSettings::MetricsType::CallGrind:
        m_ui.callgrindRB->setChecked(true);
        break;
    case CargoTestSettings::MetricsType::Perf:
        m_ui.perfRB->setChecked(true);
        break;
    }
}

void QtTestSettingsWidget::apply()
{
    m_settings->noCrashHandler = m_ui.disableCrashhandlerCB->isChecked();
    m_settings->useXMLOutput = m_ui.useXMLOutputCB->isChecked();
    m_settings->verboseBench = m_ui.verboseBenchmarksCB->isChecked();
    m_settings->logSignalsSlots = m_ui.logSignalsAndSlotsCB->isChecked();
    if (m_ui.walltimeRB->isChecked())
        m_settings->metrics = CargoTestSettings::MetricsType::Walltime;
    else if (m_ui.tickcounterRB->isChecked())
        m_settings->metrics = CargoTestSettings::MetricsType::TickCounter;
    else if (m_ui.eventCounterRB->isChecked())
        m_settings->metrics = CargoTestSettings::MetricsType::EventCounter;
    else if (m_ui.callgrindRB->isChecked())
        m_settings->metrics = CargoTestSettings::MetricsType::CallGrind;
    else if (m_ui.perfRB->isChecked())
        m_settings->metrics = CargoTestSettings::MetricsType::Perf;

    m_settings->toSettings(Core::ICore::settings());
}

CargoTestSettingsPage::CargoTestSettingsPage(CargoTestSettings *settings, Utils::Id settingsId)
{
    setId(settingsId);
    setCategory(Constants::AUTOTEST_SETTINGS_CATEGORY);
    setDisplayName(QCoreApplication::translate("cargotestFramework",
                                               QtTest::Constants::FRAMEWORK_SETTINGS_CATEGORY));
    setWidgetCreator([settings] { return new QtTestSettingsWidget(settings); });
}

} // namespace Internal
} // namespace Autotest
