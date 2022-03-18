#pragma once
#include "vec.h"

static constexpr float SRGB8_lookuptable[256] = {
    0.000000f, 0.000304f, 0.000607f, 0.000911f, 0.001214f, 0.001518f, 0.001821f, 0.002125f,
    0.002428f, 0.002732f, 0.003035f, 0.003347f, 0.003677f, 0.004025f, 0.004391f, 0.004777f,
    0.005182f, 0.005605f, 0.006049f, 0.006512f, 0.006995f, 0.007499f, 0.008023f, 0.008568f,
    0.009134f, 0.009721f, 0.010330f, 0.010960f, 0.011612f, 0.012286f, 0.012983f, 0.013702f,
    0.014444f, 0.015209f, 0.015996f, 0.016807f, 0.017642f, 0.018500f, 0.019382f, 0.020289f,
    0.021219f, 0.022174f, 0.023153f, 0.024158f, 0.025187f, 0.026241f, 0.027321f, 0.028426f,
    0.029557f, 0.030713f, 0.031896f, 0.033105f, 0.034340f, 0.035601f, 0.036889f, 0.038204f,
    0.039546f, 0.040915f, 0.042311f, 0.043735f, 0.045186f, 0.046665f, 0.048172f, 0.049707f,
    0.051269f, 0.052861f, 0.054480f, 0.056128f, 0.057805f, 0.059511f, 0.061246f, 0.063010f,
    0.064803f, 0.066626f, 0.068478f, 0.070360f, 0.072272f, 0.074214f, 0.076185f, 0.078187f,
    0.080220f, 0.082283f, 0.084376f, 0.086500f, 0.088656f, 0.090842f, 0.093059f, 0.095307f,
    0.097587f, 0.099899f, 0.102242f, 0.104616f, 0.107023f, 0.109462f, 0.111932f, 0.114435f,
    0.116971f, 0.119538f, 0.122139f, 0.124772f, 0.127438f, 0.130136f, 0.132868f, 0.135633f,
    0.138432f, 0.141263f, 0.144128f, 0.147027f, 0.149960f, 0.152926f, 0.155926f, 0.158961f,
    0.162029f, 0.165132f, 0.168269f, 0.171441f, 0.174647f, 0.177888f, 0.181164f, 0.184475f,
    0.187821f, 0.191202f, 0.194618f, 0.198069f, 0.201556f, 0.205079f, 0.208637f, 0.212231f,
    0.215861f, 0.219526f, 0.223228f, 0.226966f, 0.230740f, 0.234551f, 0.238398f, 0.242281f,
    0.246201f, 0.250158f, 0.254152f, 0.258183f, 0.262251f, 0.266356f, 0.270498f, 0.274677f,
    0.278894f, 0.283149f, 0.287441f, 0.291771f, 0.296138f, 0.300544f, 0.304987f, 0.309469f,
    0.313989f, 0.318547f, 0.323143f, 0.327778f, 0.332452f, 0.337164f, 0.341914f, 0.346704f,
    0.351533f, 0.356400f, 0.361307f, 0.366253f, 0.371238f, 0.376262f, 0.381326f, 0.386430f,
    0.391573f, 0.396755f, 0.401978f, 0.407240f, 0.412543f, 0.417885f, 0.423268f, 0.428691f,
    0.434154f, 0.439657f, 0.445201f, 0.450786f, 0.456411f, 0.462077f, 0.467784f, 0.473532f,
    0.479320f, 0.485150f, 0.491021f, 0.496933f, 0.502887f, 0.508881f, 0.514918f, 0.520996f,
    0.527115f, 0.533276f, 0.539480f, 0.545725f, 0.552011f, 0.558340f, 0.564712f, 0.571125f,
    0.577581f, 0.584078f, 0.590619f, 0.597202f, 0.603827f, 0.610496f, 0.617207f, 0.623960f,
    0.630757f, 0.637597f, 0.644480f, 0.651406f, 0.658375f, 0.665387f, 0.672443f, 0.679543f,
    0.686685f, 0.693872f, 0.701102f, 0.708376f, 0.715694f, 0.723055f, 0.730461f, 0.737911f,
    0.745404f, 0.752942f, 0.760525f, 0.768151f, 0.775822f, 0.783538f, 0.791298f, 0.799103f,
    0.806952f, 0.814847f, 0.822786f, 0.830770f, 0.838799f, 0.846873f, 0.854993f, 0.863157f,
    0.871367f, 0.879622f, 0.887923f, 0.896269f, 0.904661f, 0.913099f, 0.921582f, 0.930111f,
    0.938686f, 0.947307f, 0.955974f, 0.964686f, 0.973445f, 0.982251f, 0.991102f, 1.000000f,
};


