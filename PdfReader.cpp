/*
    Scan Tailor - Interactive post-processing tool for scanned pages.
    Copyright (C) 2008-2017 Joseph Artsimovich <joseph.artsimovich@gmail.com>
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

#include "PdfReader.h"
#include "ImageMetadata.h"
#include <QIODevice>
#include <QImage>
#include <podofo.h>

bool PdfReader::readMetadata(QIODevice& device,
	VirtualFunction1<void, ImageMetadata const&>& out)
{
	return false;
}

QImage
PdfReader::readImage(QIODevice& device)
{
	return QImage();
}
