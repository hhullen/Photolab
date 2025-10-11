#include "colorcorrection.h"

#include <algorithm>
#include <cmath>

#include "support.h"

bool ColorCorrection::MakeSimpleColorCorrection(ConstPixBytes prevPixels,
                                                PixBytes lastPixels,
                                                size_t sizeInBytes,
                                                int brightness, int contrast) {
  auto operatePixel = [&prevPixels, &lastPixels, brightness,
                       contrast](size_t i) {
    auto [oldBlue, oldGreen, oldRed] =
        Support::GetPixChannsBGR(&prevPixels.getData()[i]);
    auto [newBlue, newGreen, newRed] =
        Support::GetPixChannsBGR(&lastPixels.getData()[i]);

    const uchar b = std::clamp<uchar>(oldBlue + brightness, 0, 255);
    const uchar g = std::clamp<uchar>(oldGreen + brightness, 0, 255);
    const uchar r = std::clamp<uchar>(oldRed + brightness, 0, 255);

    newBlue = std::clamp(
        static_cast<int>(((b - 128) * contrast) / kContrastScale + 128), 0,
        255);
    newGreen = std::clamp(
        static_cast<int>(((g - 128) * contrast) / kContrastScale + 128), 0,
        255);
    newRed = std::clamp(
        static_cast<int>(((r - 128) * contrast) / kContrastScale + 128), 0,
        255);
  };

  return runner_.RunPerPixInWorkers(sizeInBytes, operatePixel);
}

bool ColorCorrection::MakeHSVColorCorrection(ConstPixBytes prevPixels,
                                             PixBytes lastPixels,
                                             size_t sizeInBytes, int h, int s,
                                             int v) {
  float newH = static_cast<float>(h);
  float newS = s / 1000.0;
  float newV = v / 1000.0;

  auto processPix = [&prevPixels, &lastPixels, newH, newS, newV,
                     this](size_t i) {
    auto [b, g, r] = Support::GetPixChannsBGR(&prevPixels.getData()[i]);

    auto [h, s, v] = RGBToHSV(r, g, b);

    h = fmod(h + newH + 360.0, 360.0);
    s = std::clamp<float>(s * newS, 0.0, 1.0);
    v = std::clamp<float>(v * newV, 0.0, 1.0);

    auto [rOut, gOut, bOut] = HSVToRGB(h, s, v);

    auto [newBlue, newGreen, newRed] =
        Support::GetPixChannsBGR(&lastPixels.getData()[i]);

    newBlue = bOut;
    newGreen = gOut;
    newRed = rOut;
  };

  return runner_.RunPerPixInWorkers(sizeInBytes, processPix);
}

std::tuple<float, float, float> ColorCorrection::RGBToHSV(uchar r, uchar g,
                                                          uchar b) {
  float rf = r / 255.0;
  float gf = g / 255.0;
  float bf = b / 255.0;

  float maxc = std::max({rf, gf, bf});
  float minc = std::min({rf, gf, bf});
  float delta = maxc - minc;

  float h = 0.0;
  if (delta > 1e-6) {
    if (maxc == rf) {
      h = 60.0 * fmod(((gf - bf) / delta), 6.0);
    } else if (maxc == gf) {
      h = 60.0 * (((bf - rf) / delta) + 2.0);
    } else {
      h = 60.0 * (((rf - gf) / delta) + 4.0);
    }
  }
  if (h < 0)
    h += 360.0;

  float s = (maxc <= 0.0) ? 0.0 : (delta / maxc);
  float v = maxc;

  return {h, s, v};
}

std::tuple<uchar, uchar, uchar> ColorCorrection::HSVToRGB(float h, float s,
                                                          float v) {
  float c = v * s;
  float x = c * (1 - fabs(fmod(h / 60.0, 2) - 1));
  float m = v - c;

  float rf2 = 0, gf2 = 0, bf2 = 0;
  if (h < 60) {
    rf2 = c;
    gf2 = x;
    bf2 = 0;
  } else if (h < 120) {
    rf2 = x;
    gf2 = c;
    bf2 = 0;
  } else if (h < 180) {
    rf2 = 0;
    gf2 = c;
    bf2 = x;
  } else if (h < 240) {
    rf2 = 0;
    gf2 = x;
    bf2 = c;
  } else if (h < 300) {
    rf2 = x;
    gf2 = 0;
    bf2 = c;
  } else {
    rf2 = c;
    gf2 = 0;
    bf2 = x;
  }

  int rOut = std::clamp<int>((rf2 + m) * 255, 0, 255);
  int gOut = std::clamp<int>((gf2 + m) * 255, 0, 255);
  int bOut = std::clamp<int>((bf2 + m) * 255, 0, 255);

  return {rOut, gOut, bOut};
}