inline Vec4f ConvertFromR10G10B10A2(uint32_t data)
{
  return Vec4f(float((data >> 0) & 0x3ff) / 1023.0f, float((data >> 10) & 0x3ff) / 1023.0f,
               float((data >> 20) & 0x3ff) / 1023.0f, float((data >> 30) & 0x003) / 3.0f);
}

inline Vec4u ConvertFromR10G10B10A2UInt(uint32_t data)
{
  return Vec4u((data >> 0) & 0x3ff, (data >> 10) & 0x3ff, (data >> 20) & 0x3ff, (data >> 30) & 0x003);
}

inline Vec4f ConvertFromR10G10B10A2SNorm(uint32_t data)
{
  int r = int(data >> 0) & 0x3ff;
  int g = int(data >> 10) & 0x3ff;
  int b = int(data >> 20) & 0x3ff;
  int a = int(data >> 30) & 3;

  if(r >= 512)
    r -= 1024;
  if(g >= 512)
    g -= 1024;
  if(b >= 512)
    b -= 1024;
  if(a >= 2)
    a -= 4;

  if(r == -512)
    r = -511;
  if(g == -512)
    g = -511;
  if(b == -512)
    b = -511;
  if(a == -2)
    a = -1;

  return Vec4f(float(r) / 511.0f, float(g) / 511.0f, float(b) / 511.0f, float(a) / 1.0f);
}

inline uint32_t ConvertToR10G10B10A2(Vec4f data)
{
  float x = data.x < 1.0f ? (data.x > 0.0f ? data.x : 0.0f) : 1.0f;
  float y = data.y < 1.0f ? (data.y > 0.0f ? data.y : 0.0f) : 1.0f;
  float z = data.z < 1.0f ? (data.z > 0.0f ? data.z : 0.0f) : 1.0f;
  float w = data.w < 1.0f ? (data.w > 0.0f ? data.w : 0.0f) : 1.0f;

  return (uint32_t(x * 1023) << 0) | (uint32_t(y * 1023) << 10) | (uint32_t(z * 1023) << 20) |
         (uint32_t(w * 3) << 30);
}

inline uint32_t ConvertToR10G10B10A2(Vec4u data)
{
  return (uint32_t(data.x & 0x3ff) << 0) | (uint32_t(data.y & 0x3ff) << 10) |
         (uint32_t(data.z & 0x3ff) << 20) | (uint32_t(data.w & 0x3) << 30);
}

inline uint32_t ConvertToR10G10B10A2SNorm(Vec4f data)
{
  float x = data.x < 1.0f ? (data.x > -1.0f ? data.x : -1.0f) : 1.0f;
  float y = data.y < 1.0f ? (data.y > -1.0f ? data.y : -1.0f) : 1.0f;
  float z = data.z < 1.0f ? (data.z > -1.0f ? data.z : -1.0f) : 1.0f;
  float w = data.w < 1.0f ? (data.w > -1.0f ? data.w : -1.0f) : 1.0f;

  uint32_t xu = x >= 0.0f ? uint32_t(x * 511 + 0.5f) : (1024U + uint32_t(-x * 511 + 0.5f));
  uint32_t yu = y >= 0.0f ? uint32_t(y * 511 + 0.5f) : (1024U + uint32_t(-y * 511 + 0.5f));
  uint32_t zu = z >= 0.0f ? uint32_t(z * 511 + 0.5f) : (1024U + uint32_t(-z * 511 + 0.5f));
  uint32_t wu = w >= 0.0f ? uint32_t(w * 1) : (2U + uint32_t(-z * 1));

  return (uint32_t(xu) << 0) | (uint32_t(yu) << 10) | (uint32_t(zu) << 20) | (uint32_t(wu) << 30);
}

