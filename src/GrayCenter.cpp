#include <GrayCenter.hpp>
#include <algorithm>
#include <cstring>
#include <initializer_list>
#include <memory>
#include <stdexcept>
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

using namespace GrayCenterLib;

Context::Context(std::vector<InitItem> &&initlist)
    : trackList(initlist.size()) {
  std::transform(
      initlist.begin(), initlist.end(), trackList.begin(),
      [](const InitItem &in) -> Data {
        return {std::abs(in.EndX - in.StartX), std::abs(in.EndY - in.StartY),
                (in.StartX + in.EndX) / 2, (in.StartY + in.EndY) / 2};
      });
}

Context::Context(std::initializer_list<InitItem> initlist)
    : trackList(initlist.size()) {
  std::transform(
      initlist.begin(), initlist.end(), trackList.begin(),
      [](const InitItem &in) -> Data {
        return {std::abs(in.EndX - in.StartX), std::abs(in.EndY - in.StartY),
                (in.StartX + in.EndX) / 2, (in.StartY + in.EndY) / 2};
      });
}

std::vector<std::pair<int, int>> Context::GetPoint() {
  std::vector<std::pair<int, int>> res(trackList.size());
  std::transform(
      trackList.begin(), trackList.end(), res.begin(),
      [](const Data &data) -> std::pair<int, int> { return {data.X, data.Y}; });
  return res;
}

void Context::foreach (
    std::function<void(int width, int height, int &X, int &Y)> workfunc) {
  for (Data &i : trackList)
    workfunc(i.Width, i.Height, i.X, i.Y);
}

void Context::Add(int width, int height, int X, int Y) {
  trackList.push_back({width, height, X, Y});
}

void Context::Delete(int index) {
  if (index >= trackList.size() || index < 0)
    throw std::runtime_error("Not valid index");
  trackList.erase(trackList.begin() + index);
}

CalCore::CalCore(int _threshold) : threshold(_threshold) {}

void CalCore::operator()(const ImageData &data, int width, int height, int &X,
                         int &Y) {
  if (X - width / 2 <= 0 || X + width / 2 >= data.width - 1)
    throw std::logic_error("Cal Xasix outside");
  if (Y - height / 2 <= 0 || Y + height / 2 >= data.height - 1)
    throw std::logic_error("Cal Yasix outside");

  long sumX = 0, sumY = 0, sumVal = 0;
  for (int yptr = Y - height / 2; yptr < Y + height / 2; yptr++) {
    for (int xptr = X - height / 2; xptr < X + height / 2; xptr++) {
      if (data.data[yptr * data.width + xptr] > threshold) {
        sumX += xptr;
        sumY += yptr;
        sumVal++;
      }
    }
  }

  if (sumVal == 0)
    throw std::runtime_error("Can't find any valid area");
  int resX = sumX / sumVal;
  int resY = sumY / sumVal;
  if (resX - width / 2 > 0 && resX + width / 2 < data.width - 1)
    X = sumX / sumVal;
  if (resY - height / 2 > 0 && resY + height / 2 < data.height - 1)
    Y = sumY / sumVal;
}

int CalCore::GetThreshold() { return threshold; }

void CalCore::SetThreshold(int _threshold) { threshold = _threshold; }

GrayCenter::GrayCenter(std::shared_ptr<Context> _context,
                       std::shared_ptr<CalCore> _calCore)
    : context(_context), calcore(_calCore) {}

std::vector<std::pair<int, int>>
GrayCenter::operator()(const std::filesystem::path &imgpath) {
  if (!std::filesystem::exists(imgpath))
    throw std::runtime_error("Image not exists");
  ImageData imgdata;
  int comp;
  imgdata.data =
      stbi_load(imgpath.c_str(), &imgdata.width, &imgdata.height, &comp, 1);
  if (imgdata.data == nullptr)
    throw std::runtime_error("Can't load image from path");
  context->foreach (std::bind(&CalCore::operator(), calcore.get(), imgdata,
                              std::placeholders::_1, std::placeholders::_2,
                              std::placeholders::_3, std::placeholders::_4));
  stbi_image_free(imgdata.data);
  return context->GetPoint();
}

std::vector<std::pair<int, int>> GrayCenter::operator()(int width, int height,
                                                        unsigned char *data) {
  ImageData imgdata = {width, height, data};
  context->foreach (std::bind(&CalCore::operator(), calcore.get(), imgdata,
                              std::placeholders::_1, std::placeholders::_2,
                              std::placeholders::_3, std::placeholders::_4));
  return context->GetPoint();
}

std::shared_ptr<Context> GrayCenter::GetContext() { return context; }

std::shared_ptr<CalCore> GrayCenter::GetCalCore() { return calcore; }
