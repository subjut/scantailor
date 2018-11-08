/*
    Scan Tailor - Interactive post-processing tool for scanned pages.
    Copyright (C) 2015  Joseph Artsimovich <joseph.artsimovich@gmail.com>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#include "OptionsWidget.h"
#include "ApplyDialog.h"
#include "Settings.h"
#include "DetectionMode.h"
#include "ScopedIncDec.h"
#include <cassert>

namespace select_content
{

OptionsWidget::OptionsWidget(
	IntrusivePtr<Settings> const& settings,
	PageSelectionAccessor const& page_selection_accessor)
:	m_ptrSettings(settings),
	m_pageParams(Dependencies()),
	m_pageSelectionAccessor(page_selection_accessor),
	m_ignoreSignalsFromUiControls(0)
{
	using namespace select_content;

	ui.setupUi(this);
	setupDetectionModeButtons();

	connect(ui.manualModeBtn, SIGNAL(toggled(bool)), this, SLOT(manualModeToggled(bool)));
	connect(ui.contentModeBtn, SIGNAL(toggled(bool)), this, SLOT(contentModeToggled(bool)));
	connect(ui.pageModeBtn, SIGNAL(toggled(bool)), this, SLOT(pageModeToggled(bool)));
	connect(ui.imageModeBtn, SIGNAL(toggled(bool)), this, SLOT(imageModeToggled(bool)));
	
	connect(ui.fineTunePageCB, SIGNAL(clicked(bool)), SLOT(fineTunePageToggled(bool)));
	
	connect(ui.applyToBtn, SIGNAL(clicked()), this, SLOT(showApplyToDialog()));
}

OptionsWidget::~OptionsWidget()
{
}

void
OptionsWidget::preUpdateUI(PageId const& page_id,
	DetectionMode const& detection_mode)
{
	ScopedIncDec<int> guard(m_ignoreSignalsFromUiControls);
	
	m_pageId = page_id;
	m_pageParams.setDetectionMode(detection_mode);

	setupUiForDetectionMode(detection_mode);
	ui.fineTunePageCB->setEnabled(false);
}

void
OptionsWidget::postUpdateUI(Params const& params)
{
	ScopedIncDec<int> guard(m_ignoreSignalsFromUiControls);

	m_pageParams = params;

	setupUiForDetectionMode(params.detectionMode());

	if (params.detectionMode() == DetectionMode::PAGE) {
		ui.fineTunePageCB->setChecked(params.isFineTuningEnabled());
	}
}

void
OptionsWidget::manualContentBoxSetExternally(
	ContentBox const& content_box, QSizeF const& content_size_px)
{
	m_pageParams.setContentBox(content_box);
	m_pageParams.setContentSizePx(content_size_px);
	m_pageParams.setDetectionMode(DetectionMode::MANUAL);
	ui.manualModeBtn->setChecked(true);
	m_ptrSettings->setPageParams(m_pageId, m_pageParams);
	emit invalidateThumbnail(m_pageId);
}


void
OptionsWidget::manualModeToggled(bool checked)
{
	if (!checked || m_ignoreSignalsFromUiControls) {
		return;
	}

	m_pageParams.setDetectionMode(DetectionMode::MANUAL);
	m_ptrSettings->setPageParams(m_pageId, m_pageParams);
	setupUiForDetectionMode(DetectionMode::MANUAL);
	
	emit invalidateThumbnail(m_pageId);
}


void
OptionsWidget::contentModeToggled(bool checked)
{
	
	if (!checked || m_ignoreSignalsFromUiControls) {
		return;
	}

	m_pageParams.setDetectionMode(DetectionMode::CONTENT);
	m_ptrSettings->setPageParams(m_pageId, m_pageParams);
	setupUiForDetectionMode(DetectionMode::CONTENT);

	emit reloadRequested();
}


void
OptionsWidget::pageModeToggled(bool checked)
{
	if (!checked || m_ignoreSignalsFromUiControls) {
		return;
	}

	m_pageParams.setDetectionMode(DetectionMode::PAGE);
	m_ptrSettings->setPageParams(m_pageId, m_pageParams);
	setupUiForDetectionMode(DetectionMode::PAGE);

	emit reloadRequested();
}

void
OptionsWidget::imageModeToggled(bool checked)
{
	if (!checked || m_ignoreSignalsFromUiControls) {
		return;
	}

	m_pageParams.setDetectionMode(DetectionMode::IMAGE);
	m_ptrSettings->setPageParams(m_pageId, m_pageParams);
	setupUiForDetectionMode(DetectionMode::IMAGE);

	emit reloadRequested();
}

void
OptionsWidget::fineTunePageToggled(bool checked)
{
	ui.fineTunePageCB->setChecked(checked);
	m_pageParams.setFineTuning(checked);
	m_ptrSettings->setPageParams(m_pageId, m_pageParams);

	emit reloadRequested();
}

void
OptionsWidget::setupDetectionModeButtons()
{
	static_assert(
		DetectionMode::LAST + 1 - DetectionMode::FIRST == 4,
		"Unexpected number of content detection modes"
		);
	
	m_detectionModeButtons[DetectionMode::MANUAL] = ui.manualModeBtn;
	m_detectionModeButtons[DetectionMode::CONTENT] = ui.contentModeBtn;
	m_detectionModeButtons[DetectionMode::PAGE] = ui.pageModeBtn;
	m_detectionModeButtons[DetectionMode::IMAGE] = ui.imageModeBtn;
}


void
OptionsWidget::setupUiForDetectionMode(DetectionMode::Mode mode)
{
	ScopedIncDec<int> guard(m_ignoreSignalsFromUiControls);

	m_detectionModeButtons[mode]->setChecked(true);

	ui.fineTunePageCB->setEnabled(mode == DetectionMode::PAGE);
}

void
OptionsWidget::showApplyToDialog()
{
	ApplyDialog* dialog = new ApplyDialog(
		this, m_pageId, m_pageSelectionAccessor
	);
	dialog->setAttribute(Qt::WA_DeleteOnClose);
	connect(
		dialog, SIGNAL(applySelection(std::set<PageId> const&)),
		this, SLOT(applySelection(std::set<PageId> const&))
	);
	dialog->show();
}

void
OptionsWidget::applySelection(std::set<PageId> const& pages)
{
	if (pages.empty()) {
		return;
	}

	assert(m_params);

	for (PageId const& page_id : pages) {
		m_ptrSettings->setPageParams(page_id, m_pageParams);
		emit invalidateThumbnail(page_id);
	}
}

} // namespace select_content
