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
#include "Params.h"
#include "ScopedIncDec.h"
#include <cassert>

namespace select_content
{

OptionsWidget::OptionsWidget(
	IntrusivePtr<Settings> const& settings,
	PageSelectionAccessor const& page_selection_accessor)
:	m_ptrSettings(settings),
	m_pageSelectionAccessor(page_selection_accessor),
	m_ignoreAutoManualToggle(0)
{
	setupUi(this);
	
	connect(autoBtn, SIGNAL(toggled(bool)), this, SLOT(modeChanged(bool)));
	connect(imageBtn, SIGNAL(toggled(bool)), this, SLOT(modeChanged(bool)));
	connect(pageBtn, SIGNAL(toggled(bool)), this, SLOT(modeChanged(bool)));
	connect(fineTunePage, SIGNAL(clicked(bool)), this, SLOT(fineTunePageToggled(bool)));
	connect(applyToBtn, SIGNAL(clicked()), this, SLOT(showApplyToDialog()));
}

OptionsWidget::~OptionsWidget()
{
}

void
OptionsWidget::preUpdateUI(PageId const& page_id)
{
	ScopedIncDec<int> guard(m_ignoreAutoManualToggle);
	
	m_pageId = page_id;
	autoBtn->setChecked(true);
	autoBtn->setEnabled(false);
	manualBtn->setEnabled(false);
	imageBtn->setEnabled(false);
	pageBtn->setEnabled(false);
	fineTunePage->setEnabled(false);
}

void
OptionsWidget::postUpdateUI(Params const& params)
{
	m_params = params;
	updateModeIndication(params.mode());
	autoBtn->setEnabled(true);
	manualBtn->setEnabled(true);
	imageBtn->setEnabled(true);
	pageBtn->setEnabled(true);
}

void
OptionsWidget::manualContentBoxSet(
	ContentBox const& content_box, QSizeF const& content_size_px)
{
	assert(m_params);

	m_params->setContentBox(content_box);
	m_params->setContentSizePx(content_size_px);
	m_params->setMode(MODE_MANUAL);
	updateModeIndication(MODE_MANUAL);
	commitCurrentParams();
	
	emit invalidateThumbnail(m_pageId);
}

void
OptionsWidget::imageContentBoxSet(
	ContentBox const& content_box, QSizeF const& content_size_px)
{
	assert(m_params);

	m_params->setContentBox(content_box);
	m_params->setContentSizePx(content_size_px);
	m_params->setMode(MODE_IMAGE);
	updateModeIndication(MODE_IMAGE);
	commitCurrentParams();

	emit reloadRequested();
}

void
OptionsWidget::pageContentBoxSet(
	ContentBox const& content_box, QSizeF const& content_size_px)
{
	assert(m_params);

	m_params->setContentBox(content_box);
	m_params->setContentSizePx(content_size_px);
	m_params->setMode(MODE_PAGE);
	updateModeIndication(MODE_PAGE);
	commitCurrentParams();

	emit reloadRequested();
}

void
OptionsWidget::modeChanged(bool const mode)
{
	if (m_ignoreAutoManualToggle) {
		return;
	}
	
	if (mode == MODE_AUTO || mode == MODE_IMAGE) {
		m_params.reset();
		m_ptrSettings->clearPageParams(m_pageId);
		fineTunePage->setEnabled(false);
		emit reloadRequested();
		fineTunePage->setEnabled(false);
	} else if (mode == MODE_PAGE) {
		m_params.reset();
		m_ptrSettings->clearPageParams(m_pageId);
		fineTunePage->setEnabled(true);
		emit reloadRequested();
	} else {
		//manual mode
		assert(m_params);
		m_params->setMode(MODE_MANUAL);
		fineTunePage->setEnabled(false);
		commitCurrentParams();
	}
}

void
OptionsWidget::fineTunePageToggled(bool const tuning_mode)
{
	m_params.reset();
	m_ptrSettings->clearPageParams(m_pageId);
	fineTunePage->setChecked(tuning_mode);
	emit reloadRequested();
}


void
OptionsWidget::updateModeIndication(AutoManualMode const mode)
{
	ScopedIncDec<int> guard(m_ignoreAutoManualToggle);
	
	if (mode == MODE_AUTO) {
		autoBtn->setChecked(true);
	} else if (mode == MODE_PAGE) {
		pageBtn->setChecked(true);
	} else if (mode == MODE_IMAGE) {
		imageBtn->setChecked(true);
	} else {
		//manual mode
		manualBtn->setChecked(true);
	}
}

void
OptionsWidget::commitCurrentParams()
{
	assert(m_params);
	m_ptrSettings->setPageParams(m_pageId, *m_params);
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
		m_ptrSettings->setPageParams(page_id, *m_params);
		emit invalidateThumbnail(page_id);
	}
}

} // namespace select_content
