# GrayCenter

灰度重心算法

## 目标

- 多目标跟踪

- 直观易用的接口

- 干净凝练的用法

- 连通域筛选目标(开发中)

- 高性能(尚未支持SIMD,OpenMP)

- 组合式API

- GUI(开发中)

## 安装

**依赖**

- CMake
- C++20编译器

**编译**

```bash
git clone https://gog.tootdebuger.love/shenzhe/GrayCenter.git --recursive
mkdir build && cd build
cmake ..
make
make install
```

## 用法

### 基本使用

```cpp
//Import namespace
using namespace GrayCenterLib;
//Create GrayCenter object
GrayCenter cal(
  //Set ROI
  std::make_shared<Context>(Context{{288, 570, 401, 672}}),
  //Set Threshold
  std::make_shared<CalCore>(100)
);
//Cal from image file
std::vector<std::pair<int,int>> res = cal("/home/shenzhe/Code/ImageExample/tezheng/00.bmp");
//Cal from memory
int width,height;
unsigned char * data;
//Set data
// ------
//Cal it!
std::vector<std::pair<int,int>> res = cal(width,height,data);
```

> 算法默认只能处理单通道8bit图片，如果是彩色图片文件，会自动转为单通道

### 多个目标

```cpp
//Style1
Context context={
  {ROIX1,ROIY1,ROIX2,ROIY2},
  {623,334,711,395},
};

//Style2
Context context({
  {ROIX1,ROIY1,ROIX2,ROIY2},
  {623,334,711,395},
})

//Style3
Context context;
context.Add(ROIWidth,ROIHeight,CenterX,CenterY);
context.Delete(0);
```
