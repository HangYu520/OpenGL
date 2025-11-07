#include "typedef.h"

/*
* -----------------------------
* 1. 图像结构体 (封装 stb_image)
* -----------------------------
*/

// 初始化预定义静态颜色常量
const Image::Color Image::Color::BLACK    =   {0, 0, 0};
const Image::Color Image::Color::RED      =   {255, 0, 0};
const Image::Color Image::Color::GREEN    =   {0, 255, 0};
const Image::Color Image::Color::BLUE     =   {0, 0, 255};
const Image::Color Image::Color::WHITE    =   {255, 255, 255};
const Image::Color Image::Color::YELLOW   =   {255, 255, 0};

Image::Color Image::Color::randColor() // 生成随机颜色
{
    static thread_local std::mt19937 gen(std::random_device{}());
    static std::uniform_int_distribution<int> dis(0, 255);
    return {
        static_cast<unsigned char>(dis(gen)),
        static_cast<unsigned char>(dis(gen)),
        static_cast<unsigned char>(dis(gen))};
};

Image::Image(int w, int h, Channel c)
        : width(w), height(h), channel(c)
{
    image_buffer = new stbi_uc[w * h * c]; // 分配图像内存并初始化为 0
}

void Image::setColor(const Pixel& pixel, const Color& color)
{
    // 设置指定像素的颜色
    stbi_uc* pixel_buffer = image_buffer + (pixel.y * width + pixel.x) * channel;
    pixel_buffer[0] = color.R;
    pixel_buffer[1] = color.G;
    pixel_buffer[2] = color.B;
    pixel_buffer[3] = 255; // 透明度
}

void Image::flipVertical() {
    const size_t bytesPerRow = width * static_cast<size_t>(channel);
    stbi_uc* tempRow = new stbi_uc[bytesPerRow];
    
    for (uint32_t i = 0; i < height / 2; ++i) {
        const size_t topOffset = i * bytesPerRow;
        const size_t bottomOffset = (height - 1 - i) * bytesPerRow;
        
        // 交换上下行数据
        memcpy(tempRow, image_buffer + topOffset, bytesPerRow);
        memcpy(image_buffer + topOffset, image_buffer + bottomOffset, bytesPerRow);
        memcpy(image_buffer + bottomOffset, tempRow, bytesPerRow);
    }
    
    delete[] tempRow;
}

void Image::load(const char* filename) // 加载图像文件
{
    int x, y, c;
    image_buffer = stbi_load(filename, &x, &y, &c, 0);
    if (image_buffer)
    {
        width = x;
        height = y;
        channel = Image::Channel(c);
        spdlog::info("Loaded image from {}, width : {}, height : {}, Channel : {}", filename, width, height, (int) channel);
    }
    else
    {
        spdlog::error("Failed to load image from {}", filename);
        exit(1);
    }
}

void Image::save(const char* filename)
{
    bool is_saved = stbi_write_png(filename, width, height, channel, image_buffer, width * channel); // 写入图像文件
    image_buffer = nullptr; // stbi_write_png() 会自动释放图像数据
    if (is_saved)
        spdlog::info("Image saved to {}", filename);
    else
        spdlog::error("Failed to save image to {}", filename);
}