Vec3f ConvertFromR11G11B10(uint32_t data)
{
  uint32_t mantissas[3] = {
      (data >> 0) & 0x3f, (data >> 11) & 0x3f, (data >> 22) & 0x1f,
  };
  int32_t exponents[3] = {
      int32_t(data >> 6) & 0x1f, int32_t(data >> 17) & 0x1f, int32_t(data >> 27) & 0x1f,
  };

  Vec3f ret;
  uint32_t *retu = (uint32_t *)&ret.x;

  // floats have 23 bit mantissa, 8bit exponent
  // R11G11B10 has 6/6/5 bit mantissas, 5bit exponents

  const int mantissaShift[] = {23 - 6, 23 - 6, 23 - 5};

  for(int i = 0; i < 3; i++)
  {
    if(mantissas[i] == 0 && exponents[i] == 0)
    {
      retu[i] = 0;
    }
    else
    {
      if(exponents[i] == 0x1f)
      {
        // infinity or nan
        retu[i] = 0x7f800000 | mantissas[i] << mantissaShift[i];
      }
      else if(exponents[i] != 0)
      {
        // shift exponent and mantissa to the right range for 32bit floats
        retu[i] = (exponents[i] + (127 - 15)) << 23 | mantissas[i] << mantissaShift[i];
      }
      else if(exponents[i] == 0)
      {
        // we know xMantissa isn't 0 also, or it would have been caught above

        exponents[i] = 1;

        const uint32_t hiddenBit = 0x40 >> (i == 2 ? 1 : 0);

        // shift until hidden bit is set
        while((mantissas[i] & hiddenBit) == 0)
        {
          mantissas[i] <<= 1;
          exponents[i]--;
        }

        // remove the hidden bit
        mantissas[i] &= ~hiddenBit;

        retu[i] = (exponents[i] + (127 - 15)) << 23 | mantissas[i] << mantissaShift[i];
      }
    }
  }

  return ret;
}
uint32_t ConvertToR11G11B10(Vec3f data);

inline Vec4f ConvertFromB5G5R5A1(uint16_t data)
{
  return Vec4f((float)((data >> 10) & 0x1f) / 31.0f, (float)((data >> 5) & 0x1f) / 31.0f,
               (float)((data >> 0) & 0x1f) / 31.0f, ((data & 0x8000) > 0) ? 1.0f : 0.0f);
}

inline uint16_t ConvertToB5G5R5A1(Vec4f data)
{
  float x = data.x < 1.0f ? (data.x > 0.0f ? data.x : 0.0f) : 1.0f;
  float y = data.y < 1.0f ? (data.y > 0.0f ? data.y : 0.0f) : 1.0f;
  float z = data.z < 1.0f ? (data.z > 0.0f ? data.z : 0.0f) : 1.0f;
  float w = data.w < 1.0f ? (data.w > 0.0f ? data.w : 0.0f) : 1.0f;

  return (uint16_t(x * 0x1f + 0.5f) << 10) | (uint16_t(y * 0x1f + 0.5f) << 5) |
         (uint16_t(z * 0x1f + 0.5f) << 0) | (uint16_t(w * 0x1 + 0.5f) << 15);
}

inline Vec3f ConvertFromB5G6R5(uint16_t data)
{
  return Vec3f((float)((data >> 11) & 0x1f) / 31.0f, (float)((data >> 5) & 0x3f) / 63.0f,
               (float)((data >> 0) & 0x1f) / 31.0f);
}

inline uint16_t ConvertToB5G6R5(Vec3f data)
{
  float x = data.x < 1.0f ? (data.x > 0.0f ? data.x : 0.0f) : 1.0f;
  float y = data.y < 1.0f ? (data.y > 0.0f ? data.y : 0.0f) : 1.0f;
  float z = data.z < 1.0f ? (data.z > 0.0f ? data.z : 0.0f) : 1.0f;

  return (uint16_t(x * 0x1f + 0.5f) << 11) | (uint16_t(y * 0x3f + 0.5f) << 5) |
         (uint16_t(z * 0x1f + 0.5f) << 0);
}

