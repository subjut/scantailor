/*
    Scan Tailor - Interactive post-processing tool for scanned pages.
    Copyright (C)  Joseph Artsimovich <joseph.artsimovich@gmail.com>
    Copyright (C) 2017  Daniel Just
		
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

#include "DetectionMode.h"
#include <QString>

namespace select_content
{

namespace str
{

static char const MANUAL[] = "manual";
static char const CONTENT[] = "content";
static char const PAGE[] = "page";
static char const IMAGE[] = "image";

}

DetectionMode::DetectionMode(Mode mode)
:	m_mode(mode)
{
}

DetectionMode::DetectionMode(QString const& from_string)
{
	if (from_string == QLatin1String(str::CONTENT)) {
		m_mode = CONTENT;
	} else if (from_string == QLatin1String(str::PAGE)) {
		m_mode = PAGE;
	} else if (from_string == QLatin1String(str::IMAGE)) {
		m_mode = IMAGE;
	} else {
		m_mode = MANUAL;
	}
}

QString
DetectionMode::toString() const
{
	char const* s = str::CONTENT;

	switch (m_mode) {
		case MANUAL:
			s = str::MANUAL;
			break;
		case CONTENT:
			s = str::CONTENT;
			break;
		case PAGE:
			s = str::PAGE;
			break;
		case IMAGE:
			s = str::IMAGE;
			break;
	}

	return QLatin1String(s);
}

} // namespace select_content
