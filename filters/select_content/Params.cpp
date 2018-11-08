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

#include "Params.h"
#include "XmlMarshaller.h"
#include "XmlUnmarshaller.h"
#include <QDomDocument>
#include <QDomElement>

namespace select_content
{

Params::Params(
	ContentBox const& content_box, QSizeF const& content_size_px,
	Dependencies const& deps, DetectionMode const mode, bool const fineTuning)
:	m_contentBox(content_box),
	m_contentSizePx(content_size_px),
	m_deps(deps),
	m_mode(defaultDetectionMode()),
	m_fineTuneCorners(fineTuning)
{
}

Params::Params(Dependencies const& deps)
:	m_deps(deps),
	m_mode(defaultDetectionMode()),
	m_fineTuneCorners(false)
{
}

Params::Params(QDomElement const& filter_el)
:	m_mode(filter_el.attribute("detectionMode"))
,	m_contentBox(filter_el.namedItem("content-box").toElement())
,	m_contentSizePx(
		XmlUnmarshaller::sizeF(
			filter_el.namedItem("content-size-px").toElement()
		)
	)
,	m_fineTuneCorners(filter_el.attribute("fine-tune-corners") == "true" ? true : false)
,	m_deps(filter_el.namedItem("dependencies").toElement())
{
}

Params::~Params()
{
}

DetectionMode
Params::defaultDetectionMode()
{
	return DetectionMode::CONTENT;
}

QDomElement
Params::toXml(QDomDocument& doc, QString const& name) const
{
	
	XmlMarshaller marshaller(doc);
	
	
	QDomElement el(doc.createElement(name));
	el.setAttribute("detectionMode", m_mode.toString());
	el.setAttribute("fine-tune-corners", m_fineTuneCorners ? "true" : "false");
	el.appendChild(m_contentBox.toXml(doc, "content-box"));
	el.appendChild(marshaller.sizeF(m_contentSizePx, "content-size-px"));
	el.appendChild(m_deps.toXml(doc, "dependencies"));
	return el;
}

void
Params::takeManualSettingsFrom(Params const& other)
{
	// These settings are specified manually,
	// so we want to preserve them after a dependency mismatch.
	m_mode = other.detectionMode();
	m_fineTuneCorners = other.isFineTuningEnabled();
}

} // namespace content_rect