inline Vec4f ConvertFromB4G4R4A4(uint16_t data)
{
  return Vec4f((float)((data >> 8) & 0xf) / 15.0f, (float)((data >> 4) & 0xf) / 15.0f,
               (float)((data >> 0) & 0xf) / 15.0f, (float)((data >> 12) & 0xf) / 15.0f);
}

inline uint16_t ConvertToB4G4R4A4(Vec4f data)
{
  float x = data.x < 1.0f ? (data.x > 0.0f ? data.x : 0.0f) : 1.0f;
  float y = data.y < 1.0f ? (data.y > 0.0f ? data.y : 0.0f) : 1.0f;
  float z = data.z < 1.0f ? (data.z > 0.0f ? data.z : 0.0f) : 1.0f;
  float w = data.w < 1.0f ? (data.w > 0.0f ? data.w : 0.0f) : 1.0f;

  return (uint16_t(x * 0xf + 0.5f) << 8) | (uint16_t(y * 0xf + 0.5f) << 4) |
         (uint16_t(z * 0xf + 0.5f) << 0) | (uint16_t(w * 0xf + 0.5f) << 12);
}

inline Vec4f ConvertFromR4G4(uint8_t data)
{
  return Vec4f((float)((data >> 0) & 0xf) / 15.0f, (float)((data >> 4) & 0xf) / 15.0f, 0.0f, 0.0f);
}

inline uint8_t ConvertToR4G4(Vec2f data)
{
  float x = data.x < 1.0f ? (data.x > 0.0f ? data.x : 0.0f) : 1.0f;
  float y = data.y < 1.0f ? (data.y > 0.0f ? data.y : 0.0f) : 1.0f;

  return (uint8_t(x * 0xf + 0.5f) << 0) | (uint8_t(y * 0xf + 0.5f) << 4);
}

Vec3f ConvertFromR9G9B9E5(uint32_t data)
{
  // get mantissas
  uint32_t mantissas[] = {
      ((data >> 0) & 0x1ff), ((data >> 9) & 0x1ff), ((data >> 18) & 0x1ff),
  };

  // get shared exponent
  uint32_t exp = ((data >> 27) & 0x1f);

  // none of the mantissas have a leading implicit 1 like normal floats (otherwise the shared
  // exponent would be a bit pointless and all floats would have to be within a power of two of each
  // other).
  // We could shift each mantissa up until the top bit is set, then overflow that into the implicit
  // bit and adjust the exponent along with, then plug these into normal floats.
  // OR we could just manually calculate the effective scale from the exponent and multiply by the
  // mantissas.

  float scale = powf(2.0f, float(exp) - 15.0f);

  // floats have 23 bit mantissa, 8bit exponent
  // R11G11B10 has 6/6/5 bit mantissas, 5bit exponents
  const int mantissaShift = 23 - 9;

  Vec3f ret;
  uint32_t *retu = (uint32_t *)&ret.x;
  float *retf = (float *)&ret.x;

  for(int i = 0; i < 3; i++)
  {
    if(mantissas[i] == 0 && exp == 0)
    {
      retu[i] = 0;
    }
    else
    {
      if(exp == 0x1f)
      {
        // infinity or nan
        retu[i] = 0x7f800000 | mantissas[i] << mantissaShift;
      }
      else
      {
        retf[i] = scale * (float(mantissas[i]) / 512.0f);
      }
    }
  }

  return ret;
}



