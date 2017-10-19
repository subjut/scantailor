/*
    Scan Tailor - Interactive post-processing tool for scanned pages.
    Copyright (C) 2007-2008  Joseph Artsimovich <joseph_a@mail.ru>
		Copyright (C) 2017 Daniel Just
		
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

#ifndef PDFREADER_H_
#define PDFREADER_H_

#include "VirtualFunction.h"

class QIODevice;
class QImage;
class ImageMetadata;

/**
* \brief Extracts Images from pdf files.
*
* Only Images larger than 1000x1000 pixels are extracted.
*/
class PdfReader
{
public:	
	static bool readMetadata(QIODevice& dev,
		VirtualFunction1<void, ImageMetadata const&>& out);
	
	/**
	* \brief Checks, if io device is a valid pdf file.
	*
	* \param device The device to read from.  This device must be
	*        opened for reading and must be seekable.
	* \return true, if file is a valid pdf file, false otherwise.
	*/
	static bool canRead(QIODevice& device, QString const& file_path);

	/**
	* \brief Reads the image from io device to QImage.
	*
	* \param device The device to read from.  This device must be
	*        opened for reading and must be seekable.
	* \param page_num A zero-based page number within a multi-page
	*        Pdf file.
	* \return The resulting image, or a null image in case of failure.
	*/
	static QImage readImage(QIODevice& dev, int page_num = 0, QString const& file_path);
};

#endif
