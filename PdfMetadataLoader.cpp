/*
    Scan Tailor - Interactive post-processing tool for scanned pages.
	  Copyright (C) 2007-2009  Joseph Artsimovich <joseph_a@mail.ru>
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

#include "PdfMetadataLoader.h"
#include "PdfReader.h"

void
PdfMetadataLoader::registerMyself()
{
	static bool registered = false;
	if (!registered) {
		ImageMetadataLoader::registerLoader(
			IntrusivePtr<ImageMetadataLoader>(new PdfMetadataLoader)
		);
		registered = true;
	}
}

ImageMetadataLoader::Status
PdfMetadataLoader::loadMetadata(
	QIODevice& io_device,
	VirtualFunction1<void, ImageMetadata const&>& out)
{
	if (!PdfReader::seemsLikePdf(io_device))
		return ImageMetadataLoader::FORMAT_NOT_RECOGNIZED;
	if (!PdfReader::readMetadata(io_device, out))
		return ImageMetadataLoader::GENERIC_ERROR;
	return ImageMetadataLoader::LOADED;
}
