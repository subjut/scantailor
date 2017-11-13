/*
    Scan Tailor - Interactive post-processing tool for scanned pages.
    Copyright (C) 2007-2017  Joseph Artsimovich <joseph_a@mail.ru>
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

#ifndef CONTENT_DETECTION_MODE_H_
#define CONTENT_DETECTION_MODE_H_

namespace select_content
{
	
class DetectionMode
{
	// Member-wise copying is OK.
public:
	enum Mode
		{
			FIRST,
			MANUAL = FIRST,
			CONTENT,
			PAGE,
			IMAGE,
			LAST = IMAGE
		};
  /**
	 * @brief Constructs a DetectionMode from a numberic value.
	 *
	 * No validation is performed on the argument.
	 */
	DetectionMode(Mode mode = DetectionMode::CONTENT);

	/**
	 * @brief Builds a DetectionMode from a string.
	 *
	 * Unknown string values are silently converted to DetectionMode::AUTO.
	 */
	explicit DetectionMode(QString const& from_string);

	QString toString() const;

	Mode get() const { return m_mode; }

	operator Mode() const { return m_mode; }

	/**
	 * @brief Set the distortion mode to a numeric value.
	 *
	 * No validation is performed on the argument.
	 */
	void set(Mode mode) { m_mode = mode; }
private:
	Mode m_mode;
};

} // namespace select_content
#endif
