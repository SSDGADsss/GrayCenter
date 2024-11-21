#ifndef GRAYCENTER_H
#define GRAYCENTER_H

#include <condition_variable>
#include <deque>
#include <filesystem>
#include <functional>
#include <initializer_list>
#include <memory>
#include <vector>

namespace GrayCenterLib {

class Context {
  friend class GrayCenter;
  struct Data {
    int Width;
    int Height;
    int X;
    int Y;
  };
  std::deque<Data> trackList;

  /**
   * @brief 遍历所有跟踪点
   * @note
   * 处理函数中，ROI的长和宽不会改变，但可以修改中心点坐标XY
   *
   * @param workfunc 处理函数
   */
  void foreach (
      std::function<void(int width, int height, int &X, int &Y)> workfunc);

public:
  struct InitItem {
    int StartX;
    int StartY;
    int EndX;
    int EndY;
  };

  /**
   * @brief 获取计算值
   */
  std::vector<std::pair<int, int>> GetPoint();

  /**
   * @brief 从数组初始化构造函数
   *
   * @param initlist 构造列表
   */
  Context(std::vector<InitItem> &&initlist);

  /**
   * @brief 大括号初始化构造函数
   *
   * @param initlist 构造列表
   */
  Context(std::initializer_list<InitItem> initlist);

  /**
   * @brief 初始化一个空的上下文
   */
  Context();

  /**
   * @brief 添加一个点
   *
   * @param width ROI宽度
   * @param height ROI高度
   * @param X ROI中心X坐标
   * @param Y ROI中心Y坐标
   */
  void Add(int width, int height, int X, int Y);

  /**
   * @brief 删除一个点
   *
   * @param index 点的编号
   */
  void Delete(int index);
};

struct ImageData {
  int width;
  int height;
  unsigned char *data;
};

class CalCore {
  friend class GrayCenter;
  int threshold;

public:
  CalCore(int _threshold);
  void operator()(const ImageData &data, int width, int height, int &X, int &Y);
  int GetThreshold();
  void SetThreshold(int _threshold);
};

class GrayCenter {
  std::shared_ptr<Context> context;
  std::shared_ptr<CalCore> calcore;

public:
  /**
   * @brief 构造函数
   *
   * @param _context 上下文组件
   * @param _calCore 计算核心
   */
  GrayCenter(std::shared_ptr<Context> _context,
             std::shared_ptr<CalCore> _calCore);
  /**
   * @brief 处理一张图片(从文件中获取)
   *
   * @param imgPath 图片路径
   */
  std::vector<std::pair<int, int>>
  operator()(const std::filesystem::path &imgPath);

  /**
   * @brief 处理一张图片(从内存中获取)
   * @return 由XY中心点坐标组成的数组
   * @param width 图片宽度
   * @param height 图片高度
   * @param data 图片数据(8bit 1channel)
   */
  std::vector<std::pair<int, int>> operator()(int width, int height,
                                              unsigned char *data);

  /**
   * @brief 获取上下文
   */
  std::shared_ptr<Context> GetContext();

  /**
   * @brief 获取计算核心
   */
  std::shared_ptr<CalCore> GetCalCore();
};

}; // namespace GrayCenterLib

#endif