inline FloatVector DecodeFormattedComponents(const ResourceFormat &fmt, const byte *data, bool *success = nullptr)
{
  FloatVector ret(0.0f, 0.0f, 0.0f, 1.0f);

  if(fmt.compType == CompType::UInt || fmt.compType == CompType::SInt || fmt.compCount == 4)
    ret.w = 0.0f;

  const uint64_t dummy = 0;
  if(!data)
    data = (const byte *)&dummy;

  // assume success, we'll set it to false if we hit an error
  if(success)
    *success = true;

  if(fmt.type == ResourceFormatType::R10G10B10A2)
  {
    Vec4f v;
    if(fmt.compType == CompType::SNorm)
      v = ConvertFromR10G10B10A2SNorm(*(const uint32_t *)data);
    else
      v = ConvertFromR10G10B10A2(*(const uint32_t *)data);
    if(fmt.compType == CompType::UInt)
    {
      v.x *= 1023.0f;
      v.y *= 1023.0f;
      v.z *= 1023.0f;
      v.w *= 3.0f;
    }
    ret.x = v.x;
    ret.y = v.y;
    ret.z = v.z;
    ret.w = v.w;

    if(fmt.BGRAOrder())
      std::swap(ret.x, ret.z);
  }
  else if(fmt.type == ResourceFormatType::R11G11B10)
  {
    Vec3f v = ConvertFromR11G11B10(*(const uint32_t *)data);
    ret.x = v.x;
    ret.y = v.y;
    ret.z = v.z;
  }
  else if(fmt.type == ResourceFormatType::R5G5B5A1)
  {
    Vec4f v = ConvertFromB5G5R5A1(*(const uint16_t *)data);
    ret.x = v.x;
    ret.y = v.y;
    ret.z = v.z;
    ret.w = v.w;

    // conversely we *expect* BGRA order for this format and the above conversion implicitly flips
    // when bit-unpacking. So if the format wasn't BGRA order, flip it back
    if(!fmt.BGRAOrder())
      std::swap(ret.x, ret.z);
  }
  else if(fmt.type == ResourceFormatType::R5G6B5)
  {
    Vec3f v = ConvertFromB5G6R5(*(const uint16_t *)data);
    ret.x = v.x;
    ret.y = v.y;
    ret.z = v.z;

    // conversely we *expect* BGRA order for this format and the above conversion implicitly flips
    // when bit-unpacking. So if the format wasn't BGRA order, flip it back
    if(!fmt.BGRAOrder())
      std::swap(ret.x, ret.z);
  }
  else if(fmt.type == ResourceFormatType::R4G4B4A4)
  {
    Vec4f v = ConvertFromB4G4R4A4(*(const uint16_t *)data);
    ret.x = v.x;
    ret.y = v.y;
    ret.z = v.z;
    ret.w = v.w;

    // conversely we *expect* BGRA order for this format and the above conversion implicitly flips
    // when bit-unpacking. So if the format wasn't BGRA order, flip it back
    if(!fmt.BGRAOrder())
      std::swap(ret.x, ret.z);
  }
  else if(fmt.type == ResourceFormatType::R4G4)
  {
    Vec4f v = ConvertFromR4G4(*(const uint8_t *)data);
    ret.x = v.x;
    ret.y = v.y;
  }
  else if(fmt.type == ResourceFormatType::R9G9B9E5)
  {
    Vec3f v = ConvertFromR9G9B9E5(*(const uint32_t *)data);
    ret.x = v.x;
    ret.y = v.y;
    ret.z = v.z;
  }
  else if(fmt.type == ResourceFormatType::D16S8)
  {
    uint32_t val = *(const uint32_t *)data;
    ret.x = float(val & 0x00ffff) / 65535.0f;
    ret.y = float((val & 0xff0000) >> 16) / 255.0f;
    ret.z = 0.0f;
  }
  else if(fmt.type == ResourceFormatType::D24S8)
  {
    uint32_t val = *(const uint32_t *)data;
    ret.x = float(val & 0x00ffffff) / 16777215.0f;
    ret.y = float((val & 0xff000000) >> 24) / 255.0f;
    ret.z = 0.0f;
  }
  else if(fmt.type == ResourceFormatType::D32S8)
  {
    struct ds
    {
      float f;
      uint32_t s;
    } val;
    val = *(const ds *)data;
    ret.x = val.f;
    ret.y = float(val.s) / 255.0f;
    ret.z = 0.0f;
  }
  else if(fmt.type == ResourceFormatType::Regular || fmt.type == ResourceFormatType::A8 ||
          fmt.type == ResourceFormatType::S8)
  {
    float *comp = &ret.x;

    CompType compType = fmt.compType;
    for(size_t c = 0; c < fmt.compCount; c++)
    {
      // alpha is never interpreted as sRGB
      if(compType == CompType::UNormSRGB && c == 3)
        compType = CompType::UNorm;

      if(fmt.compByteWidth == 8)
      {
        // we just downcast
        const uint64_t *u64 = (const uint64_t *)data;
        const int64_t *i64 = (const int64_t *)data;

        if(compType == CompType::Float)
        {
          *comp = float(*(const double *)u64);
        }
        else if(compType == CompType::UInt || compType == CompType::UScaled)
        {
          *comp = float(*u64);
        }
        else if(compType == CompType::SInt || compType == CompType::SScaled)
        {
          *comp = float(*i64);
        }
        else
        {
          if(success)
            *success = false;
        }
      }
      else if(fmt.compByteWidth == 4)
      {
        const uint32_t *u32 = (const uint32_t *)data;
        const int32_t *i32 = (const int32_t *)data;

        if(compType == CompType::Float || compType == CompType::Depth)
        {
          *comp = *(const float *)u32;
        }
        else if(compType == CompType::UInt || compType == CompType::UScaled)
        {
          *comp = float(*u32);
        }
        else if(compType == CompType::SInt || compType == CompType::SScaled)
        {
          *comp = float(*i32);
        }
        else
        {
          if(success)
            *success = false;
        }
      }
      else if(fmt.compByteWidth == 3 && compType == CompType::Depth)
      {
        // 24-bit depth is a weird edge case we need to assemble it by hand
        const uint8_t *u8 = (const uint8_t *)data;

        uint32_t depth = 0;
        depth |= uint32_t(u8[0]);
        depth |= uint32_t(u8[1]) << 8;
        depth |= uint32_t(u8[2]) << 16;

        *comp = float(depth) / float(16777215.0f);
      }
      else if(fmt.compByteWidth == 2)
      {
        const uint16_t *u16 = (const uint16_t *)data;
        const int16_t *i16 = (const int16_t *)data;

        if(compType == CompType::Float)
        {
            //TODO:
          //*comp = ConvertFromHalf(*u16);
        }
        else if(compType == CompType::UInt || compType == CompType::UScaled)
        {
          *comp = float(*u16);
        }
        else if(compType == CompType::SInt || compType == CompType::SScaled)
        {
          *comp = float(*i16);
        }
        // 16-bit depth is UNORM
        else if(compType == CompType::UNorm || compType == CompType::Depth)
        {
          *comp = float(*u16) / 65535.0f;
        }
        else if(compType == CompType::SNorm)
        {
          float f = -1.0f;

          if(*i16 == -32768)
            f = -1.0f;
          else
            f = ((float)*i16) / 32767.0f;

          *comp = f;
        }
        else
        {
          if(success)
            *success = false;
        }
      }
      else if(fmt.compByteWidth == 1)
      {
        const uint8_t *u8 = (const uint8_t *)data;
        const int8_t *i8 = (const int8_t *)data;

        if(compType == CompType::UInt || compType == CompType::UScaled)
        {
          *comp = float(*u8);
        }
        else if(compType == CompType::SInt || compType == CompType::SScaled)
        {
          *comp = float(*i8);
        }
        else if(compType == CompType::UNormSRGB)
        {
          *comp = SRGB8_lookuptable[*u8];
        }
        else if(compType == CompType::UNorm)
        {
          *comp = float(*u8) / 255.0f;
        }
        else if(compType == CompType::SNorm)
        {
          float f = -1.0f;

          if(*i8 == -128)
            f = -1.0f;
          else
            f = ((float)*i8) / 127.0f;

          *comp = f;
        }
        else
        {
          if(success)
            *success = false;
        }
      }
      else
      {
        //RDCERR("Unexpected format to convert from %u %u", fmt.compByteWidth, compType);
      }

      comp++;
      data += fmt.compByteWidth;
    }

    if(fmt.type == ResourceFormatType::A8)
    {
      ret.w = ret.x;
      ret.x = 0.0f;
    }
    else if(fmt.type == ResourceFormatType::S8)
    {
      ret.y = ret.x;
      ret.x = 0.0f;
    }

    if(fmt.BGRAOrder())
      std::swap(ret.x, ret.z);
  }
  else
  {
    if(success)
      *success = false;
  }

  return ret;
}
