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

#ifndef SELECT_CONTENT_OPTIONSWIDGET_H_
#define SELECT_CONTENT_OPTIONSWIDGET_H_

#include "ui_SelectContentOptionsWidget.h"
#include "FilterOptionsWidget.h"
#include "DetectionMode.h"
#include "IntrusivePtr.h"
#include "ContentBox.h"
#include "PageId.h"
#include "Params.h"
#include "Dependencies.h"
#include "PageSelectionAccessor.h"
#include <boost/optional.hpp>
#include <QSizeF>
#include <QRectF>
#include <memory>

namespace select_content
{

class DetectionMode;
class Settings;

class OptionsWidget :
	public FilterOptionsWidget, private Ui::SelectContentOptionsWidget
{
	Q_OBJECT
public:
	OptionsWidget(IntrusivePtr<Settings> const& settings,
		PageSelectionAccessor const& page_selection_accessor);
	
	virtual ~OptionsWidget();
	
	void preUpdateUI(PageId const& page_id,
		DetectionMode const& detection_mode = DetectionMode::CONTENT);
	
	void postUpdateUI(Params const& params);
public slots:
	void manualContentBoxSetExternally(
		ContentBox const& content_box, QSizeF const& content_size_px);
	
	void manualDetectionModeSetExternally(
		select_content::DetectionMode const& mode);
private slots:
	void manualModeToggled(bool checked);
	
	void contentModeToggled(bool checked);

	void pageModeToggled(bool checked);

	void imageModeToggled(bool checked);

	void showApplyToDialog();

	void applySelection(std::set<PageId> const& pages);

	void fineTunePageToggled(bool checked);
private:
	void setupDetectionModeButtons();

	void setupUiForDetectionMode(DetectionMode::Mode mode);
	
	Ui::SelectContentOptionsWidget ui;
	IntrusivePtr<Settings> m_ptrSettings;
	PageId m_pageId;

	/**
	* m_pageParams is not always up to date. We make sure not to commit
	* it to m_ptrSettings between preUpdateUI() and postUpdateUI(),
	* where it's certainly not up to date.
	*/
	Params m_pageParams;

//	boost::optional<Params> m_params;
	QAbstractButton* m_detectionModeButtons[DetectionMode::LAST + 1];
	int m_ignoreSignalsFromUiControls;

	PageSelectionAccessor m_pageSelectionAccessor;
};

} // namespace select_content

#endif
