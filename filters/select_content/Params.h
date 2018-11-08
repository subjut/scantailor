/*
    Scan Tailor - Interactive post-processing tool for scanned pages.
    Copyright (C)  Joseph Artsimovich <joseph.artsimovich@gmail.com>

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

#ifndef SELECT_CONTENT_PARAMS_H_
#define SELECT_CONTENT_PARAMS_H_

#include "Dependencies.h"
#include "DetectionMode.h"
#include "ContentBox.h"
#include <QSizeF>
#include <QRectF>

class QDomDocument;
class QDomElement;
class QString;

namespace select_content
{

class Params
{
	// Member-wise copying is OK.
public:
	/**
	 * @param content_box The content box.
	 * @param content_size_px Used for page sorting. Corresponds to
	 *        content_box.toTransformedRect(transform).size()
	 * @param deps Dependencies are used to decide when we need to
	 *        re-process the image due to changes made in previous stages.
	 * @param mode Tells whether the content box was detected automatically or set manually.
	 * @param contentDetect Tells whether the content box should be detected.
	 * @param pageDetect Tells whether physical page should be detected.
	 * @param mode Tells whether page corners should be fine tuned.
	 */
	Params(ContentBox const& content_box, QSizeF const& content_size_px,
		Dependencies const& deps, DetectionMode mode, bool const fineTuning);
	
	Params(Dependencies const& deps);
	
	Params(QDomElement const& filter_el);
	
	~Params();
	
	static DetectionMode defaultDetectionMode();

	DetectionMode detectionMode() const { return m_mode; }

	void setDetectionMode(DetectionMode mode) { m_mode = mode; }

	ContentBox const& contentBox() const { return m_contentBox; }

	void setContentBox(ContentBox const& content_box) { m_contentBox = content_box; }

	/** Used for page sorting by width / height. */
	QSizeF const& contentSizePx() const { return m_contentSizePx; }

	void setContentSizePx(QSizeF const& size) { m_contentSizePx = size; }
	
	Dependencies const& dependencies() const { return m_deps; }

	void setDependencies(Dependencies const& deps) { m_deps = deps; }
	
	void setFineTuning(bool fine_tune) { m_fineTuneCorners = fine_tune; }

	bool isFineTuningEnabled() const { return m_fineTuneCorners; }

	QDomElement toXml(QDomDocument& doc, QString const& name) const;

	void takeManualSettingsFrom(Params const& other);
private:
	ContentBox m_contentBox;
	QSizeF m_contentSizePx;
	Dependencies m_deps;
	DetectionMode m_mode;
	bool m_fineTuneCorners;
};

} // namespace select_content

#endif
