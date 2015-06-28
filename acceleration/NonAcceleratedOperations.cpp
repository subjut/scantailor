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

#include "NonAcceleratedOperations.h"
#include "imageproc/RasterOpGeneric.h"
#include "imageproc/GaussBlur.h"
#include <QPoint>
#include <QPointF>
#include <QRect>
#include <QSize>
#include <limits>

using namespace imageproc;

Grid<float>
NonAcceleratedOperations::anisotropicGaussBlur(
	Grid<float> const& src, float dir_x, float dir_y,
	float dir_sigma, float ortho_dir_sigma) const
{
	Grid<float> dst(src.width(), src.height());

	anisotropicGaussBlurGeneric(
		QSize(src.width(), src.height()), dir_x, dir_y, dir_sigma, ortho_dir_sigma,
		src.data(), src.stride(), [](float val) { return val; },
		dst.data(), dst.stride(), [](float& dst, float src) { dst = src; }
	);

	return dst;
}

Grid<float>
NonAcceleratedOperations::textFilterBank(
	Grid<float> const& src, std::vector<Vec2f> const& directions,
	std::vector<Vec2f> const& sigmas, float shoulder_length) const
{
	Grid<float> accum(src.width(), src.height(), /*padding=*/0);
	accum.initInterior(-std::numeric_limits<float>::max());
	QRect const rect(0, 0, src.width(), src.height());

	for (Vec2f const& s : sigmas) {
		for (Vec2f const& dir : directions) {

			//status.throwIfCancelled();

			Grid<float> blurred(src.width(), src.height(), /*padding=*/0);
			anisotropicGaussBlurGeneric(
				QSize(src.width(), src.height()), dir[0], dir[1], s[0], s[1],
				src.data(), src.stride(), [](float val) { return val; },
				blurred.data(), blurred.stride(), [](float& dst, float src) { dst = src; }
			);

			QPointF shoulder_f(dir[1], -dir[0]);
			shoulder_f *= s[1] * shoulder_length;
			QPoint const shoulder_i(shoulder_f.toPoint());

			rasterOpGenericXY(
				[rect, shoulder_i, &blurred](float& accum, float const origin_px, int x, int y) {
					QPoint const origin(x, y);
					QPoint const pt1(origin + shoulder_i);
					QPoint const pt2(origin - shoulder_i);

					float pt1_px = origin_px;
					float pt2_px = origin_px;

					if (rect.contains(pt1)) {
						pt1_px = blurred(pt1.x(), pt1.y());
					}
					if (rect.contains(pt2)) {
						pt2_px = blurred(pt2.x(), pt2.y());
					}

					float const response = 0.5f * (pt1_px + pt2_px) - origin_px;
					accum = std::max(accum, response);
				},
				accum, blurred
			);
		}
	}

	return accum;
}