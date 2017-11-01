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
#include "ImageLoader.h"
#include <QIODevice>
#include <QImage>
#include <QFile>
#include <QByteArray>

ImageMetadataLoader::Status
PdfReader::readMetadata(QIODevice& device,
	VirtualFunction1<void, ImageMetadata const&>& out)
{
	QByteArray buffer(device.readAll());
	PdfMemDocument pdfDoc;
	pdfDoc.Load(buffer.constData(), (long)buffer.length());
	TCIVecObjects iterator = pdfDoc.GetObjects().begin();

	QSize dimensions(0, 0);
	qint64 width = 0;
	qint64 height = 0;

	while (iterator != pdfDoc.GetObjects().end())
	{
		if ((*iterator)->IsDictionary())
		{
			PdfObject* pObjType = (*iterator)->GetDictionary().GetKey(PdfName::KeyType);
			PdfObject* pObjSubType = (*iterator)->GetDictionary().GetKey(PdfName::KeySubtype);
			if ((pObjType && pObjType->IsName() && (pObjType->GetName().GetName() == "XObject")) ||
				(pObjSubType && pObjSubType->IsName() && (pObjSubType->GetName().GetName() == "Image")))
			{
				width = (*iterator)->GetDictionary().GetKey(PdfName("Width"))->GetNumber();
				height = (*iterator)->GetDictionary().GetKey(PdfName("Height"))->GetNumber();

				if (dimensions.width() < width && dimensions.height() < height) {
					dimensions.setWidth(width);
					dimensions.setHeight(height);
				}

				pdfDoc.FreeObjectMemory(*iterator, true);
			}
		}
		++iterator;
	}
	
	// check size
	if (dimensions.width() >= 1000 && dimensions.height() >= 1000) {
		out(ImageMetadata(dimensions));
		return ImageMetadataLoader::LOADED;
	} else if (dimensions.width() == 0 && dimensions.height() == 0) {
		return ImageMetadataLoader::NO_IMAGES;
	} else {
		return ImageMetadataLoader::IMAGE_TOO_SMALL;
	}
	return ImageMetadataLoader::GENERIC_ERROR;
}

bool PdfReader::seemsLikePdf(QIODevice & device)
{
	// first few bytes of a pdf file: "%PDF-1."
	const char start_header[] = "\x25\x50\x44\x46\x2D\x31\x2E";
	char buffer[8];

	qint64 seen = device.peek(buffer, 8);
	return	(seen >= 7 && memcmp(buffer, start_header, 7) == 0);
}

QImage
PdfReader::readImage(QIODevice& device, int const page_num)
{
	QByteArray buffer(device.readAll());
	PdfMemDocument pdfDoc;
	pdfDoc.Load(buffer.constData(), (long)buffer.length());

	PoDoFo::PdfPagesTree* pTree = pdfDoc.GetPagesTree();
	PoDoFo::PdfPage* pPage = pTree->GetPage(page_num);

	// get the resource object for images on this page
	PdfDictionary resources(
		pPage->GetResources()->GetIndirectKey(PdfName("XObject"))->GetDictionary()
	);

	// get the references to all image objects on this page
	TKeyMap imageReferences = resources.GetKeys();

	PdfObject * pObj = nullptr;

	// stores the image to extract; only the largst one on the page is chosen
	PdfObject * pdfImage = nullptr;
	QSize dimensions(0, 0);
	qint64 width = 0;
	qint64 height = 0;

	// go through all references and extract dimensions of each image
	for (PoDoFo::TKeyMap::iterator it = imageReferences.begin(); it != imageReferences.end(); ++it) {
		// get image object dictionary
		pObj = pPage->GetFromResources(PdfName("XObject"), it->first);

		if (pObj->IsDictionary()) {
			PdfObject* pObjType = pObj->GetDictionary().GetKey(PdfName::KeyType);
			PdfObject* pObjSubType = pObj->GetDictionary().GetKey(PdfName::KeySubtype);

			if ((pObjType && pObjType->IsName() && (pObjType->GetName().GetName() == "XObject")) ||
				(pObjSubType && pObjSubType->IsName() && (pObjSubType->GetName().GetName() == "Image")))
			{

				width = pObj->GetDictionary().GetKey(PdfName("Width"))->GetNumber();
				height = pObj->GetDictionary().GetKey(PdfName("Height"))->GetNumber();

				if (dimensions.width() < width && dimensions.height() < height) {
					dimensions.setWidth(width);
					dimensions.setHeight(height);
					pdfImage = pObj;
				}
			}
		}
	}



	QImage image;

	// extract image and set correct metadata
	if (pdfImage && dimensions.width() >= 1000 && dimensions.height() >= 1000) {
		// We'll just try to get the binary stream into a QIODevice and send it through ImageLoader::load
		PdfMemStream* pStream = dynamic_cast<PdfMemStream*>(pdfImage->GetStream());
		QByteArray imageBuffer(pStream->Get(), pStream->GetLength());
		QDataStream newImage(imageBuffer);

		return ImageLoader::load(*newImage.device(), 0);

	} else {
		// image is too small or no images found on page
		return QImage();
	}

	return QImage();
}

