#include <GrayCenter.hpp>
#include <iostream>
#include <memory>

int main() {
  using namespace GrayCenterLib;
  Context context({
      {288, 570, 401, 672},
      {623, 334, 711, 395},
  });
  GrayCenter cal(std::make_shared<Context>(Context{
                     {288, 570, 401, 672},
                     {623, 334, 711, 395},
                 }),
                 std::make_shared<CalCore>(100));
  auto res = cal("/home/shenzhe/Code/ImageExample/tezheng/00.bmp");
  std::cout << res[0].first << " - " << res[0].second << std::endl;
  std::cout << res[1].first << " - " << res[1].second << std::endl;
  cal.GetContext()->Delete(0);
  res = cal("/home/shenzhe/Code/ImageExample/tezheng/00.bmp");
  std::cout << res.size() << std::endl;
}
