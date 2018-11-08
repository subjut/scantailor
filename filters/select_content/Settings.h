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

#ifndef SELECT_CONTENT_SETTINGS_H_
#define SELECT_CONTENT_SETTINGS_H_

#include "RefCountable.h"
#include "NonCopyable.h"
#include "PageId.h"
#include "Params.h"
#include "DetectionMode.h"
#include <QMutex>
#include <memory>
#include <map>
#include <set>

class AbstractRelinker;

namespace select_content
{

class DetectionMode;

class Settings : public RefCountable
{
	DECLARE_NON_COPYABLE(Settings)
public:
	Settings();
	
	virtual ~Settings();
	
	void clear();
	
	void performRelinking(AbstractRelinker const& relinker);

	void setPageParams(PageId const& page_id, Params const& params);
	
	std::auto_ptr<Params> getPageParams(PageId const& page_id) const;
	
	DetectionMode getDetectionMode(PageId const& page_id) const;

	void setDetectionMode(
		std::set<PageId> const& pages, DetectionMode const& detection_mode);

//	void clearPageParams(PageId const& page_id);
	
private:
	typedef std::map<PageId, Params> PerPageParams;
	
	mutable QMutex m_mutex;
	PerPageParams m_perPageParams;
};

} // namespace select_content

#endif
