//-----------------------------------------------------------------------------
// Torque Game Engine
// Copyright (C) GarageGames.com, Inc.
//-----------------------------------------------------------------------------

#ifndef _COLOR_H_
#define _COLOR_H_

//Includes
#ifndef _PLATFORM_H_
#include "platform/platform.h"
#endif


// Forward Declarations...
class ColorI;

class ColorF
{
  public:
   F32 red;
   F32 green;
   F32 blue;
   F32 alpha;

  public:
   ColorF() { }
   ColorF(const ColorF& in_rCopy);
   ColorF(const F32 in_r,
          const F32 in_g,
          const F32 in_b,
          const F32 in_a = 1.0f);

   void set(const F32 in_r,
            const F32 in_g,
            const F32 in_b,
            const F32 in_a = 1.0f);

   ColorF& operator*=(const ColorF& in_mul);       // Can be useful for lighting
   ColorF  operator*(const ColorF& in_mul) const;
   ColorF& operator+=(const ColorF& in_rAdd);
   ColorF  operator+(const ColorF& in_rAdd) const;
   ColorF& operator-=(const ColorF& in_rSub);
   ColorF  operator-(const ColorF& in_rSub) const;

   ColorF& operator*=(const F32 in_mul);
   ColorF  operator*(const F32 in_mul) const;
   ColorF& operator/=(const F32 in_div);
   ColorF  operator/(const F32 in_div) const;

   ColorF  operator-() const;

   bool operator==(const ColorF&) const;
   bool operator!=(const ColorF&) const;

   operator const F32*() const { return &red; }

   U32 getARGBPack() const;
   U32 getRGBAPack() const;
   U32 getBGRAPack() const;

   operator ColorI() const;

   void interpolate(const ColorF& in_rC1,
                    const ColorF& in_rC2,
                    const F32 in_factor);

   bool isValidColor() const { return (red   >= 0.0f && red   <= 1.0f) &&
                                      (green >= 0.0f && green <= 1.0f) &&
                                      (blue  >= 0.0f && blue  <= 1.0f) &&
                                      (alpha >= 0.0f && alpha <= 1.0f); }
   void clamp();
};


//-------------------------------------- ColorI's are missing some of the operations
//                                        present in ColorF since they cannot recover
//                                        properly from over/underflow.  Also, structure
//                                        designed to be castable to PALETTEENTRY's in
//                                        Win32 environments...
class ColorI
{
  public:
   U8 red;
   U8 green;
   U8 blue;
   U8 alpha;

  public:
   ColorI() { }
   ColorI(const ColorI& in_rCopy);
   ColorI(const U8 in_r,
          const U8 in_g,
          const U8 in_b,
          const U8 in_a = U8(255));

   void set(const U8 in_r,
            const U8 in_g,
            const U8 in_b,
            const U8 in_a = U8(255));

   ColorI& operator*=(const F32 in_mul);
   ColorI  operator*(const F32 in_mul) const;

   bool operator==(const ColorI&) const;
   bool operator!=(const ColorI&) const;

   void interpolate(const ColorI& in_rC1,
                    const ColorI& in_rC2,
                    const F32  in_factor);

   U32 getARGBPack() const;
   U32 getRGBAPack() const;
   U32 getABGRPack() const;

   U32 getBGRPack() const;
   U32 getRGBPack() const;

   U32 getRGBEndian() const;
   U32 getARGBEndian() const;

   U16 get565()  const;
   U16 get4444() const;

   operator ColorF() const;

   operator const U8*() const { return &red; }
};

//------------------------------------------------------------------------------
//-------------------------------------- INLINES (ColorF)
//
inline void ColorF::set(const F32 in_r,
            const F32 in_g,
            const F32 in_b,
            const F32 in_a)
{
   red   = in_r;
   green = in_g;
   blue  = in_b;
   alpha = in_a;
}

void a()
{
	ColorI* test = new ColorI(255, 255, 255);

	test->set(0, 0, 0);
}

inline ColorF::ColorF(const ColorF& in_rCopy)
{
   red   = in_rCopy.red;
   green = in_rCopy.green;
   blue  = in_rCopy.blue;
   alpha = in_rCopy.alpha;
}

inline ColorF::ColorF(const F32 in_r,
               const F32 in_g,
               const F32 in_b,
               const F32 in_a)
{
   set(in_r, in_g, in_b, in_a);
}

inline ColorF& ColorF::operator*=(const ColorF& in_mul)
{
   red   *= in_mul.red;
   green *= in_mul.green;
   blue  *= in_mul.blue;
   alpha *= in_mul.alpha;

   return *this;
}

inline ColorF ColorF::operator*(const ColorF& in_mul) const
{
   return ColorF(red   * in_mul.red,
                 green * in_mul.green,
                 blue  * in_mul.blue,
                 alpha * in_mul.alpha);
}

