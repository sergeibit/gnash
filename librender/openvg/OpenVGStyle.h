// 
//   Copyright (C) 2005, 2006, 2007, 2008, 2009, 2010 Free Software Foundation, Inc.
// 
// This program is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
// 
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301 USA

#ifndef GNASH_OPENVG_STYLE_H
#define GNASH_OPENVG_STYLE_H

#include "CachedBitmap.h"
#include "GnashImage.h"
#include "Renderer.h"

namespace gnash {

class SolidFill;
class GradientFill;
class BitmapFill;
class rgba; 
 
namespace renderer {

namespace openvg {

/// @note These helper functions are used by the boost::variant used
/// for fill styles. A variant is a C++ style version of the C union.
/// Before accessing any of the data of the variant, we have to use
/// boost::apply_visitor() to bind one of these classes to the style
/// to extract the data.

/// Get the color of a style from the variant
class GetColor : public boost::static_visitor<rgba>
{
public:
    rgba operator()(const SolidFill& f) const {
        return f.color();
    }
    rgba operator()(const GradientFill&) const {
        return rgba();
    }
    rgba operator()(const BitmapFill&) const {
        return rgba();
    }
};

/// Get the fill type. Each fill type has it's own sub types,
/// so we map the sub type name to the fill type name.
class GetType : public boost::static_visitor<SWF::FillType>
{
public:
    SWF::FillType operator()(const SolidFill& f) const {
        return SWF::FILL_SOLID;
    }
    SWF::FillType operator()(const GradientFill& g) const {
        switch (g.type()) {
          case GradientFill::LINEAR:
              return SWF::FILL_LINEAR_GRADIENT;
              break;
          case GradientFill::RADIAL:
              return SWF::FILL_RADIAL_GRADIENT;
              break;
          default:
              break;              
        }
    }
    SWF::FillType operator()(const BitmapFill& b) const {
        switch (b.type()) {
          case BitmapFill::TILED:
              if (b.smoothingPolicy() == BitmapFill::SMOOTHING_OFF) {
                  return SWF::FILL_TILED_BITMAP_HARD;
              } else {
                  return SWF::FILL_TILED_BITMAP;
              }
              break;
          case BitmapFill::CLIPPED:
              if (b.smoothingPolicy() == BitmapFill::SMOOTHING_OFF) {
                  return SWF::FILL_CLIPPED_BITMAP_HARD;
              } else {
                  return SWF::FILL_CLIPPED_BITMAP;
              }
              break;
          default:
              break;
        }
    }
};

/// Get the bitmap data of a style from the variant
class GetBitmap : public boost::static_visitor<CachedBitmap *>
{
public:
    CachedBitmap *operator()(const SolidFill&) const {
        return 0;
    }
    CachedBitmap *operator()(const GradientFill&) const {
        return 0;
    }
    CachedBitmap *operator()(const BitmapFill& b) const {
        return const_cast<CachedBitmap *>(b.bitmap());
    }
};

/// Get the matrix of a style from the variant
class GetMatrix : public boost::static_visitor<SWFMatrix>
{
public:
    SWFMatrix operator()(const SolidFill&) const {
    }
    SWFMatrix operator()(const GradientFill& g) const {
        return g.matrix();
    }
    SWFMatrix operator()(const BitmapFill& b) const {
        return b.matrix();
    }
};

} // namespace gnash::renderer::openvg
} // namespace gnash::renderer
} // namespace gnash

#endif // __RENDER_OPENVG_STYLE_H__

// local Variables:
// mode: C++
// indent-tabs-mode: nil
// End:
