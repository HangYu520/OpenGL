# 学习 OpenGL

## 依赖
- glfw3 (opengl 窗口， 头文件已包含)
- glad (获取 OpenGL 函数， 函数已包含)
- spdlog (日志库, 头文件已包含)

## 编译
windows + mingw + cmake
mkdir build && cd build
cmake -G "MinGW Makefiles" ..
make

## 参考
[OpenGL 教程](https://learnopengl.com)