inline ColorF& ColorF::operator+=(const ColorF& in_rAdd)
{
   red   += in_rAdd.red;
   green += in_rAdd.green;
   blue  += in_rAdd.blue;
   alpha += in_rAdd.alpha;

   return *this;
}

inline ColorF ColorF::operator+(const ColorF& in_rAdd) const
{
   return ColorF(red   + in_rAdd.red,
                  green + in_rAdd.green,
                  blue  + in_rAdd.blue,
                  alpha + in_rAdd.alpha);
}

inline ColorF& ColorF::operator-=(const ColorF& in_rSub)
{
   red   -= in_rSub.red;
   green -= in_rSub.green;
   blue  -= in_rSub.blue;
   alpha -= in_rSub.alpha;

   return *this;
}

inline ColorF ColorF::operator-(const ColorF& in_rSub) const
{
   return ColorF(red   - in_rSub.red,
                 green - in_rSub.green,
                 blue  - in_rSub.blue,
                 alpha - in_rSub.alpha);
}

inline ColorF& ColorF::operator*=(const F32 in_mul)
{
   red   *= in_mul;
   green *= in_mul;
   blue  *= in_mul;
   alpha *= in_mul;

   return *this;
}

inline ColorF ColorF::operator*(const F32 in_mul) const
{
   return ColorF(red   * in_mul,
                  green * in_mul,
                  blue  * in_mul,
                  alpha * in_mul);
}

inline ColorF& ColorF::operator/=(const F32 in_div)
{
   AssertFatal(in_div != 0.0f, "Error, div by zero...");
   F32 inv = 1.0f / in_div;

   red   *= inv;
   green *= inv;
   blue  *= inv;
   alpha *= inv;

   return *this;
}

inline ColorF ColorF::operator/(const F32 in_div) const
{
   AssertFatal(in_div != 0.0f, "Error, div by zero...");
   F32 inv = 1.0f / in_div;

   return ColorF(red * inv,
                  green * inv,
                  blue  * inv,
                  alpha * inv);
}

inline ColorF ColorF::operator-() const
{
   return ColorF(-red, -green, -blue, -alpha);
}

inline bool ColorF::operator==(const ColorF& in_Cmp) const
{
   return (red == in_Cmp.red && green == in_Cmp.green && blue == in_Cmp.blue && alpha == in_Cmp.alpha);
}

inline bool ColorF::operator!=(const ColorF& in_Cmp) const
{
   return (red != in_Cmp.red || green != in_Cmp.green || blue != in_Cmp.blue || alpha != in_Cmp.alpha);
}

inline U32 ColorF::getARGBPack() const
{
   return (U32(alpha * 255.0f + 0.5) << 24) |
          (U32(red   * 255.0f + 0.5) << 16) |
          (U32(green * 255.0f + 0.5) <<  8) |
          (U32(blue  * 255.0f + 0.5) <<  0);
}

inline U32 ColorF::getRGBAPack() const
{
   return (U32(alpha * 255.0f + 0.5) <<  0) |
          (U32(red   * 255.0f + 0.5) << 24) |
          (U32(green * 255.0f + 0.5) << 16) |
          (U32(blue  * 255.0f + 0.5) <<  8);
}

inline U32 ColorF::getBGRAPack() const
{
   return (U32(alpha * 255.0f + 0.5) <<  0) |
          (U32(red   * 255.0f + 0.5) <<  8) |
          (U32(green * 255.0f + 0.5) << 16) |
          (U32(blue  * 255.0f + 0.5) << 24);
}

inline void ColorF::interpolate(const ColorF& in_rC1,
                    const ColorF& in_rC2,
                    const F32  in_factor)
{
   F32 f2 = 1.0f - in_factor;
   red   = (in_rC1.red   * f2) + (in_rC2.red   * in_factor);
   green = (in_rC1.green * f2) + (in_rC2.green * in_factor);
   blue  = (in_rC1.blue  * f2) + (in_rC2.blue  * in_factor);
   alpha = (in_rC1.alpha * f2) + (in_rC2.alpha * in_factor);
}

inline void ColorF::clamp()
{
   if (red > 1.0f)
      red = 1.0f;
   else if (red < 0.0f)
      red = 0.0f;

   if (green > 1.0f)
      green = 1.0f;
   else if (green < 0.0f)
      green = 0.0f;

   if (blue > 1.0f)
      blue = 1.0f;
   else if (blue < 0.0f)
      blue = 0.0f;

   if (alpha > 1.0f)
      alpha = 1.0f;
   else if (alpha < 0.0f)
      alpha = 0.0f;
}