bool ColorCorrection::MakeHSLColorCorrection(ConstPixBytes prevPixels,
                                             PixBytes lastPixels,
                                             size_t sizeInBytes, int newH,
                                             int newS, int newL) {
  auto processPix = [this, &prevPixels, &lastPixels, newH, newS,
                     newL](size_t i) {
    auto [b, g, r] = Support::GetPixChannsBGR(&prevPixels.getData()[i]);

    auto [h, s, l] = RGBToHSL(r, g, b);

    h = fmod(h + newH, 360.0f);
    if (h < 0.0f)
      h += 360.0f;

    s = std::clamp(s * (newS / 1000.0f), 0.0f, 1.0f);
    l = std::clamp(l * (newL / 1000.0f), 0.0f, 1.0f);

    auto [rOut, gOut, bOut] = HSLToRGB(h, s, l);

    auto [newBlue, newGreen, newRed] =
        Support::GetPixChannsBGR(&lastPixels.getData()[i]);

    newBlue = bOut;
    newGreen = gOut;
    newRed = rOut;
  };

  return runner_.RunPerPixInWorkers(sizeInBytes, processPix);
}

std::tuple<float, float, float> ColorCorrection::RGBToHSL(uchar r, uchar g,
                                                          uchar b) {
  float rf = r / 255.0;
  float gf = g / 255.0;
  float bf = b / 255.0;

  float cmax = std::max({rf, gf, bf});
  float cmin = std::min({rf, gf, bf});
  float delta = cmax - cmin;

  float h = 0.0f;
  if (delta != 0.0f) {
    if (cmax == rf)
      h = 60.0f * fmod(((gf - bf) / delta), 6.0f);
    else if (cmax == gf)
      h = 60.0f * (((bf - rf) / delta) + 2.0f);
    else
      h = 60.0f * (((rf - gf) / delta) + 4.0f);
  }
  if (h < 0.0f)
    h += 360.0f;

  float l = (cmax + cmin) / 2.0f;

  float s = 0.0f;
  if (delta != 0.0f)
    s = delta / (1.0f - fabs(2.0f * l - 1.0f));

  return {h, s, l};
}

std::tuple<uchar, uchar, uchar> ColorCorrection::HSLToRGB(float h, float s,
                                                          float l) {
  float c = (1.0f - fabs(2.0f * l - 1.0f)) * s;
  float x = c * (1.0f - fabs(fmod(h / 60.0f, 2.0f) - 1.0f));
  float m = l - c / 2.0f;

  float r1, g1, b1;
  if (0 <= h && h < 60) {
    r1 = c;
    g1 = x;
    b1 = 0;
  } else if (60 <= h && h < 120) {
    r1 = x;
    g1 = c;
    b1 = 0;
  } else if (120 <= h && h < 180) {
    r1 = 0;
    g1 = c;
    b1 = x;
  } else if (180 <= h && h < 240) {
    r1 = 0;
    g1 = x;
    b1 = c;
  } else if (240 <= h && h < 300) {
    r1 = x;
    g1 = 0;
    b1 = c;
  } else {
    r1 = c;
    g1 = 0;
    b1 = x;
  }

  uchar rOut = static_cast<uchar>(std::clamp((r1 + m) * 255.0f, 0.0f, 255.0f));
  uchar gOut = static_cast<uchar>(std::clamp((g1 + m) * 255.0f, 0.0f, 255.0f));
  uchar bOut = static_cast<uchar>(std::clamp((b1 + m) * 255.0f, 0.0f, 255.0f));

  return {rOut, gOut, bOut};
}
