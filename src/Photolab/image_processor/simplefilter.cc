#include "simplefilter.h"

#include <algorithm>

#include "image_processor/support.h"

bool SimpleFilter::MakeNegative(ConstPixBytes prevPixels, PixBytes lastPixels,
                                size_t sizeBytes) {
  auto operatePixel = [&lastPixels, &prevPixels](size_t i) {
    auto [oldBlue, oldGreen, oldRed] =
        Support::GetPixChannsBGR(&prevPixels.getData()[i]);
    auto [newBlue, newGreen, newRed] =
        Support::GetPixChannsBGR(&lastPixels.getData()[i]);

    newBlue = 255 - oldBlue;
    newGreen = 255 - oldGreen;
    newRed = 255 - oldRed;
  };

  return runner_.RunPerPixInWorkers(sizeBytes, operatePixel);
}

bool SimpleFilter::MakeBlackAndWhite(ConstPixBytes prevPixels,
                                     PixBytes lastPixels, size_t sizeBytes,
                                     std::string param) {
  void (SimpleFilter::*procFunc)(ConstPixBytes &, PixBytes &, size_t) = nullptr;

  if (param == kAverage) {
    procFunc = &SimpleFilter::MakeAveragePixel;
  } else if (param == kBrightness) {
    procFunc = &SimpleFilter::MakeByBrightnessPixel;
  } else if (param == kDesaturation) {
    procFunc = &SimpleFilter::MakeByDesaturationPixel;
  } else if (param == kBlue) {
    procFunc = &SimpleFilter::MakeByBlueChanPixel;
  } else if (param == kGreen) {
    procFunc = &SimpleFilter::MakeByGreenChanPixel;
  } else if (param == kRed) {
    procFunc = &SimpleFilter::MakeByRedChanPixel;
  } else {
    return false;
  }

  auto operatePixels = [&prevPixels, &lastPixels, &procFunc, this](size_t i) {
    (this->*procFunc)(prevPixels, lastPixels, i);
  };

  return runner_.RunPerPixInWorkers(sizeBytes, operatePixels);
}

bool SimpleFilter::MakeToning(ConstPixBytes prevPixels, PixBytes lastPixels,
                              size_t sizeBytes, const BytesBGRA color) {
  auto operatePixels = [&lastPixels, &prevPixels, &color, this](size_t i) {
    auto [oldBlue, oldGreen, oldRed] =
        Support::GetPixChannsBGR(&prevPixels.getData()[i]);
    auto [newBlue, newGreen, newRed] =
        Support::GetPixChannsBGR(&lastPixels.getData()[i]);

    newBlue = MakeChanTone(oldBlue, color[kBlueIdx], color[kAlphaIdx]);
    newGreen = MakeChanTone(oldGreen, color[kGreenIdx], color[kAlphaIdx]);
    newRed = MakeChanTone(oldRed, color[kRedIdx], color[kAlphaIdx]);
  };

  return runner_.RunPerPixInWorkers(sizeBytes, operatePixels);
}

uchar SimpleFilter::MakeChanTone(uchar chan, uchar tone, uchar alpha) {
  return (chan * alpha * tone) / 65025 + chan - (alpha * chan) / 255;
}

void SimpleFilter::MakeAveragePixel(ConstPixBytes &prevPixels,
                                    PixBytes &lastPixels, size_t idx) {
  auto [oldBlue, oldGreen, oldRed] =
      Support::GetPixChannsBGR(&prevPixels.getData()[idx]);
  auto [newBlue, newGreen, newRed] =
      Support::GetPixChannsBGR(&lastPixels.getData()[idx]);
  float avg = oldBlue / 3.0 + oldGreen / 3.0 + oldRed / 3.0;

  newBlue = avg;
  newGreen = avg;
  newRed = avg;
}

void SimpleFilter::MakeByBrightnessPixel(ConstPixBytes &prevPixels,
                                         PixBytes &lastPixels, size_t idx) {
  static const float magicRedNumber = 0.299;
  static const float magicGreenNumber = 0.587;
  static const float magicBlueNumber = 0.114;
  auto [oldBlue, oldGreen, oldRed] =
      Support::GetPixChannsBGR(&prevPixels.getData()[idx]);
  auto [newBlue, newGreen, newRed] =
      Support::GetPixChannsBGR(&lastPixels.getData()[idx]);

  float v = oldBlue * magicBlueNumber + oldGreen * magicGreenNumber +
            oldRed * magicRedNumber;

  newBlue = v;
  newGreen = v;
  newRed = v;
}

void SimpleFilter::MakeByDesaturationPixel(ConstPixBytes &prevPixels,
                                           PixBytes &lastPixels, size_t idx) {
  auto [oldBlue, oldGreen, oldRed] =
      Support::GetPixChannsBGR(&prevPixels.getData()[idx]);
  auto [newBlue, newGreen, newRed] =
      Support::GetPixChannsBGR(&lastPixels.getData()[idx]);
  uchar min = std::min(std::min(oldBlue, oldGreen), oldRed);
  uchar max = std::max(std::max(oldBlue, oldGreen), oldRed);

  uchar v = (min + max) / 2;

  newBlue = v;
  newGreen = v;
  newRed = v;
}

void SimpleFilter::MakeByBlueChanPixel(ConstPixBytes &prevPixels,
                                       PixBytes &lastPixels, size_t idx) {
  auto [oldBlue, oldGreen, oldRed] =
      Support::GetPixChannsBGR(&prevPixels.getData()[idx]);
  auto [newBlue, newGreen, newRed] =
      Support::GetPixChannsBGR(&lastPixels.getData()[idx]);
  newBlue = oldBlue;
  newGreen = oldBlue;
  newRed = oldBlue;
}

void SimpleFilter::MakeByGreenChanPixel(ConstPixBytes &prevPixels,
                                        PixBytes &lastPixels, size_t idx) {
  auto [oldBlue, oldGreen, oldRed] =
      Support::GetPixChannsBGR(&prevPixels.getData()[idx]);
  auto [newBlue, newGreen, newRed] =
      Support::GetPixChannsBGR(&lastPixels.getData()[idx]);
  newBlue = oldGreen;
  newGreen = oldGreen;
  newRed = oldGreen;
}

void SimpleFilter::MakeByRedChanPixel(ConstPixBytes &prevPixels,
                                      PixBytes &lastPixels, size_t idx) {
  auto [oldBlue, oldGreen, oldRed] =
      Support::GetPixChannsBGR(&prevPixels.getData()[idx]);
  auto [newBlue, newGreen, newRed] =
      Support::GetPixChannsBGR(&lastPixels.getData()[idx]);
  newBlue = oldRed;
  newGreen = oldRed;
  newRed = oldRed;
}