//------------------------------------------------------------------------------
//-------------------------------------- INLINES (ColorI)
//
inline void ColorI::set(const U8 in_r,
            const U8 in_g,
            const U8 in_b,
            const U8 in_a)
{
   red   = in_r;
   green = in_g;
   blue  = in_b;
   alpha = in_a;
}

inline ColorI::ColorI(const ColorI& in_rCopy)
{
   red   = in_rCopy.red;
   green = in_rCopy.green;
   blue  = in_rCopy.blue;
   alpha = in_rCopy.alpha;
}

inline ColorI::ColorI(const U8 in_r,
               const U8 in_g,
               const U8 in_b,
               const U8 in_a)
{
   set(in_r, in_g, in_b, in_a);
}

inline ColorI& ColorI::operator*=(const F32 in_mul)
{
   red   = U8((F32(red)   * in_mul) + 0.5f);
   green = U8((F32(green) * in_mul) + 0.5f);
   blue  = U8((F32(blue)  * in_mul) + 0.5f);
   alpha = U8((F32(alpha) * in_mul) + 0.5f);

   return *this;
}

inline ColorI ColorI::operator*(const F32 in_mul) const
{
   ColorI temp(*this);
   temp *= in_mul;
   return temp;
}

inline bool ColorI::operator==(const ColorI& in_Cmp) const
{
   return (red == in_Cmp.red && green == in_Cmp.green && blue == in_Cmp.blue && alpha == in_Cmp.alpha);
}

inline bool ColorI::operator!=(const ColorI& in_Cmp) const
{
   return (red != in_Cmp.red || green != in_Cmp.green || blue != in_Cmp.blue || alpha != in_Cmp.alpha);
}

inline void ColorI::interpolate(const ColorI& in_rC1,
                    const ColorI& in_rC2,
                    const F32  in_factor)
{
   F32 f2= 1.0f - in_factor;
   red   = U8(((F32(in_rC1.red)   * f2) + (F32(in_rC2.red)   * in_factor)) + 0.5f);
   green = U8(((F32(in_rC1.green) * f2) + (F32(in_rC2.green) * in_factor)) + 0.5f);
   blue  = U8(((F32(in_rC1.blue)  * f2) + (F32(in_rC2.blue)  * in_factor)) + 0.5f);
   alpha = U8(((F32(in_rC1.alpha) * f2) + (F32(in_rC2.alpha) * in_factor)) + 0.5f);
}

inline U32 ColorI::getARGBPack() const
{
   return (U32(alpha) << 24) |
          (U32(red)   << 16) |
          (U32(green) <<  8) |
          (U32(blue)  <<  0);
}

inline U32 ColorI::getRGBAPack() const
{
   return (U32(alpha) <<  0) |
          (U32(red)   << 24) |
          (U32(green) << 16) |
          (U32(blue)  <<  8);
}

inline U32 ColorI::getABGRPack() const
{
   return (U32(alpha) << 24) |
          (U32(red)   << 16) |
          (U32(green) <<  8) |
          (U32(blue)  <<  0);
}


inline U32 ColorI::getBGRPack() const
{
   return (U32(blue)  << 16) |
          (U32(green) <<  8) |
          (U32(red)   <<  0);
}

inline U32 ColorI::getRGBPack() const
{
   return (U32(red)   << 16) |
          (U32(green) <<  8) |
          (U32(blue)  <<  0);
}

inline U32 ColorI::getRGBEndian() const
{
#if defined(TORQUE_BIG_ENDIAN)
      return(getRGBPack());
#else
      return(getBGRPack());
#endif
}

inline U32 ColorI::getARGBEndian() const
{
#if defined(TORQUE_BIG_ENDIAN)
   return(getABGRPack());
#else
   return(getARGBPack());
#endif
}

inline U16 ColorI::get565() const
{
   return U16((U16(red   >> 3) << 11) |
              (U16(green >> 2) <<  5) |
              (U16(blue  >> 3) <<  0));
}

inline U16 ColorI::get4444() const
{
   return U16(U16(U16(alpha >> 4) << 12) |
              U16(U16(red   >> 4) <<  8) |
              U16(U16(green >> 4) <<  4) |
              U16(U16(blue  >> 4) <<  0));
}

//-------------------------------------- INLINE CONVERSION OPERATORS
inline ColorF::operator ColorI() const
{
   return ColorI(U8(red   * 255.0f + 0.5),
                  U8(green * 255.0f + 0.5),
                  U8(blue  * 255.0f + 0.5),
                  U8(alpha * 255.0f + 0.5));
}

inline ColorI::operator ColorF() const
{
   const F32 inv255 = 1.0f / 255.0f;

   return ColorF(F32(red)   * inv255,
                 F32(green) * inv255,
                 F32(blue)  * inv255,
                 F32(alpha) * inv255);
}

#endif //_COLOR_H_
